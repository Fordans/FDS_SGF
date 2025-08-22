#include "core/Game.h"

#include "spdlog/spdlog.h"

Game Game::game;

#ifdef _DEBUG

int main()
{
    spdlog::set_level(spdlog::level::info);

    Game::getInstance().run();
}

#else

#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    spdlog::set_level(spdlog::level::off);

    Game::getInstance().run();

    return 0;
}

#endif