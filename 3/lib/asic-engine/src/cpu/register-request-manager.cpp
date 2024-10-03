#include <algorithm>
#include <asic-engine/cpu/abstracts/registers.hpp>
#include <asic-engine/cpu/register-request-manager.hpp>
#include <asic-engine/cpu/register-request.hpp>
#include <functional>
#include <list>
#include <memory>
#include <mutex>

RegisterRequestsManager::RegisterRequestsManager(std::mutex &m) : m(m) {}

void RegisterRequestsManager::add_request(RegisterRequest &req) noexcept {
    std::unique_lock<std::mutex> lock(m);
    auto regs = req.registers();
    using ptr = std::shared_ptr<register_t>;

    std::vector<int> reg_ids(regs.size());
    auto end = reg_ids.begin();

    for (ptr reg : regs) {
        RegisterBlock &reg_blk = reg->get_register_block();
        auto it = std::find(reg_ids.begin(), end, reg->get_register_id());
        if (it != end) {
            reg_blk.borrow_register(*reg);
            continue;
        }
        if (!reg_blk.is_register_locked(reg->get_register_id())) {
            reg_blk.borrow_register(*reg);
            *end = reg->get_register_id();
            ++end;
        }
    }
    if (req.is_ready()) return;
    requests_list.push_back(req);
}

void RegisterRequestsManager::free_registers(RegisterRequest &req) noexcept {
    std::unique_lock<std::mutex> lock(m);
    using ptr = std::shared_ptr<register_t>;

    // перераспределяем владение регистрами, отдавая владение освобождаемого
    // в пользу того, кто раньше стоит за ресурсом в очереди
    auto it = requests_list.begin();
    while (it != requests_list.end()) {
        // если владение всеми регистрами было передано, завераем цикл
        if (std::ranges::all_of(req.registers(), [](ptr reg) {
                return reg->is_aquired();
            }))
            break;
        // если есть возможность, передаём владение регистрами
        send_possession(req, it->get());

        // если текущий запрос из очереди готов к исполнению - извлекаем его
        if (extract_if_ready(it)) continue;
        ++it;
    }

    // освобождаем все регистры, владение которыми не было передано
    for (ptr reg : req.registers()) {
        if (reg->is_borrowed()) reg->get_register_block().aquire_register(*reg);
    }
}

bool RegisterRequestsManager::extract_if_ready(reqlist_t::iterator &it
) noexcept {
    if (it->get().is_ready()) {
        auto next = it;
        ++next;
        requests_list.erase(it);
        it = next;
        return true;
    }
    return false;
}

void RegisterRequestsManager::send_possession(
    RegisterRequest &from, RegisterRequest &to
) noexcept {
    using ptr = std::shared_ptr<register_t>;

    auto from_regs = from.registers();

    // для каждого из регистров запроса на освобождение
    for (ptr from_reg : from_regs) {

        // если ресурсы уже освобождены ранее - пропускаем
        if (from_reg->is_aquired()) continue;

        auto to_regs = to.registers();
        auto it = std::ranges::find_if(to_regs, [from_reg](ptr p) {
            return *p == *from_reg;
        });

        // если ни один из регистров текущего запроса не запрашивает те же
        // ресурсы, что текущий регистр, продолжаем
        if (it == to_regs.end()) continue;

        // освобождаем все ссылки текущего, указывающие на текущий регистр
        for (ptr from_reg1 : from_regs) {
            if (*from_reg1 == *from_reg)
                from_reg->get_register_block().aquire_register(*from_reg);
        }

        // пока находится регистр, равный текущему, отдаём ему владение
        // ресурсом
        do {
            auto to_reg = it->get();
            to_reg->get_register_block().borrow_register(*to_reg);
            ++it;
        } while ((it = std::ranges::find_if(
                      it,
                      to_regs.end(),
                      [from_reg](ptr p) { return *p == *from_reg; }
                  ))
                 != to_regs.end());
    }
}

