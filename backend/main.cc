#include "./exception/CustomException.h"
#include "./secure/Buldrokkas_teeConfig.h"

#include <drogon/drogon.h>
#include <JwtUtil.h>
#include <Buldrokkas_tee.h>

using namespace drogon;
using namespace tl::jwt;

int main()
{
    // CORS
    app().registerSyncAdvice([](const HttpRequestPtr &req) -> HttpResponsePtr {
        if (req->method() == drogon::Options)
        {
            auto method = req->getHeader("access-control-request-method");
            auto header = req->getHeader("access-control-request-headers");
            auto origin = req->getHeader("origin");

            auto resp = HttpResponse::newHttpResponse();
            resp->addHeader("access-control-allow-methods", method);
            resp->addHeader("access-control-allow-headers", header);
            resp->addHeader("access-control-allow-origin", origin);
            return resp;
        }
        return nullptr;
    });
    app().registerPreSendingAdvice(
        [](const HttpRequestPtr &req, const HttpResponsePtr &resp) {
            if (resp->getHeader("access-control-allow-origin") == "")
            {
                auto origin = req->getHeader("origin");
                resp->addHeader("access-control-allow-origin",
                                origin != "" ? origin : "*");
            }
        });

    // 设置jwt的密钥
    app().registerBeginningAdvice([] {
        auto jwtUtil = app().getPlugin<JwtUtil>();
        if (!jwtUtil)
        {
            LOG_ERROR << "JwtUtil 插件获取失败，请检查配置文件";
        }
        jwtUtil->setSecret("FrostNova");
    });
    // 注册jwt登录检查处理器
    app().registerBeginningAdvice([] { registerJwtLoginCheckHandler(); });

    // 注册全局异常处理器
    app().setExceptionHandler(
        [](const std::exception &e,
           const HttpRequestPtr &req,
           std::function<void(const HttpResponsePtr &)> &&callback) {
            LOG_ERROR << req->path() << " throws an exception: " << e.what();
            Json::Value json;
            const auto *customError = dynamic_cast<const CustomException *>(&e);
            auto statusCode = k500InternalServerError;
            if (customError != nullptr)
            {
                auto code = customError->getCode();
                switch (code)
                {
                    case PARAMETER_ERROR:
                    case PARAM_MISSING:
                    case PARAM_FORMAT_ERROR:
                    case INVALID_PARAM_VALUE:
                    case PARAM_TYPE_MISMATCH:
                        statusCode = k400BadRequest;
                        break;
                    case AUTHORITY_ERROR:
                    case INVALID_TOKEN:
                    case TOKEN_EXPIRED:
                    case PASSWORD_ERROR:
                    case USER_IS_DISABLED:
                        statusCode = k401Unauthorized;
                        break;
                    case USER_NOT_EXITS:
                        statusCode = k404NotFound;
                        break;
                    case USER_IS_EXISTS:
                        statusCode = k409Conflict;
                        break;
                    case PARAM_LENGTH_ERROR:
                        statusCode = k422UnprocessableEntity;
                        break;
                    default:
                        statusCode = k500InternalServerError;
                }
                json["code"] = code;
                json["error"] = customError->what();
            }
            else
            {
                json["code"] = -1;
                json["error"] = "未知错误";
                statusCode = k500InternalServerError;
            }
            auto resp = HttpResponse::newHttpJsonResponse(json);
            resp->setStatusCode(statusCode);
            callback(resp);
            return;
        });

    // Load config file
    drogon::app().loadConfigFile("../config.yaml");
    // Run HTTP framework,the method will block in the internal event loop
    drogon::app().run();
    return 0;
}
