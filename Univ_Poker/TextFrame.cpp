#include "TextFrame.h"
#include "TextureManager.h"

TextFrame::TextFrame(Properties props, int size, int police, std::string text, SDL_Color colorText, std::string text2, SDL_Color colorText2, bool active): GameObject(props) {
	SetText(size, police, text, colorText, text2, colorText2);
	m_Active = active;
}

TextFrame::~TextFrame() {
	SDL_DestroyTexture(m_TextureText);
}

void TextFrame::Refresh() {
	if (m_TextureText != nullptr) {
		SDL_DestroyTexture(m_TextureText);
		m_TextureText = nullptr;
	}
	if (m_Text != "") TextureManager::GetInstance()->LoadTTF(&m_TextureText, m_Text, m_ColorText, m_Size, m_Police);
	SDL_QueryTexture(m_TextureText, NULL, NULL, &m_WidthText, &m_HeightText);
	if (m_Text2 != "") {
		SDL_Texture* TextureText2;
		TextureManager::GetInstance()->LoadTTF(&TextureText2, m_Text2, m_ColorText2, m_Size);
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

void TextFrame::SetFrame(Properties props) {
	m_TextureID = props.TextureID;
	m_X = props.X;
	m_Y = props.Y;
	m_Width = props.Width;
	m_Height = props.Height;
	m_Angle = props.Angle;
	m_Scale_x = props.Scale_x;
	m_Scale_y = props.Scale_y;
	m_Flip = props.Flip;
}

void TextFrame::SetText(int size, int police, std::string text, SDL_Color colorText, std::string text2, SDL_Color colorText2) {
	m_Text = text; m_Text2 = text2;
	m_ColorText = colorText; m_ColorText2 = colorText2;
	m_Size = size; m_Police = police;
	m_Refresh = true;
}



void TextFrame::Events(SDL_Event* ev) {

}

void TextFrame::Update(float dt) {

}

void TextFrame::Draw() {
	if (m_Active) {
		if (m_Refresh) { Refresh(); m_Refresh = false; }
		TextureManager::GetInstance()->Draw(m_TextureID, m_X, m_Y, m_Width, m_Height, m_Scale_x, m_Scale_y, m_Angle, m_Flip); //Cadre
		TextureManager::GetInstance()->Draw(&m_TextureText
			, m_X + (m_Width * m_Scale_x * 0.5) - (m_WidthText * 0.5)
			, m_Y + (m_Height * m_Scale_y * 0.5) - (m_HeightText * 0.5)
			, m_WidthText, m_HeightText, 1.0, 1.0
			, m_Angle, m_Flip);
	}
}

void TextFrame::Clean() {
	TextureManager::GetInstance()->Clean();
}