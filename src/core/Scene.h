#pragma once

#include <SDL3/SDL.h>

// This is an interface for all scenes in the game.
class Scene
{
public:
	Scene() = default;
	virtual ~Scene() = default;
	virtual void init() = 0;
	virtual void handleEvents(SDL_Event* event) = 0;
	virtual void update(float deltaTime) = 0;
	virtual void render() = 0;
	virtual void clean() = 0;
};