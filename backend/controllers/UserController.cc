#include "../exception/CustomException.h"
#include "../models/SysUser.h"
#include "UserController.h"
#include <Buldrokkas_tee.h>
#include <JwtUtil.h>
#include <drogon/orm/Criteria.h>

using namespace std;
using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::FrostNova;
using namespace trantor;
using namespace tl::secure;
using namespace tl::jwt;

UserController::UserController()
    : RestfulController({"user_id",
                         "username",
                         "password",
                         "nickname",
                         "email",
                         "phone",
                         "gender",
                         "avatar",
                         "status",
                         "is_delete",
                         "create_time",
                         "update_time"})
{
    enableMasquerading({"user_id",
                        "username",
                        "",  // "password"
                        "nickname",
                        "email",
                        "phone",
                        "gender",
                        "avatar",
                        "status",
                        "",  // "is_delete"
                        "create_time",
                        "update_time"});
}

// Add definition of your processing function here
Task<HttpResponsePtr> UserController::login(const HttpRequestPtr req,
                                            UserLogin user) const
{
    auto passwordEncoder = DrClassMap::getSingleInstance<PasswordEncoderBase>();

    CoroMapper<SysUser> mapper(app().getDbClient());
    SysUser userInDb;
    try
    {
        userInDb = co_await mapper.findOne(
            Criteria{SysUser::Cols::_username, user.username} &&
            Criteria{SysUser::Cols::_delete_time, CompareOperator::IsNull});
    }
    catch (const UnexpectedRows &e)
    {
        LOG_ERROR << e.what();
        throw BusinessException("不存在的用户名", USER_NOT_EXITS);
    }

    // 密码错误
    if (userInDb.getValueOfPassword() != passwordEncoder->encode(user.password))
    {
        throw BusinessException("用户名或密码错误，登录失败。", PASSWORD_ERROR);
    }

    // 被禁用的用户，不样登录
    if (userInDb.getValueOfStatus() == 1)
    {
        throw BusinessException("用户已被禁用，请联系管理员。",
                                USER_IS_DISABLED);
    }

    // 生成 JWT token
    auto jwtUtil = app().getPlugin<JwtUtil>();
    Json::Value userData;
    userData["userId"] = userInDb.getValueOfUserId();
    userData["username"] = userInDb.getValueOfUsername();
    // @{ test data TODO: query in db
    userData["authorities"].append("ROLE_admin");
    userData["authorities"].append("auth:user:query");
    userData["authorities"].append("auth:user:insert");
    userData["authorities"].append("auth:user:delete");
    userData["authorities"].append("auth:user:update");
    // @}
    auto token = jwtUtil->encode(userData);

    Json::Value json;
    json["data"]["token"] = token;
    co_return HttpResponse::newHttpJsonResponse(json);
}

Task<HttpResponsePtr> UserController::list(const HttpRequestPtr req,
                                           const UserQuery query) const
{
    LOG_DEBUG << req->query();
    CoroMapper<SysUser> mapper(app().getDbClient());

    // 只查询未被删除的用户
    Criteria criteria =
        Criteria{SysUser::Cols::_delete_time, CompareOperator::IsNull};
    // 依据参数设置其余查询条件（不包括分页）
    if (!query.username.empty())
    {
        criteria = criteria && Criteria{SysUser::Cols::_username,
                                        CompareOperator::Like,
                                        "%" + query.username + "%"};
    }
    if (!query.phoneNumber.empty())
    {
        criteria = criteria && Criteria{SysUser::Cols::_phone,
                                        CompareOperator::Like,
                                        "%" + query.phoneNumber + "%"};
    }
    if (query.status != -1)
    {
        criteria = criteria && Criteria{SysUser::Cols::_status, query.status};
    }
    if (query.dateRange[0].microSecondsSinceEpoch() != 0 &&
        query.dateRange[1].microSecondsSinceEpoch() != 0)
    {
        criteria = criteria &&
                   Criteria{SysUser::Cols::_create_time,
                            CompareOperator::GE,
                            query.dateRange[0]} &&
                   Criteria{SysUser::Cols::_create_time,
                            CompareOperator::LT,
                            query.dateRange[1]};
    }

    size_t total = co_await mapper.count(criteria);
    auto maxPage = (total + query.pageSize - 1) / query.pageSize + (total == 0);
    size_t page = min(query.page, maxPage);

    // 封装返回数据
    Json::Value json;
    json["data"]["list"] = Json::Value(Json::arrayValue);
    // 如果没有数据，则直接返回
    if (total > 0) [[likely]]
    {
        auto userList =
            co_await mapper.paginate(page, query.pageSize).findBy(criteria);
        for (const auto &user : userList)
        {
            json["data"]["list"].append(
                user.toMasqueradedJson(masqueradingVector()));
        }
    }
    json["data"]["total"] = total;
    json["data"]["page"] = page;
    json["data"]["pageSize"] = query.pageSize;
    json["data"]["maxPage"] = maxPage;
    if (query.page > page) [[unlikely]]
    {
        json["message"] = "page超出maxPage，使用最后一页。";
    }
    auto resp = HttpResponse::newHttpJsonResponse(json);
    co_return resp;
}

