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

void JwtLoginCheckFilter::doFilter(const drogon::HttpRequestPtr &req,
                                   drogon::FilterCallback &&fcb,
                                   drogon::FilterChainCallback &&fccb)
{
    auto authHeader = req->getHeader("Authorization");
    if (authHeader.empty() || authHeader.find("Bearer ") != 0)
    {
        auto resp = HttpResponse::newHttpResponse(k401Unauthorized,
                                                  drogon::CT_TEXT_PLAIN);
        resp->setBody("缺少 Authorization 请求头，或者其值不合法");
        fcb(resp);
        return;
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
        req->attributes()->insert("user", user);
        req->attributes()->insert("authorities", user.authorities());
        fccb();
        return;
    }
    auto resp =
        HttpResponse::newHttpResponse(k401Unauthorized, drogon::CT_TEXT_PLAIN);
    switch (result.first)
    {
        case ExpiredToken:
            resp->setBody("token 已过期");
            break;
        case InvalidToken:
            resp->setBody("不合法的 token");
            break;
        default:
            resp->setBody("不合法的 token");
            break;
    }
    fcb(resp);
}
