#pragma once

#include "asic-runner/loader.hpp"
#include <memory>
#include <thread>
#include <vector>

class ASICRunner {
  public:
    ASICRunner(ASICLoader &loader);

    void run();

    void wait_for_complete();

  private:
    void run_cpu(ASICLoader::CPUHandlers &handler);

    void run_transfers();

    void run_peripherals();

    ASICLoader &loader;
    std::vector<std::unique_ptr<std::thread>> threads;
    bool stop;
    std::string stop_reason;
};
