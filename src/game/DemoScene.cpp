#include "game/DemoScene.h"
#include "core/Game.h"

#include <string>
#include <sstream>
#include <iomanip>

void DemoScene::init()
{
	font = TTF_OpenFont("res/DFPPOPCorn-W12.fds", 24);

	SFX["firePlayer"] = Mix_LoadWAV("res/se_plst00.fds");
	SFX["diePlayer"] = Mix_LoadWAV("res/se_pldead00.fds");
	SFX["dieEnemy"] = Mix_LoadWAV("res/se_enep00.fds");
	SFX["getItem"] = Mix_LoadWAV("res/se_item00.fds");
	SFX["powerup"] = Mix_LoadWAV("res/se_power0.fds");

	bgm = Mix_LoadMUS("res/th06_02.fds");
	if (bgm != nullptr)
	{
		Mix_PlayMusic(bgm, -1);
	}

	std::random_device rd;
	gen = std::mt19937(rd());
	dis = std::uniform_real_distribution<float>(0.0f, 1.0f);

	player.texture = LOAD_TEXTURE("res/reimu_pixel.fds");
	player.dstRect = { 450, 1000, 80, 80 };
	player.detectRect = {
		int(player.dstRect.x + player.dstRect.w / 2),
		int(player.dstRect.y + player.dstRect.h / 2),
		1, 1
	};
	player.cooldown = 100;

	background.texture = LOAD_TEXTURE("res/test_bg.fds");
	background.dstRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

	stage.texture = LOAD_TEXTURE("res/test_stage.fds");
	stage.dstRect = { 50, 50, 900, 1100 };

	bulletTemplatePlayer.texture = LOAD_TEXTURE("res/rune.fds");
	bulletTemplatePlayer.dstRect = { 0, 0, 64, 64 };

	devilTemplate.texture = LOAD_TEXTURE("res/devil.fds");
	devilTemplate.dstRect = { 0, 0, 80, 80 };
	devilTemplate.cooldown = 1000;

	bulletTemplateEnemy.texture = LOAD_TEXTURE("res/bullet.fds");
	bulletTemplateEnemy.dstRect = { 0, 0, 32, 32 };

	pointTemplate.texture = LOAD_TEXTURE("res/point.fds");
	pointTemplate.type = ItemType::POINT;
}

void DemoScene::handleEvents(SDL_Event* event)
{

}

void DemoScene::update(float deltaTime)
{
	if(!m_isPlayerDead)
	{
		player.score++;
	}
	updateUI();
	spawnEnemy();
	updateEnemy(deltaTime);
	updateBulletsEnemy(deltaTime);
	updatePoint();
	updateBulletsPlayer(deltaTime);
	onController(deltaTime);
}

void DemoScene::render()
{
	RENDER_TEXTURE(background.texture, &background.dstRect);
	RENDER_TEXTURE(stage.texture, &stage.dstRect);
	renderUI();
	renderBulletsPlayer();
	if(!m_isPlayerDead)
	{
		RENDER_TEXTURE(player.texture, &player.dstRect);
	}
	renderBulletsEnemy();
	renderEnemy();
	renderPoint();
}

void DemoScene::clean()
{
	for (auto& b : bulletsPlayer)
	{
		if (b != nullptr)
		{
			delete b;
		}
	}
	bulletsPlayer.clear();

	for (auto& b : bulletsEnemy)
	{
		if (b != nullptr)
		{
			delete b;
		}
	}
	bulletsEnemy.clear();

	for (auto& d : devils)
	{
		if (d != nullptr)
		{
			delete d;
		}
	}
	devils.clear();

	for (auto& p : points)
	{
		if (p != nullptr)
		{
			delete p;
		}
	}
	points.clear();

	for (auto& s : SFX)
	{
		if (s.second != nullptr)
		{
			Mix_FreeChunk(s.second);
		}
	}

	TTF_CloseFont(font);

	Mix_HaltMusic();
	Mix_FreeMusic(bgm);

	SDL_DestroyTexture(player.texture);
	SDL_DestroyTexture(background.texture);
	SDL_DestroyTexture(stage.texture);
	SDL_DestroyTexture(bulletTemplatePlayer.texture);
	SDL_DestroyTexture(devilTemplate.texture);
	SDL_DestroyTexture(bulletTemplateEnemy.texture);
	SDL_DestroyTexture(pointTemplate.texture);
}

