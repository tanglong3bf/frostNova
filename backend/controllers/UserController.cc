#include "UserController.h"
#include <Buldrokkas_tee.h>
#include <JwtUtil.h>

using namespace drogon::orm;
using namespace drogon_model::FrostNova;
using namespace tl::secure;
using namespace tl::jwt;

// Add definition of your processing function here
Task<HttpResponsePtr> UserController::login(const HttpRequestPtr req,
                                            SysUser user) const
{
    auto passwordEncoder = DrClassMap::getSingleInstance<PasswordEncoderBase>();

    CoroMapper<SysUser> mapper(app().getDbClient());
    auto userInDb = co_await mapper.findOne(
        Criteria{SysUser::Cols::_username, user.getValueOfUsername()} &&
        Criteria{SysUser::Cols::_status, 0} &&
        Criteria{SysUser::Cols::_is_delete, 0});
    if (userInDb.getValueOfPassword() ==
        passwordEncoder->encode(user.getValueOfPassword()))
    {
        auto jwtUtil = app().getPlugin<JwtUtil>();
        Json::Value userData;
        userData["userId"] = userInDb.getValueOfUserId();
        userData["username"] = userInDb.getValueOfUsername();
        auto token = jwtUtil->encode(userData);

        Json::Value json;
        json["data"]["token"] = token;
        co_return HttpResponse::newHttpJsonResponse(json);
    }
    Json::Value json;
    json["code"] = -1;
    json["error"] = "用户名或密码错误，登录失败。";
    co_return HttpResponse::newHttpJsonResponse(json);
}

template <>
drogon_model::FrostNova::SysUser drogon::fromRequest(const HttpRequest &req)
{
    auto jsonPtr = req.getJsonObject();
    if (!jsonPtr)
    {
        throw std::invalid_argument("不支持的请求体格式，请使用 json。");
    }
    auto json = *jsonPtr;
    drogon_model::FrostNova::SysUser value;
    if (!json.isMember("username") || !json["username"].isString())
    {
        throw std::invalid_argument("缺少必备参数：username。");
    }
    if (json["username"].asString().length() < 8)
    {
        throw std::invalid_argument("用户名长度不能少于 8 个字符。");
    }
    if (!json.isMember("password") || !json["password"].isString())
    {
        throw std::invalid_argument("缺少必备参数：password。");
    }
    if (json["password"].asString().length() < 6)
    {
        throw std::invalid_argument("密码长度不能少于 6 个字符。");
    }
    value.updateByJson(json);
    return value;
}
