#include "asic-engine/handler-device.hpp"
#include <asic-engine/asic/loader.hpp>
#include <asic-engine/asic/runner.hpp>
#include <memory>
#include <stdexcept>
#include <thread>
#include <vector>

ASICRunner::ASICRunner(ASICLoader &loader) : loader(loader) {}

void ASICRunner::run() {
    run_peripherals();
    run_transfers();
    for (auto &cpu : loader.get_cpu_handlers()) run_cpu(cpu);
}

void ASICRunner::wait_for_complete() {
    while (!stop && !loader.cpus_stopped());
    stop = true;
    for (auto &thr : threads) thr->join();
    if (!stop_reason.empty())
        throw std::runtime_error(std::format(
            "ASICRunner: One of threads got exception: {}", stop_reason
        ));
}

void ASICRunner::run_cpu(ASICLoader::CPUHandlers &handler) {
    auto func = [this](ASICLoader::CPUHandlers &h) {
        try {
            while (!stop) {
                h.control.handle();
                for (auto &dev : h.executors) dev.get().handle();
            }
        } catch (const std::exception &e) {
            stop = 1;
            stop_reason =
                std::format("Got exeception from cpu {}: {}", h.name, e.what());
        }
    };
    threads.emplace_back(std::make_unique<std::thread>(func, std::ref(handler))
    );
}

void ASICRunner::run_transfers() {
    threads.emplace_back(std::make_unique<std::thread>([this]() {
        while (!stop) {
            for (auto &trn : loader.get_transfers()) {
                try {
                    trn.dev.transfer();
                } catch (const std::exception &e) {
                    stop = 1;
                    stop_reason = std::format(
                        "Got exception from bus device {}: {}",
                        trn.name,
                        e.what()
                    );
                }
            }
        }
    }));
}

void ASICRunner::run_peripherals() {
    for (auto &dev : loader.get_peripheral_handlers()) {
        threads.emplace_back(std::make_unique<std::thread>(
            [this](ASICLoader::PeripheralHandler &h) {
                try {
                    while (!stop) { h.peripheral.handle(); }
                } catch (const std::exception &e) {
                    stop = 1;
                    stop_reason = std::format(
                        "Got exception from peripheral {}: {}", h.name, e.what()
                    );
                }
            },
            std::ref(dev)
        ));
    }
}