void DemoScene::onController(float deltaTime) noexcept
{
	auto state = SDL_GetKeyboardState(nullptr);
	// Fullscreen
	if (state[SDL_SCANCODE_F11])
	{
		ENTER_FULLSCREEN;
	}
	else if (state[SDL_SCANCODE_ESCAPE])
	{
		QUIT_FULLSCREEN;
	}

	if (m_isPlayerDead)
	{
		if (state[SDL_SCANCODE_X])
		{
			CHANGE_SCENE(new DemoScene());
		}
		return;
	}

	// Movement
	int rate = 400;
	if (state[SDL_SCANCODE_LSHIFT])
	{
		rate /= 2;
	}
	float stepLength = rate * deltaTime;

	if (state[SDL_SCANCODE_RIGHT] && state[SDL_SCANCODE_UP])
	{
			player.dstRect.x += stepLength;
			player.dstRect.y -= stepLength;
	}
	else if (state[SDL_SCANCODE_RIGHT] && state[SDL_SCANCODE_DOWN])
	{
			player.dstRect.x += stepLength;
			player.dstRect.y += stepLength;
	}
	else if (state[SDL_SCANCODE_LEFT] && state[SDL_SCANCODE_DOWN])
	{
			player.dstRect.x -= stepLength;
			player.dstRect.y += stepLength;
	}
	else if (state[SDL_SCANCODE_LEFT] && state[SDL_SCANCODE_UP])
	{
			player.dstRect.x -= stepLength;
			player.dstRect.y -= stepLength;
	}
	else if (state[SDL_SCANCODE_RIGHT])
	{
		player.dstRect.x += stepLength;
	}
	else if (state[SDL_SCANCODE_LEFT])
	{
		player.dstRect.x -= stepLength;
	}
	else if (state[SDL_SCANCODE_UP])
	{
		player.dstRect.y -= stepLength;
	}
	else if (state[SDL_SCANCODE_DOWN])
	{
		player.dstRect.y += stepLength;
	}

	if (player.dstRect.x < stage.dstRect.x)
	{
		player.dstRect.x = stage.dstRect.x;
	}
	if (player.dstRect.y < stage.dstRect.y)
	{
		player.dstRect.y = stage.dstRect.y;
	}
	if (player.dstRect.x > stage.dstRect.x + stage.dstRect.w - player.dstRect.w)
	{
		player.dstRect.x = stage.dstRect.x + stage.dstRect.w - player.dstRect.w;
	}
	if (player.dstRect.y > stage.dstRect.y + stage.dstRect.h - player.dstRect.h)
	{
		player.dstRect.y = stage.dstRect.y + stage.dstRect.h - player.dstRect.h;
	}
	player.detectRect = {
	int(player.dstRect.x + player.dstRect.w / 2),
	int(player.dstRect.y + player.dstRect.h / 2),
	8, 8
	};

	// Fire
	if (state[SDL_SCANCODE_Z])
	{
		Uint64 currentTime = SDL_GetTicks();
		if(currentTime - player.lastFireTime > player.cooldown)
		{
			firePlayer();
			player.lastFireTime = currentTime;
		}
	}
}

void DemoScene::firePlayer() noexcept
{
	Mix_PlayChannel(0, SFX["firePlayer"], 0);
	Ammo* bullet = new Ammo(bulletTemplatePlayer);
	bullet->dstRect.x = player.dstRect.x + player.dstRect.w / 6;
	bullet->dstRect.y = player.dstRect.y + player.dstRect.h / 6;
	bulletsPlayer.push_back(bullet);
}

