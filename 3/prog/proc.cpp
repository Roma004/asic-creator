#include "asic-runner/loader.hpp"
#include "asic-runner/runner.hpp"
#include <logger/logger.hpp>

int main() {
    ASICLoader asic("prog/asic.json", "prog/mods");
    ASICRunner runner(asic);

    info.write("CPUs registered: {}", asic.get_cpu_handlers().size());
    info.write("Transfers registered: {}", asic.get_transfers().size());
    info.write(
        "Peripherals registered: {}", asic.get_peripheral_handlers().size()
    );

    runner.run();
    runner.wait_for_complete();

    info.write("Exit normally{}", "");
}
