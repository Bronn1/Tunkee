#pragma once

#include <string>
#include <stdexcept>

namespace core 
{
class DamageCalculatorLogicException : public std::runtime_error
{
public:
    DamageCalculatorLogicException(const std::string& what) : runtime_error(what) {}
    virtual ~DamageCalculatorLogicException() = default;
};

class InvalidUnitException : public std::runtime_error
{
public:
    InvalidUnitException(const std::string& what) : runtime_error(what) {}
    virtual ~InvalidUnitException() = default;
};

class InvalidPlayerIDException : public std::runtime_error
{
public:
    InvalidPlayerIDException(const std::string& what) : runtime_error(what) {}
    virtual ~InvalidPlayerIDException() = default;
};
}// namespace core 