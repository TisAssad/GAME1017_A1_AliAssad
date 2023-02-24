#pragma once
#ifndef _BULLET_H_
#define _BULLET_H_
#include <SDL.h>

class Bullet
{
public:
	Bullet(const SDL_FPoint spawn, const double speed, bool fromPlayer = true);
	void Update(const double dt);;
	SDL_FRect* GetRekt() { return &m_dst; };
	bool IsPlayerBullet();
	bool m_deleteMe;
	
private:
	bool m_isPlayerBullet; 
	SDL_Rect m_srcBullet; 
	SDL_FRect m_dst; 
	float m_speed; 
	double m_dx, m_dy;
};

#endif