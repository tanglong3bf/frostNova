#pragma once

#include <drogon/HttpController.h>
#include <drogon/orm/RestfulController.h>
#include "../models/SysUser.h"

using namespace drogon;

struct UserQuery
{
    std::string username;
    std::string phoneNumber;
    int status{-1};
    size_t page{1};
    size_t pageSize{10};
    trantor::Date dateRange[2];
};

class UserController : public drogon::HttpController<UserController>,
                       public drogon::RestfulController
{
  public:
    UserController();

  public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(UserController::login, "/login", Post);
    ADD_METHOD_TO(UserController::list, "/user", Get);
    ADD_METHOD_VIA_REGEX(UserController::updateStatus,
                         "/user/([1-9]\\d*)/status/(0|1)",
                         Put);

    METHOD_LIST_END
    Task<HttpResponsePtr> login(const HttpRequestPtr req,
                                drogon_model::FrostNova::SysUser user) const;
    Task<HttpResponsePtr> list(const HttpRequestPtr req,
                               const UserQuery query) const;
    Task<HttpResponsePtr> updateStatus(const HttpRequestPtr req,
                                       const int user_id,
                                       const int status) const;
};

namespace drogon
{
template <>
drogon_model::FrostNova::SysUser fromRequest(const HttpRequest &req);
template <>
UserQuery fromRequest(const HttpRequest &req);
}  // namespace drogon
