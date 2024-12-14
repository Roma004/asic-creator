#pragma once
#include "concurent-queue.hpp"
#include <asic-engine/data-master/abstracts/packet.hpp>
#include <memory>
#include <concurent-queue.hpp>

using PacketQueue = ConcurentQueue<std::shared_ptr<PacketInterface>>;
