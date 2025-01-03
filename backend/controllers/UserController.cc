#include "UserController.h"
#include <Buldrokkas_tee.h>
#include <JwtUtil.h>
#include <drogon/orm/Criteria.h>

using namespace std;
using namespace drogon::orm;
using namespace drogon_model::FrostNova;
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
                                            SysUser user) const
{
    auto passwordEncoder = DrClassMap::getSingleInstance<PasswordEncoderBase>();

    CoroMapper<SysUser> mapper(app().getDbClient());
    auto userInDb = co_await mapper.findOne(
        Criteria{SysUser::Cols::_username, user.getValueOfUsername()} &&
        Criteria{SysUser::Cols::_is_delete, 0});

    // 密码错误
    if (userInDb.getValueOfPassword() !=
        passwordEncoder->encode(user.getValueOfPassword()))
    {
        Json::Value json;
        json["code"] = -1;
        json["error"] = "用户名或密码错误，登录失败。";
        auto resp = HttpResponse::newHttpJsonResponse(json);
        resp->setStatusCode(drogon::k401Unauthorized);
        co_return resp;
    }

    // 被禁用的用户，不样登录
    if (userInDb.getValueOfStatus() == 1)
    {
        Json::Value json;
        json["error"] = "用户已被禁用，请联系管理员。";
        auto resp = HttpResponse::newHttpJsonResponse(json);
        resp->setStatusCode(drogon::k401Unauthorized);
        co_return resp;
    }

    // 生成 JWT token
    auto jwtUtil = app().getPlugin<JwtUtil>();
    Json::Value userData;
    userData["userId"] = userInDb.getValueOfUserId();
    userData["username"] = userInDb.getValueOfUsername();
    // @{ test data TODO: query in db
    userData["authorities"].append("ROLE_admin");
    userData["authorities"].append("auth:user:query");
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
    Criteria criteria = Criteria{SysUser::Cols::_is_delete, 0};
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
                user.toMasqueradedJson(UserController::masqueradingVector()));
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
    CoroMapper<SysUser> mapper(app().getDbClient());
    auto count =
        co_await mapper.updateBy(std::tuple{SysUser::Cols::_status},
                                 Criteria{SysUser::Cols::_user_id, user_id},
                                 status);
    LOG_DEBUG << "update status count: " << count;
    auto resp = HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
    co_return resp;
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
        query.status = std::stoi(status);
    }

    auto createTimeRange = req.getParameter("dateRange");
    if (!createTimeRange.empty())
    {
        auto createTimes = utils::splitString(createTimeRange, ",");
        auto startDate = utils::splitString(createTimes[0], "-");
        query.dateRange[0] = trantor::Date(std::stoi(startDate[0]),
                                           std::stoi(startDate[1]),
                                           std::stoi(startDate[2]));
        auto endDate = utils::splitString(createTimes[1], "-");
        query.dateRange[1] = trantor::Date(std::stoi(endDate[0]),
                                           std::stoi(endDate[1]),
                                           std::stoi(endDate[2]))
                                 .after(86400);  // +1 day
    }

    auto pageStr = req.getParameter("page");
    if (!pageStr.empty())
    {
        auto page = std::stoi(pageStr);
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
        auto pageSize = std::stoi(pageSizeStr);
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
