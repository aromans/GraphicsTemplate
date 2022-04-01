#ifndef GRAPHICSTEMPLATE_LOG_H
#define GRAPHICSTEMPLATE_LOG_H

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <memory>

namespace Engine {

    class Log {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return s_EngineLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    private:
        static std::shared_ptr<spdlog::logger> s_EngineLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };

}

// Engine logger
#define ENG_CORE_TRACE(...) ::Engine::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define ENG_CORE_INFO(...) ::Engine::Log::GetEngineLogger()->info(__VA_ARGS__)
#define ENG_CORE_WARN(...) ::Engine::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define ENG_CORE_ERROR(...) ::Engine::Log::GetEngineLogger()->error(__VA_ARGS__)
#define ENG_CORE_FATAL(...) ::Engine::Log::GetEngineLogger()->critical(__VA_ARGS__)

// Client logger
#define ENG_TRACE(...) ::Engine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define ENG_INFO(...) ::Engine::Log::GetClientLogger()->info(__VA_ARGS__)
#define ENG_WARN(...) ::Engine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ENG_ERROR(...) ::Engine::Log::GetClientLogger()->error(__VA_ARGS__)
#define ENG_FATAL(...) ::Engine::Log::GetClientLogger()->critical(__VA_ARGS__)

#endif //GRAPHICSTEMPLATE_LOG_H
