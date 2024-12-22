#pragma once

#include <drogon/HttpController.h>
#include "../models/SysUser.h"

using namespace drogon;

class UserController : public drogon::HttpController<UserController>
{
  public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(UserController::login, "/login", Post);

    METHOD_LIST_END
    Task<HttpResponsePtr> login(const HttpRequestPtr req,
                                drogon_model::FrostNova::SysUser user) const;
};

namespace drogon
{
template <>
drogon_model::FrostNova::SysUser fromRequest(const HttpRequest &req);
}  // namespace drogon
