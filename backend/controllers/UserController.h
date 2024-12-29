#pragma once

#include <drogon/HttpController.h>
#include "../models/SysUser.h"

using namespace drogon;

struct UserQuery
{
    std::string username;
    std::string phoneNumber;
    int status;
    size_t page;
    size_t pageSize;
    std::string dateRange[2];
};

class UserController : public drogon::HttpController<UserController>

{
  public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(UserController::login, "/login", Post);
    ADD_METHOD_TO(UserController::list, "/user", Get);

    METHOD_LIST_END
    Task<HttpResponsePtr> login(const HttpRequestPtr req,
                                drogon_model::FrostNova::SysUser user) const;
    Task<HttpResponsePtr> list(const HttpRequestPtr req,
                               const UserQuery query) const;
};

namespace drogon
{
template <>
drogon_model::FrostNova::SysUser fromRequest(const HttpRequest &req);
template <>
UserQuery fromRequest(const HttpRequest &req);
}  // namespace drogon
