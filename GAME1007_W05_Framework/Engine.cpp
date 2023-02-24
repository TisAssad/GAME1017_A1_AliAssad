#include "Engine.h"
#include <iostream>
#include <string>
using namespace std;

Engine::Engine() :m_pWindow(nullptr), m_pRenderer(nullptr), m_isRunning(false), m_enemy({ 0, 0 })
{
	
	m_pShipTexture = nullptr;
	m_pBGTexture = nullptr;
	m_pEnemyTexture = nullptr;
	m_scrollSpeed = 0.0f;
	speed = 600;
	angle = 90.0;
	m_enemyCtr = 0;
	m_enemyMax = 120;
	m_laser = nullptr;
	m_emlaser = nullptr;
	m_boom = nullptr;
	m_gator = nullptr;
	m_pew = false;
	m_epew = false;
}

int Engine::Run()
{
	if (m_isRunning)
	{
		return 1; // 1 arbitrarily means that engine is already running.
	}
	if (Init("GAME1007_M3", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, NULL))
	{
		return 2; // 2 arbitrarily means that something went wrong in init.
	}
	while (m_isRunning) // Main game loop. Run while isRunning = true.
	{
		Wake();
		HandleEvents(); // Input
		Update();       // Processing
		Render();       // Output
		if (m_isRunning == true)
			Sleep();

	}
	Clean(); // Deinitialize SDL and free up memory.
	return 0;
}

int Engine::Init(const char* title, const int xPos, const int yPos,
	const int width, const int height, const int flags)
{
	cout << "Initializing framework..." << endl;
	srand((unsigned)time(nullptr));
	SDL_Init(SDL_INIT_EVERYTHING);
	m_pWindow = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
	if (m_pWindow == nullptr) 
	{
		cout << "Error during window creation!" << endl;
		return 1;
	}
	m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
	if (m_pRenderer == nullptr) // Or NULL is okay too
	{
		cout << "Error during renderer creation!" << endl;
		return 1;
	}
	if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0)
	{
		cout << SDL_GetError() << endl; // Prints last SDL error msg.
		return 1;
	}
	m_pShipTexture = IMG_LoadTexture(m_pRenderer, "../assets/img/ship.png");
	m_pBGTexture = IMG_LoadTexture(m_pRenderer, "../assets/img/background.jpg");
	m_pBulletTexture = IMG_LoadTexture(m_pRenderer, "../assets/img/ship.png");
	m_pEnemyTexture = IMG_LoadTexture(m_pRenderer, "../Assets/img/enemies.png");
	m_fps = (Uint32)round((1.0 / (double)FPS) * 1000); 
	m_pKeystates = SDL_GetKeyboardState(nullptr);
	lastFrameTime = chrono::high_resolution_clock::now();
	m_dstShip = { WIDTH / 2.0f - 154.0f / 2.0f, HEIGHT / 2.0f - 221.0f / 2.0f , 154.0f, 221.0f };
	m_dstBullet = { WIDTH / 2.0f - 154.0f / 2.0f, HEIGHT / 2.0f - 221.0f / 2.0f , 154.0f, 221.0f };
	m_enemy = Enemy({ 512.0f, 384.0f });
	m_bg1 = { 0.0f, 0.0f, 1920.0f, 1080.0f };
	m_bg2 = { 0.0f, -1920.0f, 1920.0f, 1080.0f };
	if (Mix_Init(MIX_INIT_MP3) != 0)
	{
		// Configure mixer.
		Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);
		Mix_AllocateChannels(16);
		// Load sounds.
		m_boom = Mix_LoadWAV("../Assets/aud/boom.wav");
		m_laser = Mix_LoadWAV("../Assets/aud/laser.wav");
		m_gator = Mix_LoadMUS("../Assets/aud/gator.mp3");
		m_emlaser = Mix_LoadWAV("../Assets/aud/emlaser.wav");
		if (m_laser == nullptr || m_boom == nullptr || m_gator == nullptr)
			cout << Mix_GetError() << endl;
	}
	else return false; // Mixer init failed.
	if (TTF_Init() == 0)
	{
		m_pFont = TTF_OpenFont("../Assets/img/LTYPE.ttf", 50);
		string temp = "BEGIN GAME";
		m_pLabelSurf = TTF_RenderText_Solid(m_pFont, temp.c_str(), { 255, 255, 255, 255 });
		m_pLabelText = SDL_CreateTextureFromSurface(m_pRenderer, m_pLabelSurf);
		m_labelRect = { 0, 0, m_pLabelSurf->w, m_pLabelSurf->h };
		SDL_FreeSurface(m_pLabelSurf);
	}
	else
	{
		cout << TTF_GetError() << endl;
		return false;
	}
	Mix_PlayMusic(m_gator, -1); 
	Mix_VolumeMusic(20);
	m_isRunning = true; // Start your engine.
	return 0;
}

