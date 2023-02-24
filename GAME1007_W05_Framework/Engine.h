// pragma once
#ifndef __ENGINE_H__
#define __ENGINE_H__
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <chrono>
#include <vector>
#include "Bullet.h"
#include "Enemy.h"
#include "Pause.h"
using namespace std;

#define WIDTH 1024
#define HEIGHT 768
#define FPS 120

class Engine
{
public: 
	Engine(); 
	int Run();
	vector<Bullet*> m_enemyBulletVec;


private:
	Uint32 m_start, m_end, m_delta, m_fps;
	const Uint8* m_pKeystates;
	SDL_Window* m_pWindow; 
	SDL_Renderer* m_pRenderer; 
	bool m_isRunning;
	std::vector<Bullet*> m_bulletVec;
	
	double speed, angle;
	SDL_Texture* m_pShipTexture, * m_pBGTexture, * m_pBulletTexture, * m_pEnemyTexture;
	SDL_Rect m_srcShip;
	SDL_FRect m_dstShip, m_dstBullet, m_bg1, m_bg2; 
	/*vector<Bullet*> m_bulletVec;*/
	
	vector<Enemy*> m_enemyVec;
	float m_scrollSpeed;
	SDL_FRect m_box;
	Enemy m_enemy;
	int m_enemyCtr, m_enemyMax;

	Mix_Chunk* m_boom, * m_laser, * m_emlaser;
	Mix_Music* m_gator;
	bool m_pew, m_epew;

	TTF_Font* m_pFont;
	SDL_Surface* m_pLabelSurf;
	SDL_Texture* m_pLabelText;
	SDL_Rect m_labelRect;

	// Pause stuff


	chrono::time_point<chrono::steady_clock> lastFrameTime, thisFrameTime; 
	chrono::duration<double> lastFrameDuration;
	double deltaTime;

	int Init(const char*, const int, const int, const int, const int, const int);
	void HandleEvents();
	void Wake();
	bool KeyDown(SDL_Scancode);
	void Update();
	void Sleep();
	void Render();
	void Clean();
};

#endif
