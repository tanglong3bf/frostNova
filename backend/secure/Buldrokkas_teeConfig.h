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

class JwtLoginCheckFilter : public drogon::HttpFilter<JwtLoginCheckFilter>
{
    virtual void doFilter(const drogon::HttpRequestPtr &req,
                          drogon::FilterCallback &&fcb,
                          drogon::FilterChainCallback &&fccb) override;
};
