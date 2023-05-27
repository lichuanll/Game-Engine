#pragma once
#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();
int main(int argc, char** argv)
{
	Hazel::Log::Init();
	//Hazel::Log::GetCoreLogger()->warn("Initialized Log!");
	//Hazel::Log::GetClientLogger()->info("Hello!");
	HZ_CORE_WARN("Initialized Log!");
	HZ_INFO("Hello!");
	auto app = Hazel::CreateApplication();
	printf("fafafa");
	app->Run();
	delete app;
}
#endif