#pragma once

#include "asic-runner/loader.hpp"
#include <future>
#include <vector>

class ASICRunner {
  public:
    ASICRunner(ASICLoader &loader);
    ASICRunner(const ASICLoader &) = delete;
    ASICRunner(ASICLoader &&) = delete;

    ASICRunner &operator=(const ASICRunner &) = delete;
    ASICRunner &operator=(ASICRunner &&) = delete;

    void run();

    void wait_for_complete();

  private:
    void run_cpu(ASICLoader::CPUHandlers &handler);

    void run_transfers();

    void run_peripherals();

    ASICLoader &loader;
    std::vector<std::future<void>> results;
    std::atomic_bool stop;
};