Task<HttpResponsePtr> UserController::updateStatus(const HttpRequestPtr req,
                                                   const int user_id,
                                                   const int status) const
{
    // TODO: 之后通过角色判断
    if (user_id == 1)
    {
        throw CustomException("不能修改超级管理员的状态");
    }
    CoroMapper<SysUser> mapper(app().getDbClient());
    auto count =
        co_await mapper.updateBy(tuple{SysUser::Cols::_status},
                                 Criteria{SysUser::Cols::_user_id, user_id},
                                 status);
    LOG_DEBUG << "update status count: " << count;
    auto resp = HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
    co_return resp;
}

Task<HttpResponsePtr> UserController::newUser(const HttpRequestPtr req,
                                              const UserCreate user) const
{
    auto passwordEncoder = DrClassMap::getSingleInstance<PasswordEncoderBase>();
    SysUser userInDb;
    userInDb.setUsername(user.username);
    userInDb.setNickname(user.nickname);
    userInDb.setPhone(user.phoneNumber);
    // 默认密码：123456
    userInDb.setPassword(passwordEncoder->encode("123456"));
    userInDb.setCreateTime(Date::now());
    CoroMapper<SysUser> mapper(app().getDbClient());

    userInDb = co_await mapper.insert(userInDb);
    Json::Value json;
    json["data"]["id"] = userInDb.getValueOfUserId();
    auto resp = HttpResponse::newHttpJsonResponse(json);
    resp->setStatusCode(k201Created);
    co_return resp;
}

Task<HttpResponsePtr> UserController::deleteUser(const HttpRequestPtr req,
                                                 const int user_id) const
{
    // 之后通过角色判断
    if (user_id == 1)
    {
        throw CustomException("不能删除超级管理员");
    }
    CoroMapper<SysUser> mapper(app().getDbClient());
    auto count =
        co_await mapper.updateBy(tuple{SysUser::Cols::_delete_time},
                                 Criteria{SysUser::Cols::_user_id, user_id},
                                 Date::now());
    if (count == 0)
    {
        throw BusinessException("用户不存在或已被删除", USER_NOT_EXITS);
    }
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}

Task<HttpResponsePtr> UserController::batchDeleteUsers(
    const HttpRequestPtr req,
    UserBatchDelete userIdsAndPassword) const
{
    CoroMapper<SysUser> mapper(app().getDbClient());
    auto user = req->attributes()->get<User>("user");
    auto currentUserInDb = co_await mapper.findOne(
        Criteria{SysUser::Cols::_username, user.username()});

    // 验证密码
    auto passwordEncoder = DrClassMap::getSingleInstance<PasswordEncoderBase>();
    if (passwordEncoder->encode(userIdsAndPassword.password) !=
        currentUserInDb.getValueOfPassword())
    {
        throw BusinessException("密码错误", PASSWORD_ERROR);
    }

    // 之后通过角色判断
    if (any_of(userIdsAndPassword.user_ids.begin(),
               userIdsAndPassword.user_ids.end(),
               [](const int user_id) { return user_id == 1; }))
    {
        throw CustomException("不能删除超级管理员");
    }
    auto count = co_await mapper.updateBy(tuple{SysUser::Cols::_delete_time},
                                          Criteria{SysUser::Cols::_user_id,
                                                   CompareOperator::In,
                                                   userIdsAndPassword.user_ids},
                                          Date::now());
    if (count == 0)
    {
        throw BusinessException("用户不存在或已被删除", USER_NOT_EXITS);
    }
    else if (count != userIdsAndPassword.user_ids.size())
    {
        Json::Value json;
        json["message"] = "部分用户删除失败";
        co_return HttpResponse::newHttpJsonResponse(json);
    }
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}

