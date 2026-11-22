#include "Application.h"
#include "Logger.h"
#include "ResourceSystem.h"

#include <exception>
#include <string>

int main()
{
    InitializeLogging("rogalique.log");

    try
    {
        Game::Application application;
        application.run();
        Engine::ResourceSystem::Instance()->Clear();
    }
    catch (const std::exception &exception)
    {
        LOG_ERROR(std::string("Fatal: ") + exception.what());
        return 1;
    }

    return 0;
}
