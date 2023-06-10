#include "InputBox.h"
#include "TextureManager.h"

InputBox::InputBox(Properties props, int size, int police, std::string text, SDL_Color color, int limit, bool number_only) : GameObject(props) {
	m_Size = size; m_Police = police; m_Text = text; m_Color = color;
	m_IsSelected = false; m_NumberOnly = number_only;
	m_LimitChar = limit;
	m_Frame = new TextFrame(props, size, police, text, color);
}

InputBox::~InputBox() {
	delete(m_Frame);
}

void InputBox::Refresh() {
	delete(m_Frame);
	m_Frame = new TextFrame(Properties(m_TextureID, m_X, m_Y, m_Width, m_Height, m_Scale_x, m_Scale_y, m_Angle, m_Flip), m_Size, m_Police, m_Text, m_Color);
}

void InputBox::Events(SDL_Event* ev) {
	if (ev->type == SDL_MOUSEBUTTONDOWN && ev->button.button == SDL_BUTTON_LEFT) {
		if (ev->button.x >= m_X && ev->button.y >= m_Y && ev->button.x <= m_X + m_Width && ev->button.y <= m_Y + m_Height) {
			//La souris clic gauche au dessus de la zone
			if (!m_IsSelected) m_IsSelected = true;
		}
		else if (m_IsSelected) m_IsSelected = false;
	}
	// Si la zone est selectionnée et que l'utilisateur écrit: concatener le texte
	else if (m_IsSelected && ev->type == SDL_KEYDOWN) {
		bool tmp = false;
		if (ev->key.keysym.sym == SDLK_BACKSPACE && m_Text.length() > 0) { m_Text.pop_back(); tmp = true; }
		else if (m_NumberOnly && m_Text.length() < m_LimitChar && ev->key.keysym.sym >= SDLK_0 && ev->key.keysym.sym <= SDLK_9) { m_Text += std::to_string((int)(ev->key.keysym.sym - SDLK_0)); tmp = true; }
		else if (!m_NumberOnly && m_Text.length() < m_LimitChar) { m_Text += (char)(ev->key.keysym.sym); tmp = true; }
		if (tmp) {
			delete(m_Frame);
			m_Frame = new TextFrame(Properties(m_TextureID, m_X, m_Y, m_Width, m_Height, m_Scale_x, m_Scale_y, m_Angle, m_Flip), m_Size, m_Police, m_Text, m_Color);
		}
	}
}

void InputBox::SetText(std::string text) {
	m_Text = text;
	delete(m_Frame);
	m_Frame = new TextFrame(Properties(m_TextureID, m_X, m_Y, m_Width, m_Height, m_Scale_x, m_Scale_y, m_Angle, m_Flip), m_Size, m_Police, text, m_Color);
}

void InputBox::Update(float dt) {

}

void InputBox::Draw() {
	m_Frame->Draw();
}

void InputBox::Clean() {
	TextureManager::GetInstance()->Clean();
}