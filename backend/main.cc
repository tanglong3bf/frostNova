#include <drogon/drogon.h>
#include <JwtUtil.h>
#include <Buldrokkas_tee.h>

using namespace drogon;
using namespace tl::jwt;

int main()
{
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