template <>
UserQuery drogon::fromRequest(const HttpRequest &req)
{
    UserQuery query;
    auto username = req.getParameter("username");
    if (!username.empty())
    {
        query.username = username;
    }
    auto phoneNumber = req.getParameter("phoneNumber");
    if (!phoneNumber.empty())
    {
        query.phoneNumber = phoneNumber;
    }
    auto status = req.getParameter("status");
    if (!status.empty())
    {
        query.status = stoi(status);
    }

    auto createTimeRange = req.getParameter("dateRange");
    if (!createTimeRange.empty())
    {
        auto createTimes = splitString(createTimeRange, ",");
        auto startDate = splitString(createTimes[0], "-");
        query.dateRange[0] =
            Date(stoi(startDate[0]), stoi(startDate[1]), stoi(startDate[2]));
        auto endDate = splitString(createTimes[1], "-");
        query.dateRange[1] = Date(stoi(endDate[0]),
                                  stoi(endDate[1]),
                                  stoi(endDate[2]))
                                 .after(86400);  // +1 day
    }

    auto pageStr = req.getParameter("page");
    if (!pageStr.empty())
    {
        auto page = stoi(pageStr);
        if (page < 1)
        {
            LOG_INFO << "page 参数错误，使用默认值。";
            page = 1;
        }
        query.page = page;
    }
    auto pageSizeStr = req.getParameter("pageSize");
    if (!pageSizeStr.empty())
    {
        auto pageSize = stoi(pageSizeStr);
        switch (pageSize)
        {
            case 20:
            case 30:
            case 50:
                query.pageSize = pageSize;
                break;
            default:
                LOG_INFO << "pageSize 参数错误，使用默认值。";
            case 10:
                query.pageSize = 10;
                break;
        }
    }
    return query;
}

template <>
UserLogin drogon::fromRequest(const HttpRequest &req)
{
    UserLogin user;
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr)
    {
        throw ParamException("请使用 json 格式传递参数", PARAM_FORMAT_ERROR);
    }
    auto &json = *jsonPtr;
    if (!json.isMember("username"))
    {
        throw ParamException("缺少必备参数：username", PARAM_MISSING);
    }
    auto username = json["username"].asString();
    if (username.size() < 8)
    {
        throw ParamException("用户名长度过短", PARAM_LENGTH_ERROR);
    }
    if (!json.isMember("password"))
    {
        throw ParamException("缺少必备参数：password", PARAM_MISSING);
    }
    auto password = json["password"].asString();
    if (password.size() < 6)
    {
        throw ParamException("密码长度过短", PARAM_LENGTH_ERROR);
    }
    user.username = username;
    user.password = password;
    return user;
}

template <>
UserCreate drogon::fromRequest(const HttpRequest &req)
{
    UserCreate user;
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr)
    {
        throw ParamException("请使用 json 格式传递参数", PARAM_FORMAT_ERROR);
    }
    auto &json = *jsonPtr;
    if (!json.isMember("username"))
    {
        throw ParamException("缺少必备参数：username", PARAM_MISSING);
    }
    auto username = json["username"].asString();
    if (username.size() < 8)
    {
        throw ParamException("用户名长度过短", PARAM_LENGTH_ERROR);
    }
    if (!json.isMember("nickname"))
    {
        throw ParamException("缺少必备参数：nickname", PARAM_MISSING);
    }
    auto nickname = json["nickname"].asString();
    string phoneNumber;
    if (json.isMember("phoneNumber"))
    {
        phoneNumber = json["phoneNumber"].asString();
        if (phoneNumber.size() != 11)
        {
            throw ParamException("手机号码长度错误", PARAM_LENGTH_ERROR);
        }
    }
    user.username = username;
    user.nickname = nickname;
    user.phoneNumber = phoneNumber;
    return user;
}

template <>
UserBatchDelete drogon::fromRequest(const HttpRequest &req)
{
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr)
    {
        throw ParamException("请使用 json 格式传递参数", PARAM_FORMAT_ERROR);
    }
    auto &json = *jsonPtr;
    if (!json.isObject() || !json.isMember("ids") || !json["ids"].isArray())
    {
        throw ParamException("缺少必备参数 ids 或者格式错误",
                             PARAM_FORMAT_ERROR);
    }
    if (!json.isObject() || !json.isMember("password") ||
        !json["password"].isString())
    {
        throw ParamException("缺少必备参数 password 或者格式错误",
                             PARAM_FORMAT_ERROR);
    }
    UserBatchDelete result;
    for (auto &item : json["ids"])
    {
        if (!item.isInt())
        {
            throw ParamException("数组元素应为整数类型", PARAM_FORMAT_ERROR);
        }
        result.user_ids.push_back(item.asUInt());
    }
    result.password = json["password"].asString();
    return result;
}