void Engine::HandleEvents()
{
	cout << "Handling events..." << endl;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT: // Pressing 'X' icon in SDL window.
			m_isRunning = false; // Tell Run() we're done.
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.scancode == SDL_SCANCODE_RCTRL)
			{
				m_bulletVec.push_back(new Bullet({m_dstShip.x + m_dstShip.w / 2, m_dstShip.y + m_dstShip.h / 2}, 400) );

				m_pew = true;

				if (!Mix_Playing(13))
				{
					Mix_PlayChannel(13, m_laser, -1);
				}
			}
			break;
		case SDL_KEYUP:
			if (event.key.keysym.scancode == SDL_SCANCODE_RCTRL)
			{
				m_pew = false;
				Mix_HaltChannel(13);
			}
			break;
		}
	}
}

void Engine::Wake()
{
	m_start = SDL_GetTicks();

	thisFrameTime = chrono::high_resolution_clock::now(); 
	lastFrameDuration = thisFrameTime - lastFrameTime;
	deltaTime = lastFrameDuration.count(); 
	lastFrameTime = thisFrameTime;
}

bool Engine::KeyDown(SDL_Scancode c)
{
	if (m_pKeystates != nullptr)
	{
		if (m_pKeystates[c] == 1)
			return true;
	}
	return false;
}

void Engine::Update()
{
	cout << "Updating frame..." << endl;
	string tickLabel = "DT: " + to_string(deltaTime);
	SDL_SetWindowTitle(m_pWindow, tickLabel.c_str());
	m_bg1.y += m_scrollSpeed * deltaTime;
	m_bg2.y += m_scrollSpeed * deltaTime;
	if (m_bg1.x >= 1920.0f /* or m_bg1.h*/)
	{
		m_bg1.y = 0.0f;
		m_bg2.y = -1920.0f;
	}
	
	
	
	// Move ship.
	if (KeyDown(SDL_SCANCODE_W) && m_dstShip.y > -35)
	{
		m_dstShip.y -= speed * deltaTime;
	}
	else if (KeyDown(SDL_SCANCODE_S) && m_dstShip.y < 587)
	{
		m_dstShip.y += speed * deltaTime;
	}
	if (KeyDown(SDL_SCANCODE_A) && m_dstShip.x > 25)
	{
		m_dstShip.x -= speed * deltaTime;
	}
	else if (KeyDown(SDL_SCANCODE_D) && m_dstShip.x < 850)
	{
		m_dstShip.x += speed * deltaTime;
	}
	
	for (auto bullet : m_bulletVec)
	{
		bullet->Update(deltaTime);
	}

	if (m_enemyCtr++ % m_enemyMax == 0)
	{
		m_enemyVec.push_back(new Enemy({(float)(rand() % 984),(float)(rand() % 712) }));
		cout << "New enemy spawned!" << endl;
	}

	for (auto enemy : m_enemyVec)
	{
		enemy->Update();

		m_epew = true;

		if (!Mix_Playing(16))
		{
			Mix_PlayChannel(16, m_emlaser, -1);
		}
	}
}


void Engine::Sleep()
{
	m_end = SDL_GetTicks(); // GetTicks return time in ms since initialization
	m_delta = m_end - m_start; // 1055 - 1050 = 5ms
	if (m_delta < m_fps)
		SDL_Delay(m_fps - m_delta);
}

void Engine::Render()
{
	cout << "Rendering changes..." << endl;
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer);

	// Render the GameObjects.
	SDL_RenderCopyF(m_pRenderer, m_pBGTexture, NULL, &m_bg1); 
	SDL_RenderCopyF(m_pRenderer, m_pBGTexture, NULL, &m_bg2);
															  

	SDL_RenderCopyExF(m_pRenderer, m_pShipTexture, NULL, &m_dstShip, angle, NULL, SDL_FLIP_NONE);
	

	SDL_SetRenderDrawColor(m_pRenderer, 0, 255, 120, 255);
	for (auto bullet : m_bulletVec)
	{
		SDL_RenderFillRectF(m_pRenderer, bullet->GetRekt());

		
	}
	
	SDL_Point origin = { WIDTH / 2, HEIGHT / 2 };
	SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
	

	SDL_RenderCopy(m_pRenderer, m_pLabelText, NULL, &m_labelRect);


	for (Enemy* enemy : m_enemyVec)
	{
		SDL_RenderCopyExF(m_pRenderer, m_pEnemyTexture, enemy->GetSrc(), enemy->GetDst(),
			-90, NULL, SDL_FLIP_NONE);

		enemy->Update();

	}

	SDL_RenderPresent(m_pRenderer); // Flips the buffers.
}

void Engine::Clean()
{
	cout << "Cleaning up..." << endl;
	for (auto bullet : m_bulletVec)
	{
		delete bullet;
		bullet = nullptr;
	}
	m_bulletVec.clear();
	m_bulletVec.shrink_to_fit();
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyTexture(m_pShipTexture);
	SDL_DestroyTexture(m_pBGTexture);
	SDL_DestroyTexture(m_pBulletTexture);
	SDL_DestroyTexture(m_pEnemyTexture);
	SDL_DestroyTexture(m_pLabelText);
	Mix_FreeChunk(m_laser);
	Mix_FreeChunk(m_emlaser);
	Mix_FreeChunk(m_boom);
	Mix_FreeMusic(m_gator);
	Mix_CloseAudio();
	TTF_CloseFont(m_pFont);
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}




