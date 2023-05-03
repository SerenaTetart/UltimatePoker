#include <iostream>

#include "Engine.h"
#include "Game.h"

int main(int argc, char** argv) {
	Engine::GetInstance()->Init();

	Uint64 last_tick = SDL_GetTicks64();
	Uint64 now = SDL_GetTicks64();

	while (Engine::GetInstance()->IsRunning()) {
		now = SDL_GetTicks64(); 
		if (now - last_tick > 1000 / Engine::GetInstance()->GetFramerate()) {
			Engine::GetInstance()->Events();
			Engine::GetInstance()->Update();
			Engine::GetInstance()->Render();
			last_tick = SDL_GetTicks64();
		}
		else SDL_Delay((1000 / Engine::GetInstance()->GetFramerate()) - (now - last_tick));
	}

	Engine::GetInstance()->Clean();
	return 0;
}
