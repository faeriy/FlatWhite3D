#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"

namespace FlatWhite {
	class FLATWHITE3D_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()
		{
			// static std::shared_ptr<spdlog::logger> s_Logger;
			return s_CoreLogger;
		};

		inline static std::shared_ptr<spdlog::logger>& GetClientLogger()
		{
			// static std::shared_ptr<spdlog::logger> s_Logger;
			return s_ClientLogger;
		};

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#define FW_CORE_TRACE(...) ::FlatWhite::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define FW_CORE_INFO(...)  ::FlatWhite::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FW_CORE_WARN(...)  ::FlatWhite::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FW_CORE_ERROR(...) ::FlatWhite::Log::GetCoreLogger()->error(__VA_ARGS__)
// #define FW_CORE_FATAL(...) ::FlatWhite::Log::GetCoreLogger()->fatal(__VA_ARGS__)

#define FW_TRACE(...) ::FlatWhite::Log::GetClientLogger()->trace(__VA_ARGS__)
#define FW_INFO(...)  ::FlatWhite::Log::GetClientLogger()->info(__VA_ARGS__)
#define FW_WARN(...)  ::FlatWhite::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FW_ERROR(...) ::FlatWhite::Log::GetClientLogger()->error(__VA_ARGS__)
// #define FW_FATAL(...) ::FlatWhite::Log::GetClientLogger()->fatal(__VA_ARGS__)
