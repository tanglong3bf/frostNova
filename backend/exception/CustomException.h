#pragma once

#include <exception>
#include <string>

class CustomException : public std::exception
{
  public:
    CustomException(const std::string &message, const int code);

    const char *what() const noexcept override;

    int getCode() const noexcept;

  private:
    std::string message_;
    int code_;
    mutable std::string what_;
};
