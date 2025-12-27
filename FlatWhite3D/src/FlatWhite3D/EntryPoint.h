#pragma once

#ifdef FW_PLATFORM_WINDOWS

extern FlatWhite::Application* FlatWhite::CreateApplication();

int main(int argc, char** argv)
{
	FlatWhite::Log::Init();
	FW_CORE_WARN("Starting FlatWhite3D Engine...");
	int version = 0;
	FW_INFO("version: {0}", version);
	
	auto app = FlatWhite::CreateApplication();
	app->Run();
	delete app;
}
#endif