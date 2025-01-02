#pragma once

#include <Buldrokkas_tee.h>

class CustomUserService : public tl::secure::UserService<CustomUserService>
{
  public:
    CustomUserService()
    {
    }

  public:
    tl::secure::User loadUserByUsername(const std::string &) const override;
};

void registerJwtLoginCheckHandler();
