#pragma once

#include <SDL3/SDL.h>

struct Drawable
{
	SDL_Texture* texture = nullptr;
	SDL_FRect dstRect = { 0, 0, 64, 64 };
};

struct Player
{
	SDL_Texture* texture = nullptr;
	SDL_FRect dstRect = { 0, 0, 64, 64 };
	SDL_Rect detectRect = { 0, 0, 1, 1 };
	Uint64 cooldown = 100;
	Uint64 lastFireTime = 0;
	int score = 0;
};

struct Ammo
{
	SDL_Texture* texture = nullptr;
	SDL_FRect dstRect = { 0, 0, 64, 64 };
	SDL_FPoint target = { 0, 0 };
	int power = 1;
};

struct Enemy
{
	SDL_Texture* texture = nullptr;
	SDL_FRect dstRect = { 0, 0, 64, 64 };
	Uint64 cooldown = 100;
	Uint64 lastFireTime = 0;
	int HP = 3;
};

struct Animation
{
	SDL_Texture* texture = nullptr;
	SDL_FRect dstRect = { 0, 0, 64, 64 };
	int totalFrame = 0;
	int currentFrame = 0;
	int startTime = 0;
	int FPS = 10;
};

enum class ItemType
{
	POINT,
	POWERUP,
	BOMB,
	FULLPOWER
};

struct Item
{
	SDL_Texture* texture = nullptr;
	SDL_FRect dstRect = { 0, 0, 40, 40 };
	ItemType type;
};