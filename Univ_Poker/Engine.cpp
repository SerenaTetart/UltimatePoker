#include <iostream>

#include "Engine.h"
#include "TextureManager.h"

Engine* Engine::s_Instance = nullptr;

bool Engine::Init() {
	m_Framerate = 60;
	if (SDL_Init(SDL_INIT_VIDEO) != 0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != 0) {
		SDL_Log("Echec de l'initialisation de SDL: %s", SDL_GetError());
		return false;
	}

	if (TTF_Init() != 0) {
		SDL_Log("Echec de l'initialisation de SDL_TTF: %s", TTF_GetError());
		return false;
	}

	m_Window = SDL_CreateWindow("Ultimate Poker"
		, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED
		, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (m_Window == nullptr) {
		SDL_Log("Echec de la création de la fenêtre: %s", SDL_GetError());
		return false;
	}

	m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
	if (m_Renderer == nullptr) {
		SDL_Log("Echec de la création du rendu: %s", SDL_GetError());
		return false;
	}
	//=== Chargement du menu ===//
	TextureManager::GetInstance()->Load("txtframe_menu", "assets/txtframes/basic_1.png");
	TextureManager::GetInstance()->Load("inputbox_menu", "assets/txtframes/basic_input.png");
	TextureManager::GetInstance()->Load("inputbox_menu2", "assets/txtframes/basic_input2.png");
	last_update = SDL_GetTicks64();
	return (m_IsRunning = true);
}

bool Engine::Clean() {
	TextureManager::GetInstance()->Clean();
	Game::GetInstance()->Clean();
	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);
	IMG_Quit();
	SDL_Quit();
	return true;
}

void Engine::Quit() {
	Game::GetInstance()->Quit();
	m_IsRunning = false;
}

void Engine::Update() {
	float deltaTime = (SDL_GetTicks64() - last_update); //En secondes
	Game::GetInstance()->Update(deltaTime);
	last_update = SDL_GetTicks64();
}

void Engine::Render() {
	SDL_RenderClear(m_Renderer);
	Game::GetInstance()->Render();
	SDL_RenderPresent(m_Renderer);
}

void Engine::Events() {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
		case SDL_QUIT:
			Quit();
			break;
	}
	Game::GetInstance()->Events(&event);
}