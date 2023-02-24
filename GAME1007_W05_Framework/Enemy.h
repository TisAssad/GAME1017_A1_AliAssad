#pragma once
#ifndef __ENEMY_H__
#define __ENEMY_H__
#include "Bullet.h"
#include <vector>
#include <SDL_rect.h>

enum state { IDLE, MOVE };

class Enemy
{
public:
	Enemy(const SDL_FPoint spawn);
	void Update();
	state GetState();
	SDL_Rect* GetSrc();
	SDL_FRect* GetDst();
private:
	 
	SDL_Rect m_src;
	SDL_FRect m_dst;
	int m_bulletCtr, m_bulletMax;
	std::vector<Bullet*> m_bulletVec;
};

#endif