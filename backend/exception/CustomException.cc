#include "CustomException.h"
#include <sstream>

CustomException::CustomException(const std::string &message, const int code)
    : message_(message), code_(code)
{
}

const char *CustomException::what() const noexcept
{
    if (what_.empty())
    {
        std::ostringstream oss;
        oss << "[" << code_ << "] " << message_;
        what_ = oss.str();
    }
    return what_.c_str();
}

int CustomException::getCode() const noexcept
{
    return code_;
}