void DemoScene::fireEnemy(Enemy* enemy) noexcept
{
	Ammo* bullet = new Ammo(bulletTemplateEnemy);
	bullet->dstRect.x = enemy->dstRect.x + enemy->dstRect.w / 3;
	bullet->dstRect.y = enemy->dstRect.y + enemy->dstRect.h / 4;
	bullet->target = getEnemyTarget(enemy);
	bulletsEnemy.push_back(bullet);
}

void DemoScene::updateBulletsPlayer(float deltaTime) noexcept
{
	for (auto it = bulletsPlayer.begin(); it != bulletsPlayer.end();)
	{
		int rate = 1000;
		Ammo* b = *it;
		b->dstRect.y -= rate * deltaTime;
		if (b->dstRect.y < stage.dstRect.y)
		{
			delete b;
			it = bulletsPlayer.erase(it);
		}
		else
		{
			bool hasHit = false;
			SDL_Rect bullectRect = { int(b->dstRect.x), int(b->dstRect.y), b->dstRect.w, b->dstRect.h };
			for (auto e : devils)
			{
				SDL_Rect devilRect = { int(e->dstRect.x), int(e->dstRect.y), e->dstRect.w, e->dstRect.h };
				if (SDL_HasRectIntersection(&bullectRect, &devilRect))
				{
					e->HP -= b->power;
					delete b;
					it = bulletsPlayer.erase(it);
					hasHit = true;
					break;
				}
			}
			if (!hasHit)
			{
				++it;
			}
		}
	}
}

void DemoScene::renderBulletsPlayer() noexcept
{
	for (auto b : bulletsPlayer)
	{
		RENDER_TEXTURE(b->texture, &b->dstRect);
	}
}

