#pragma once

#include "errorCode.h"
#include <exception>
#include <string>

class CustomException : public std::exception
{
  public:
    CustomException(const std::string &message, const int code = -1);

    const char *what() const noexcept override;

    int getCode() const noexcept;

  private:
    std::string message_;
    int code_;
};

class ParamException : public CustomException
{
  public:
    ParamException(const std::string &message,
                   const int code = PARAMETER_ERROR);
};

class AuthorityException : public CustomException
{
  public:
    AuthorityException(const std::string &message,
                       const int code = AUTHORITY_ERROR);
};

class DatabaseException : public CustomException
{
  public:
    DatabaseException(const std::string &message,
                      const int code = DATABASE_ERROR);
};

class BusinessException : public CustomException
{
  public:
    BusinessException(const std::string &message,
                      const int code = BUSINESS_ERROR);
};
