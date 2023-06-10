#ifndef ENGINE_H
#define ENGINE_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960

class Engine {

	public:
		static Engine* GetInstance() {
			return s_Instance = (s_Instance != nullptr)? s_Instance : new Engine();
		}

		bool Init();
		bool Clean();
		void Quit();

		void Update();
		void Render();
		void Events();

		inline bool IsRunning() { return m_IsRunning; }
		inline SDL_Renderer* GetRenderer() { return m_Renderer; }
		inline int GetFramerate() { return m_Framerate; }
		inline void SetFramerate(int framerate) { m_Framerate = framerate; }
		inline int GetSoundVolume() { return m_SoundVolume; }
		inline void SetSoundVolume(int volume) { m_SoundVolume = volume; }

	private:
		Engine() { m_SoundVolume = 100; m_Framerate = 60; last_update = SDL_GetTicks64(); m_IsRunning = false; m_Window = nullptr; m_Renderer = nullptr; };
		bool m_IsRunning; int m_Framerate, m_SoundVolume; Uint64 last_update;
		SDL_Window* m_Window;
		SDL_Renderer* m_Renderer;

		static Engine* s_Instance;
};

#endif