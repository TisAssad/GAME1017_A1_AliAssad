#include "Enemy.h"
#include "Bullet.h"
#include "Engine.h"
#include <vector>
#include <iostream>

Enemy::Enemy(const SDL_FPoint spawn)
{
	m_src = { 0, 0, 159, 151 };
	m_dst = { spawn.x - 20.0f, spawn.y - 28.5f, 559.0f, 651.0f };
	m_bulletCtr = 0;
	m_bulletMax = 60 + rand() % 121;

	
}

void Enemy::Update()
{
	if (m_bulletCtr++ % m_bulletMax == 0)
	{
		m_bulletVec.push_back(new Bullet({ m_dst.x + m_dst.w / 2, m_dst.y + m_dst.h / 2 }, -400, true));
		cout << "The enemy is commencing fire!" << endl;

		
	}
}

state Enemy::GetState()
{
	return state();
}

SDL_Rect* Enemy::GetSrc()
{
	return &m_src;
}

SDL_FRect* Enemy::GetDst()
{
	return &m_dst;
}
