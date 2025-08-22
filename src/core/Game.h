#pragma once

#include "core/Scene.h"

#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

class Game
{
public:
	static Game& getInstance() noexcept;
	void run() noexcept;
	void shutdown() noexcept;
	void changeScene(Scene* newScene) noexcept;
	SDL_Texture* loadTexture(const char* file) noexcept;
	void renderTexture(SDL_Texture* texture, SDL_FRect* dstRect) noexcept;
	SDL_Texture* createTextureFromSurface(SDL_Surface* surface) noexcept;
	void toggleFullscreen() noexcept;
	void enterFullscreen() noexcept;
	void quitFullscreen() noexcept;
private:
	static Game game;
	Game() noexcept;
	~Game() noexcept;
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	void handleEvents() noexcept;
	void update(float deltaTime) noexcept;
	void render() noexcept;
public:
	float windowWidth = 1600;
	float windowHeight = 1200;
	int TARGET_FPS;
	Uint64 frameTime;
	float deltaTime = 0.0f;
	bool isFullscreen = false;
private:
	bool m_isRunning = true;
	SDL_Window* m_wnd;
	SDL_Renderer* m_renderer;
	Scene* m_currentScene;
};

// macros for convenience
#define RUN Game::getInstance().run()
#define LOAD_TEXTURE(file) Game::getInstance().loadTexture(file)
#define RENDER_TEXTURE(SDL_Texture_Pointer, SDL_FRect_Pointer) Game::getInstance().renderTexture(SDL_Texture_Pointer, SDL_FRect_Pointer)
#define WINDOW_WIDTH Game::getInstance().windowWidth
#define WINDOW_HEIGHT Game::getInstance().windowHeight
#define CREATE_TEXTURE_FROM_SURFACE(SDL_Surface_Pointer) Game::getInstance().createTextureFromSurface(SDL_Surface_Pointer)
#define CHANGE_SCENE(Scene_Pointer) Game::getInstance().changeScene(Scene_Pointer)
#define TOGGLE_FULLSCREEN Game::getInstance().toggleFullscreen()
#define ENTER_FULLSCREEN Game::getInstance().enterFullscreen()
#define QUIT_FULLSCREEN Game::getInstance().quitFullscreen()