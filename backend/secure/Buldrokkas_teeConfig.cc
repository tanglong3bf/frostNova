#include "Buldrokkas_teeConfig.h"
#include <JwtUtil.h>
#include "../models/SysUser.h"
#include <drogon/HttpAppFramework.h>
#include <drogon/orm/Mapper.h>

using namespace std;
using namespace drogon;
using namespace drogon::orm;
using namespace tl::secure;
using namespace tl::jwt;
using namespace drogon_model::FrostNova;

User CustomUserService::loadUserByUsername(const string &username) const
{
    Mapper<SysUser> mapper(app().getDbClient());
    auto userInDb = mapper.findOne({SysUser::Cols::_username, username});
    auto user =
        User(userInDb.getValueOfUsername(), userInDb.getValueOfPassword());
    return user;
}

void registerJwtLoginCheckHandler()
{
    auto loginCheckHandler = [](const HttpRequestPtr &req) -> optional<User> {
        auto authHeader = req->getHeader("Authorization");
        if (authHeader.empty() || authHeader.find("Bearer ") != 0)
        {
            return nullopt;
        }

        auto token = authHeader.substr(7);  // remove "Bearer "
        auto jwtUtil = app().getPlugin<JwtUtil>();
        auto result = jwtUtil->decode(token);
        if (result.first == Ok)
        {
            auto userId = result.second->get("userId", "").asString();
            auto username = result.second->get("username", "").asString();

            User user(username, "");
            auto authoritiesJson = result.second->get("authorities", {});
            if (authoritiesJson.isArray() && authoritiesJson.size() > 0)
            {
                for (auto &authority : authoritiesJson)
                {
                    user.addAuthority(authority.asString());
                }
            }
            return user;
        }
        LOG_ERROR << "Failed to decode JWT token: " << to_string(result.first);
        return nullopt;
    };
    auto buldrokkas_tee = app().getPlugin<tl::secure::Buldrokkas_tee>();
    buldrokkas_tee->registerLoginCheckHandler(loginCheckHandler);
}
