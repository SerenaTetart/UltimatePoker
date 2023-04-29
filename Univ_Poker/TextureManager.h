#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <string>
#include <map>

#include "Engine.h"

#include "Game.h"

class TextureManager {
public:
	static TextureManager* GetInstance() { return s_Instance = (s_Instance != nullptr) ? s_Instance : new TextureManager(); };

	bool Load(std::string id, std::string filename);
	bool LoadTTF(std::string id, std::string text, SDL_Color color, int size, int police=0);
	bool LoadTTF_2(SDL_Texture** textureMessage, std::string text, SDL_Color color, int size, int police=0); //N'enregistre pas dans la map des textures.
	void Drop(std::string id);
	void Clean();

	void Draw(std::string id, int x, int y, int width, int height, double scale_x = 1.0, double scale_y = 1.0, double angle = 0, SDL_RendererFlip = SDL_FLIP_NONE);
	void Draw_2(SDL_Texture** texture, int x, int y, int width, int height, double scale_x = 1.0, double scale_y = 1.0, double angle = 0, SDL_RendererFlip = SDL_FLIP_NONE);
	void DrawFrame(std::string id, int x, int y, int width, int height, int row, int frame, double scale_x=1.0, double scale_y=1.0, double angle=0, SDL_RendererFlip=SDL_FLIP_NONE);
	void DrawTTF(std::string id, int x, int y, double angle=0, SDL_RendererFlip=SDL_FLIP_NONE);
	/* Draw from sprite sheet :
	* row= row of the sprite sheet
	* frame= column of the sprite sheet*/

private:
	TextureManager(){};
	std::map<std::string, SDL_Texture*> m_TextureMap;
	static TextureManager* s_Instance;
};

#endif