#include <WifiClient/client/application.h>

auto main(int argc, char* argv[]) -> int
{
	WifiClient::Client::Application app(argc, argv);
	return WifiClient::Client::Application::exec();
}