void DemoScene::updateEnemy(float deltaTime) noexcept
{
	Uint64 currentTime = SDL_GetTicks();
	int rate = 250;
	for (auto it = devils.begin(); it != devils.end();)
	{
		auto d = *it;
		d->dstRect.y += rate * deltaTime;
		if (d->dstRect.y > stage.dstRect.h - 20)
		{
			delete d;
			it = devils.erase(it);
		}
		else
		{
			if (currentTime - d->lastFireTime > d->cooldown && !m_isPlayerDead)
			{
				fireEnemy(d);
				d->lastFireTime = currentTime;
			}

			if (d->HP <= 0)
			{
				onEnemyDied(d);
				it = devils.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}

void DemoScene::updateBulletsEnemy(float deltaTime) noexcept
{
	for (auto it = bulletsEnemy.begin(); it != bulletsEnemy.end();)
	{
		int rate = 300;
		Ammo* b = *it;
		b->dstRect.x += rate * b->target.x * deltaTime;
		b->dstRect.y += rate * b->target.y * deltaTime;
		if (b->dstRect.y < stage.dstRect.y ||
			b->dstRect.y > stage.dstRect.h ||
			b->dstRect.x < stage.dstRect.x ||
			b->dstRect.x > stage.dstRect.w)
		{
			delete b;
			it = bulletsEnemy.erase(it);
		}
		else
		{
			SDL_Rect bulletRect = { int(b->dstRect.x), int(b->dstRect.y), b->dstRect.w, b->dstRect.h };
			if (SDL_HasRectIntersection(&player.detectRect, &bulletRect) && !m_isPlayerDead)
			{
				onPlayerDied();
				delete b;
				it = bulletsEnemy.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}

void DemoScene::renderBulletsEnemy() noexcept
{
	for (auto b : bulletsEnemy)
	{
		RENDER_TEXTURE(b->texture, &b->dstRect);
	}
}

void DemoScene::renderEnemy() noexcept
{
	for (auto d : devils)
	{
		RENDER_TEXTURE(d->texture, &d->dstRect);
	}
}

void DemoScene::spawnEnemy() noexcept
{
	if (dis(gen) > 1 / 60.0f)
	{
		return;
	}
	Enemy* d = new Enemy(devilTemplate);
	d->dstRect.x = dis(gen) * (stage.dstRect.w - 48);
	d->dstRect.y = stage.dstRect.y;
	devils.push_back(d);
}

void DemoScene::onEnemyDied(Enemy* enemy) noexcept
{
	Mix_PlayChannel(-1, SFX["dieEnemy"], 0);
	if (dis(gen) < 0.8f)
	{
		spawnPoint(enemy);
	}
	delete enemy;
}

void DemoScene::onPlayerDied() noexcept
{
	Mix_PlayChannel(-1, SFX["diePlayer"], 0);
	m_isPlayerDead = true;
	SDL_Log("You Died");
	SDL_Log("Score : %d", player.score * 10);
}

SDL_FPoint DemoScene::getEnemyTarget(Enemy* enemy) const noexcept
{
	float x = (player.dstRect.x + (player.dstRect.w / 2)) - (enemy->dstRect.x + (enemy->dstRect.w / 2));
	float y = (player.dstRect.y + (player.dstRect.h / 2)) - (enemy->dstRect.y + (enemy->dstRect.h / 2));
	float length = sqrt(x * x + y * y);
	x /= length;
	y /= length;
	return SDL_FPoint(x, y);
}

void DemoScene::spawnPoint(Enemy* enemy) noexcept
{
	Item* point = new Item(pointTemplate);
	point->dstRect.x = enemy->dstRect.x + enemy->dstRect.w / 3;
	point->dstRect.y = enemy->dstRect.y + enemy->dstRect.h / 4;
	points.push_back(point);
}

void DemoScene::updatePoint() noexcept
{
	for (auto it = points.begin(); it != points.end();)
	{
		Item* p = *it;
		p->dstRect.y += 6;
		SDL_Rect playerRect = { int(player.dstRect.x),int(player.dstRect.y), player.dstRect.w, player.dstRect.h };
		SDL_Rect pointRect = { int(p->dstRect.x), int(p->dstRect.y), p->dstRect.w, p->dstRect.h };
		if (p->dstRect.y >= stage.dstRect.h)
		{
			delete p;
			it = points.erase(it);
		}
		else if (SDL_HasRectIntersection(&playerRect, &pointRect) && !m_isPlayerDead)
		{
			onGetPoint();
			delete p;
			it = points.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void DemoScene::renderPoint() noexcept
{
	for (auto p : points)
	{
		RENDER_TEXTURE(p->texture, &p->dstRect);
	}
}

void DemoScene::onGetPoint() noexcept
{
	Mix_PlayChannel(1, SFX["getItem"], 0);
	SDL_Log("Point get");
	player.score += 100;
}

void DemoScene::updateUI() noexcept
{
}

void DemoScene::renderUI() noexcept
{
	std::stringstream ss;
	ss << "Score    " << std::setw(10) << std::setfill('0') << player.score * 10;
	SDL_Surface* surface = TTF_RenderText_Blended(font, ss.str().c_str(), 0, {255, 50, 50, 255});
	SDL_Texture* texture = CREATE_TEXTURE_FROM_SURFACE(surface);
	SDL_FRect rect = { 970, 100, 600, 50 };
	RENDER_TEXTURE(texture, &rect);
	SDL_DestroySurface(surface);
	SDL_DestroyTexture(texture);

	surface = TTF_RenderText_Blended(font, ss.str().c_str(), 0, { 255, 255, 255, 255 });
	texture = CREATE_TEXTURE_FROM_SURFACE(surface);
	rect = { 972, 102, 600, 50 };
	RENDER_TEXTURE(texture, &rect);

	SDL_DestroySurface(surface);
	SDL_DestroyTexture(texture);

}
