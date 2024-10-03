#pragma once
#include <asic-engine/data-master/abstracts/packet.hpp>
#include <memory>
#include <queue.hpp>

using PacketQueue = ThreadSafeQueue<std::shared_ptr<PacketInterface>>;
