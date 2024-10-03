#pragma once
#include <stdexcept>

class KeyError : public std::logic_error {
  public:
    explicit KeyError(const std::string &msg) noexcept :
        std::logic_error(msg) {}
};

class RegisterSetExc : public std::logic_error {
  public:
    explicit RegisterSetExc(const std::string &msg) noexcept :
        std::logic_error(msg) {}
};

class GenericInstructionExc : public std::logic_error {
  public:
    explicit GenericInstructionExc(const std::string &msg) noexcept :
        std::logic_error(msg) {}
};

class DataMasterPortExc : public std::logic_error {
  public:
    explicit DataMasterPortExc(const std::string &msg) noexcept :
        std::logic_error(msg) {}
};
