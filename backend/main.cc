#include <drogon/drogon.h>
#include <JwtUtil.h>
#include <Buldrokkas_tee.h>

using namespace drogon;
using namespace tl::jwt;

int main()
{
    // CORS
    app().registerSyncAdvice([](const HttpRequestPtr& req) -> HttpResponsePtr {
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
        [](const HttpRequestPtr& req, const HttpResponsePtr& resp) {
            if (resp->getHeader("access-control-allow-origin") == "")
            {
                resp->addHeader("access-control-allow-origin",
                                req->getHeader("origin"));
            }
        });

    app().registerBeginningAdvice([]() {
        auto jwtUtil = app().getPlugin<JwtUtil>();
        if (!jwtUtil)
        {
            LOG_ERROR << "JwtUtil 插件获取失败，请检查配置文件";
        }
        jwtUtil->setSecret("FrostNova");
    });
    // Load config file
    drogon::app().loadConfigFile("../config.yaml");
    // Run HTTP framework,the method will block in the internal event loop
    drogon::app().run();
    return 0;
}
