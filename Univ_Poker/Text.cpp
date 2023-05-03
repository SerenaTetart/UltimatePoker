#include "Text.h"
#include "Game.h"
#include "TextureManager.h"

Text::Text(Properties props, int size, int police, std::string text, SDL_Color colorText, std::string text2, SDL_Color colorText2, bool floating) : GameObject(props) {
	SetText(size, police, text, colorText, text2, colorText2);
	m_Timer = SDL_GetTicks64();
	m_Floating = floating;
}

Text::~Text() {
	SDL_DestroyTexture(m_TextureText);
}

void Text::Refresh() {
	if (m_TextureText != nullptr) {
		SDL_DestroyTexture(m_TextureText);
		m_TextureText = nullptr;
	}
	if (m_Text != "") TextureManager::GetInstance()->LoadTTF_2(&m_TextureText, m_Text, m_ColorText, m_Size, m_Police);
	SDL_QueryTexture(m_TextureText, NULL, NULL, &m_WidthText, &m_HeightText);
	if (m_Text2 != "") {
		SDL_Texture* TextureText2;
		TextureManager::GetInstance()->LoadTTF_2(&TextureText2, m_Text2, m_ColorText2, m_Size);
		int width, height;
		SDL_QueryTexture(TextureText2, NULL, NULL, &width, &height);
		m_WidthText += width;
		SDL_Texture* targetText = SDL_CreateTexture(Engine::GetInstance()->GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_WidthText, m_HeightText);
		if (targetText != nullptr) {
			SDL_SetRenderTarget(Engine::GetInstance()->GetRenderer(), targetText);
			SDL_SetRenderDrawColor(Engine::GetInstance()->GetRenderer(), 0, 0, 0, 255);
			SDL_RenderClear(Engine::GetInstance()->GetRenderer());
			SDL_Rect dstRect = { 0, 0, m_WidthText - width, m_HeightText };
			SDL_RenderCopy(Engine::GetInstance()->GetRenderer(), m_TextureText, NULL, &dstRect);
			dstRect = { m_WidthText - width, m_HeightText - height, width, height };
			SDL_RenderCopy(Engine::GetInstance()->GetRenderer(), TextureText2, NULL, &dstRect);
			SDL_SetRenderTarget(Engine::GetInstance()->GetRenderer(), NULL);
			SDL_DestroyTexture(m_TextureText);
			SDL_DestroyTexture(TextureText2);
			m_TextureText = targetText;
		}
	}
}

void Text::SetText(int size, int police, std::string text, SDL_Color colorText, std::string text2, SDL_Color colorText2) {
	m_Text = text; m_Text2 = text2;
	m_ColorText = colorText; m_ColorText2 = colorText2;
	m_Size = size; m_Police = police;
	m_Refresh = true;
}



void Text::Events(SDL_Event* ev) {

}

void Text::Update(float dt) {
	if (m_Floating) {
		m_Y -= 50*(dt/1000);
	}
}

void Text::Draw() {
	if (m_Refresh) { Refresh(); m_Refresh = false; }
	TextureManager::GetInstance()->Draw_2(&m_TextureText, m_X, m_Y, m_WidthText, m_HeightText, m_Scale_x, m_Scale_y, m_Angle, m_Flip);
}

void Text::Clean() {
	TextureManager::GetInstance()->Clean();
}