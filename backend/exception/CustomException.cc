#include "CustomException.h"

using namespace std;

CustomException::CustomException(const std::string &message, const int code)
    : message_(message), code_(code)
{
}

const char *CustomException::what() const noexcept
{
    return message_.c_str();
}

int CustomException::getCode() const noexcept
{
    return code_;
}

ParamException ::ParamException(const std::string &message, const int code)
    : CustomException(message, code)
{
}

AuthorityException ::AuthorityException(const std::string &message,
                                        const int code)
    : CustomException(message, code)
{
}

DatabaseException ::DatabaseException(const std::string &message,
                                      const int code)
    : CustomException(message, code)
{
}

BusinessException ::BusinessException(const std::string &message,
                                      const int code)
    : CustomException(message, code)
{
}
