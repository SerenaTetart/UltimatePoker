#include "TextureManager.h"

TextureManager* TextureManager::s_Instance = nullptr;

bool TextureManager::Load(std::string id, std::string filename) {
	SDL_Surface* surface = IMG_Load(filename.c_str());
	if (surface == nullptr) {
		SDL_Log("Echec du chargement de la texture: %s\n%s", filename.c_str(), SDL_GetError());
		return false;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Engine::GetInstance()->GetRenderer(), surface);
	if (texture == nullptr) {
		SDL_Log("Echec de la création de la texture depuis la surface: %s", SDL_GetError());
		return false;
	}
	SDL_FreeSurface(surface); surface = NULL;
	m_TextureMap[id] = texture;
	return true;
}

bool TextureManager::LoadTTF(std::string id, std::string text, SDL_Color color, int size, int police) {
	TTF_Font* font = nullptr;
	if (police == 0) {
		if (Game::GetInstance()->GetGameStyle() == "basic") font = TTF_OpenFont("assets/ttf/Regardant.ttf", size);
		else if (Game::GetInstance()->GetGameStyle() == "cosmic") font = TTF_OpenFont("assets/ttf/game_font_7.ttf", size);
	}
	else if (police == 1) font = TTF_OpenFont("assets/ttf/Regardant.ttf", size);
	if (font == nullptr) {
		SDL_Log("Echec du chargement de la police TTF: %s", TTF_GetError());
		return false;
	}
	SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, text.c_str(), color);
	if (surfaceMessage == nullptr) {
		SDL_Log("Echec du chargement du TTF: %s\n%s", text.c_str(), TTF_GetError());
		return false;
	}
	SDL_Texture* Message = SDL_CreateTextureFromSurface(Engine::GetInstance()->GetRenderer(), surfaceMessage);
	if (Message == nullptr) {
		SDL_Log("Echec de la création de la texture du TTF depuis la surface: %s", SDL_GetError());
		return false;
	}
	TTF_CloseFont(font);
	SDL_FreeSurface(surfaceMessage); surfaceMessage = nullptr;
	m_TextureMap[id] = Message;
	return true;
}

bool TextureManager::LoadTTF(SDL_Texture** textureMessage, std::string text, SDL_Color color, int size, int police) {
	TTF_Font* font = nullptr;
	if (police == 0) {
		if (Game::GetInstance()->GetGameStyle() == "basic") font = TTF_OpenFont("assets/ttf/Regardant.ttf", size);
		else if (Game::GetInstance()->GetGameStyle() == "cosmic") font = TTF_OpenFont("assets/ttf/game_font_7.ttf", size);
	}
	else if (police == 1) font = TTF_OpenFont("assets/ttf/Regardant.ttf", size);
	if (font == nullptr) {
		SDL_Log("Echec du chargement de la police TTF: %s", TTF_GetError());
		return false;
	}
	TTF_SetFontStyle(font, TTF_STYLE_BOLD);
	SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, text.c_str(), color);
	if (surfaceMessage == nullptr) {
		SDL_Log("Echec du chargement du TTF: %s\n%s", text.c_str(), TTF_GetError());
		return false;
	}
	*textureMessage = SDL_CreateTextureFromSurface(Engine::GetInstance()->GetRenderer(), surfaceMessage);
	if (*textureMessage == nullptr) {
		SDL_Log("Echec de la création de la texture du TTF depuis la surface: %s", SDL_GetError());
		return false;
	}
	TTF_CloseFont(font);
	SDL_FreeSurface(surfaceMessage); surfaceMessage = nullptr;
	return true;
}

bool TextureManager::LoadTTF_Wrapped(SDL_Texture** textureMessage, std::string text, SDL_Color color, int size, int police, int width) {
	TTF_Font* font = nullptr;
	if (police == 0) {
		if (Game::GetInstance()->GetGameStyle() == "basic") font = TTF_OpenFont("assets/ttf/Regardant.ttf", size);
		else if (Game::GetInstance()->GetGameStyle() == "cosmic") font = TTF_OpenFont("assets/ttf/game_font_7.ttf", size);
	}
	else if (police == 1) font = TTF_OpenFont("assets/ttf/Regardant.ttf", size);
	if (font == nullptr) {
		SDL_Log("Echec du chargement de la police TTF: %s", TTF_GetError());
		return false;
	}
	TTF_SetFontStyle(font, TTF_STYLE_BOLD);
	SDL_Surface* surfaceMessage = TTF_RenderText_Blended_Wrapped(font, text.c_str(), color, width);
	if (surfaceMessage == nullptr) {
		SDL_Log("Echec du chargement du TTF: %s\n%s", text.c_str(), TTF_GetError());
		return false;
	}
	*textureMessage = SDL_CreateTextureFromSurface(Engine::GetInstance()->GetRenderer(), surfaceMessage);
	if (*textureMessage == nullptr) {
		SDL_Log("Echec de la création de la texture du TTF depuis la surface: %s", SDL_GetError());
		return false;
	}
	TTF_CloseFont(font);
	SDL_FreeSurface(surfaceMessage); surfaceMessage = nullptr;
	return true;
}

void TextureManager::DrawTTF(std::string id, int x, int y, double angle, SDL_RendererFlip flip) {
	int width, height;
	SDL_QueryTexture(m_TextureMap[id], NULL, NULL, &width, &height);
	SDL_Rect dstRect = { x, y, width, height };
	SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[id], NULL, &dstRect, angle, nullptr, flip);
}

void TextureManager::Draw(std::string id, int x, int y, int width, int height, double scale_x, double scale_y, double angle, SDL_RendererFlip flip) {
	SDL_Rect srcRect = { 0, 0, width, height };
	SDL_Rect dstRect = { x, y, width*scale_x, height*scale_y };
	SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[id], &srcRect, &dstRect, angle, nullptr, flip);
}

void TextureManager::Draw(SDL_Texture** texture, int x, int y, int width, int height, double scale_x, double scale_y, double angle, SDL_RendererFlip flip) {
	SDL_Rect srcRect = { 0, 0, width, height };
	SDL_Rect dstRect = { x, y, width * scale_x, height * scale_y };
	SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), *texture, &srcRect, &dstRect, angle, nullptr, flip);
}

void TextureManager::DrawFrame(std::string id, int x, int y, int width, int height, int row, int frame, double scale_x, double scale_y, double angle, SDL_RendererFlip flip) {
	SDL_Rect srcRect = { width*frame, height*row, width, height };
	SDL_Rect dstRect = { x, y, width*scale_x, height*scale_y };
	SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[id], &srcRect, &dstRect, angle, nullptr, flip);
}

void TextureManager::Drop(std::string id) {
	SDL_DestroyTexture(m_TextureMap[id]);
	m_TextureMap.erase(id);
}

void TextureManager::Clean() {
	std::map<std::string, SDL_Texture*>::iterator it;
	for (it = m_TextureMap.begin(); it != m_TextureMap.end(); it++) {
		SDL_DestroyTexture(it->second);
	}

	m_TextureMap.clear();

	SDL_Log("texture map cleaned !");
}