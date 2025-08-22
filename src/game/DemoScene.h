#pragma once

#include "core/Scene.h"
#include "game/Object.h"

#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <list>
#include <random>
#include <map>

class DemoScene : public Scene
{
public:
	void init() override;
	void handleEvents(SDL_Event* event) override;
	void update(float deltaTime) override;
	void render() override;
	void clean() override;
private:
	void onController(float deltaTime) noexcept;
	void firePlayer() noexcept;
	void fireEnemy(Enemy* enemy) noexcept;
	void updateBulletsPlayer(float deltaTime) noexcept;
	void renderBulletsPlayer() noexcept;
	void updateEnemy(float deltaTime) noexcept;
	void updateBulletsEnemy(float deltaTime) noexcept;
	void renderBulletsEnemy() noexcept;
	void renderEnemy() noexcept;
	void spawnEnemy() noexcept;
	void onEnemyDied(Enemy* enemy) noexcept;
	void onPlayerDied() noexcept;
	SDL_FPoint getEnemyTarget(Enemy* enemy) const noexcept;
	void spawnPoint(Enemy* enemy) noexcept;
	void updatePoint() noexcept;
	void renderPoint() noexcept;
	void onGetPoint() noexcept;
	void updateUI() noexcept;
	void renderUI() noexcept;
private:
	std::map<std::string, Mix_Chunk*> SFX = std::map<std::string, Mix_Chunk*>();
	std::mt19937 gen;
	std::uniform_real_distribution<float> dis;
	Mix_Music* bgm = nullptr;
	TTF_Font* font = nullptr;
	bool m_holdingShift = false;
	bool m_isPlayerDead = false;
	Player player = Player();
	Drawable background = Drawable();
	Drawable stage = Drawable();
	Ammo bulletTemplatePlayer = Ammo();
	std::list<Ammo*> bulletsPlayer = std::list<Ammo*>();
	Ammo bulletTemplateEnemy = Ammo();
	std::list<Ammo*> bulletsEnemy = std::list<Ammo*>();
	Enemy devilTemplate = Enemy();
	std::list<Enemy*> devils = std::list<Enemy*>();
	Item pointTemplate = Item();
	std::list<Item*> points = std::list<Item*>();
};