#include "Game.h"
#include "game/DemoScene.h"

Game& Game::getInstance() noexcept
{
    return game;
}

void Game::run() noexcept
{
    while (m_isRunning)
    {
        Uint64 startTime = SDL_GetTicks();

        handleEvents();
        update(deltaTime);
        render();

        Uint64 elaspedTime = SDL_GetTicks() - startTime;
        if (elaspedTime < frameTime)
        {
            SDL_Delay(frameTime - elaspedTime);
            deltaTime = frameTime / 1000.0f;
        }
        else
        {
            deltaTime = elaspedTime / 1000.0f;
        }
    }
}

void Game::shutdown() noexcept
{
    m_isRunning = false;
}

void Game::changeScene(Scene* newScene) noexcept
{
    if (m_currentScene != nullptr)
    {
        m_currentScene->clean();
        delete m_currentScene;
    }
    m_currentScene = newScene;
    m_currentScene->init();
}

SDL_Texture* Game::loadTexture(const char* file) noexcept
{
    SDL_Texture* texture = IMG_LoadTexture(m_renderer, file);
    return texture;
}

void Game::renderTexture(SDL_Texture* texture, SDL_FRect* dstRect) noexcept
{
    SDL_RenderTexture(m_renderer, texture, nullptr, dstRect);
}

SDL_Texture* Game::createTextureFromSurface(SDL_Surface* surface) noexcept
{
    return SDL_CreateTextureFromSurface(m_renderer, surface);
}

void Game::toggleFullscreen() noexcept
{
    isFullscreen = !isFullscreen;
    SDL_SetWindowFullscreen(m_wnd, isFullscreen);
}

void Game::enterFullscreen() noexcept
{
    SDL_SetWindowFullscreen(m_wnd, true);
    isFullscreen = true;
}

void Game::quitFullscreen() noexcept
{
    SDL_SetWindowFullscreen(m_wnd, false);
    isFullscreen = false;
}

Game::Game() noexcept
{
    TARGET_FPS = 60;
    frameTime = 1000 / TARGET_FPS;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
    Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3);
    TTF_Init();

    // Replace with your desired window dimensions
    m_wnd = SDL_CreateWindow("FDS_SGF", windowWidth, windowHeight, 0);
    m_renderer = SDL_CreateRenderer(m_wnd, nullptr);

    SDL_SetRenderLogicalPresentation(m_renderer, windowWidth, windowHeight, SDL_LOGICAL_PRESENTATION_STRETCH);
    
    SDL_AudioSpec audio_spec = { MIX_DEFAULT_FORMAT, 2, 2048 };
    Mix_OpenAudio(0, &audio_spec);
    Mix_AllocateChannels(32);

    changeScene(new DemoScene());
}

Game::~Game() noexcept
{
    if (m_currentScene != nullptr)
    {
        m_currentScene->clean();
        delete m_currentScene;
    }

    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_wnd);

    Mix_CloseAudio();

    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}

void Game::handleEvents() noexcept
{
    static SDL_Event event;
    auto state = SDL_GetKeyboardState(nullptr);
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            shutdown();
        }
        m_currentScene->handleEvents(&event);
    }
}

void Game::update(float deltaTime) noexcept
{
    m_currentScene->update(deltaTime);
}

void Game::render() noexcept
{
    SDL_RenderClear(m_renderer);
    m_currentScene->render();
    SDL_RenderPresent(m_renderer);
}
