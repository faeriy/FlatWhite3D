#pragma once

#ifdef FW_PLATFORM_WINDOWS

extern FlatWhite::Application* FlatWhite::CreateApplication();

int main(int argc, char** argv)
{
	printf("Inside FlatWhite3D Engine!\n");
	auto app = FlatWhite::CreateApplication();
	app->Run();
	delete app;
}
#endif