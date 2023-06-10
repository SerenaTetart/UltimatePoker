#include "Button.h"
#include "TextureManager.h"

Button::Button(Properties props, void(*functionPtr)(), int size, int police, std::string text, SDL_Color color, bool keepColor, bool active) : GameObject(props) {
	m_FunctionPtr = functionPtr; m_Active = active;
	m_Size = size; m_Police = police; m_Text = text; m_Color = color;
	m_IsSelected = false; m_KeepColor = keepColor;
	if (keepColor) color = { 255, 0, 0 };
	if (text != "") m_Frame = new TextFrame(props, size, police, text, color);
}

Button::~Button() {
	delete(m_Frame);
}

void Button::Refresh() {
	if (m_Text != "" && !m_KeepColor) m_Frame->SetText(m_Size, m_Police, m_Text, m_Color);
	else if (m_Text != "" && m_KeepColor) m_Frame->SetText(m_Size, m_Police, m_Text, { 255, 0, 0 });
}

void Button::Events(SDL_Event *ev) {
	if ((ev->type == SDL_MOUSEMOTION || ev->type == SDL_MOUSEBUTTONDOWN) && m_Active) {
		if (ev->button.x >= m_X && ev->button.y >= m_Y && ev->button.x <= m_X + m_Width * m_Scale_x && ev->button.y <= m_Y + m_Height * m_Scale_y) {
			//La souris se situe au dessus du bouton
			if (!m_IsSelected) {
				m_IsSelected = true;
				if (m_Text != "") m_Frame->SetText(m_Size, m_Police, m_Text, { 255, 0, 0 });
			}
			if (ev->type == SDL_MOUSEBUTTONDOWN && ev->button.button == SDL_BUTTON_LEFT) {
				//Bouton pressé par un clic gauche
				if (m_Text != "" && !m_KeepColor) m_Frame->SetText(m_Size, m_Police, m_Text, m_Color);
				m_IsSelected = false;
				m_FunctionPtr();
			}
		}
		else if (m_IsSelected) {
			if (m_Text != "" && !m_KeepColor) m_Frame->SetText(m_Size, m_Police, m_Text, m_Color);
			m_IsSelected = false;
		}
	}
}

void Button::Update(float dt) {

}

void Button::Draw() {
	if (m_Active) {
		if (m_Text != "") m_Frame->Draw();
		else TextureManager::GetInstance()->Draw(m_TextureID, m_X, m_Y, m_Width, m_Height, m_Scale_x, m_Scale_y, m_Angle, m_Flip);
	}
}

void Button::Clean() {
	TextureManager::GetInstance()->Clean();
}