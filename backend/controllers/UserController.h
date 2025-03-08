#pragma once

#include <drogon/HttpController.h>
#include <drogon/orm/RestfulController.h>

using namespace drogon;

/// @{ DTO
struct UserQuery
{
    std::string username;
    std::string phoneNumber;
    int status{-1};
    size_t page{1};
    size_t pageSize{10};
    trantor::Date dateRange[2];
};

struct UserLogin
{
    std::string username;
    std::string password;
};

struct UserCreate
{
    std::string username;
    std::string nickname;
    std::string phoneNumber;
};

struct UserBatchDelete
{
    std::vector<uint32_t> user_ids;
    std::string password;
};

/// @}

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
    ADD_METHOD_TO(UserController::newUser, "/user", Post);
    ADD_METHOD_VIA_REGEX(UserController::deleteUser,
                         "/user/([1-9]\\d*)",
                         Delete);
    ADD_METHOD_TO(UserController::batchDeleteUsers, "/user", Delete);

    METHOD_LIST_END
    Task<HttpResponsePtr> login(const HttpRequestPtr req, UserLogin user) const;
    Task<HttpResponsePtr> list(const HttpRequestPtr req,
                               const UserQuery query) const;
    Task<HttpResponsePtr> updateStatus(const HttpRequestPtr req,
                                       const int user_id,
                                       const int status) const;
    Task<HttpResponsePtr> newUser(const HttpRequestPtr req,
                                  const UserCreate user) const;
    Task<HttpResponsePtr> deleteUser(const HttpRequestPtr req,
                                     const int user_id) const;
    Task<HttpResponsePtr> batchDeleteUsers(
        const HttpRequestPtr req,
        UserBatchDelete userIdsAndPassword) const;
};

namespace drogon
{
template <>
UserQuery fromRequest(const HttpRequest &req);
template <>
UserLogin fromRequest(const HttpRequest &req);
template <>
UserCreate fromRequest(const HttpRequest &req);
template <>
UserBatchDelete fromRequest(const HttpRequest &req);
}  // namespace drogon
