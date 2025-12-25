#include <FlatWhite.h>


class Sandbox : public FlatWhite::Application
{
	public:
	Sandbox() {}
	~Sandbox() {}
};

FlatWhite::Application* FlatWhite::CreateApplication()
{
	return new Sandbox();
}