#include "asic-engine/handler-device.hpp"
#include <asic-runner/loader.hpp>
#include <asic-runner/runner.hpp>
#include <future>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

ASICRunner::ASICRunner(ASICLoader &loader) : loader(loader) {}

void ASICRunner::run() {
    run_peripherals();
    run_transfers();
    for (auto &cpu : loader.get_cpu_handlers()) run_cpu(cpu);
}

void ASICRunner::wait_for_complete() {
    while (!stop && !loader.cpus_stopped());
    if (stop) {
        std::stringstream s;
        for (auto &res : results) {
            if (!res.valid()) continue;
            try {
                res.get();
            } catch (const std::runtime_error &e) {
                throw std::runtime_error(
                    std::format("ASICRunner got exception: {}", e.what())
                );
            }
        }
        return;
    }
    stop = true;
    for (auto &res : results) {
        while (!res.valid());
        res.get();
    }
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
            std::cerr << "sdfsdfsdfsdfsdf 2" << std::endl;
            throw std::runtime_error(
                std::format("Got exeception from cpu {}: {}", h.name, e.what())
            );
        }
    };
    results.emplace_back(std::async(std::launch::async, func, std::ref(handler))
    );
}

void ASICRunner::run_transfers() {
    results.emplace_back(std::async(std::launch::async, [this]() {
        while (!stop) {
            for (auto &trn : loader.get_transfers()) {
                try {
                    trn.dev.transfer();
                } catch (const std::exception &e) {
                    std::cerr << "sdfsdfsdfsdfsdf 1" << std::endl;
                    stop = 1;
                    throw std::runtime_error(std::format(
                        "Got exception from bus device {}: {}",
                        trn.name,
                        e.what()
                    ));
                }
            }
        }
    }));
}

void ASICRunner::run_peripherals() {
    for (auto &dev : loader.get_peripheral_handlers()) {
        results.emplace_back(std::async(
            std::launch::async,
            [this](ASICLoader::PeripheralHandler &h) {
                try {
                    while (!stop) { h.peripheral.handle(); }
                } catch (const std::exception &e) {
                    stop = 1;
                    std::cerr << "sdfsdfsdfsdfsdf 3" << std::endl;
                    throw std::runtime_error(std::format(
                        "Got exception from peripheral {}: {}", h.name, e.what()
                    ));
                }
            },
            std::ref(dev)
        ));
    }
}
