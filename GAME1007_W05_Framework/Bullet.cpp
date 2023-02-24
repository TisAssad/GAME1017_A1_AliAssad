#include "Bullet.h"
#include <chrono>
#include "SDL.h"
Bullet::Bullet(const SDL_FPoint spawn, const double speed, bool fromPlayer)
{
	m_isPlayerBullet = fromPlayer;
	if (m_isPlayerBullet)
		m_dst = { spawn.x - 4.0f, spawn.y - 4.0f, 8.0f, 8.0f };
	else 
		m_dst = { spawn.x - 2.0f, spawn.y - 2.0f, 4.0f, 4.0f };
	
	
	m_speed = speed;
	m_dx = 1.0; 
	m_dy = 0.0;
	
	m_srcBullet = {  };
}

void Bullet::Update(const double dt)
{
	m_dst.x += m_dx * m_speed / 15;
	m_dst.y += m_dy * m_speed / 15;
}

bool Bullet::IsPlayerBullet()
{
	return m_isPlayerBullet;
}
