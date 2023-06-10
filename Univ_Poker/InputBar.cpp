#include "InputBar.h"
#include "TextureManager.h"

InputBar::InputBar(Properties props) : GameObject(props) {
	m_Selected = false; m_Dragging = false; m_Load = 100;
}

InputBar::~InputBar() {

}

void InputBar::Events(SDL_Event* ev) {
	if (!m_Dragging) {
		if (ev->type == SDL_MOUSEMOTION && ev->button.x >= (m_X - m_Width* m_Scale_x * 0.5) && ev->button.y >= (m_Y - m_Height* m_Scale_y * 0.5)
			&& ev->button.x <= (m_X - m_Width* m_Scale_x * 0.5) + m_Width * m_Scale_x && ev->button.y <= (m_Y - m_Height* m_Scale_y * 0.5) + m_Height * m_Scale_y) {
			m_Selected = true;
		}
		else if (ev->type == SDL_MOUSEMOTION) m_Selected = false;
		else if (m_Selected && ev->type == SDL_MOUSEBUTTONDOWN && ev->button.button == SDL_BUTTON_LEFT) {
			m_Load = (int)(((ev->button.x - (m_X - m_Width* m_Scale_x * 0.5)) / (m_Width*m_Scale_x)) * 100);
			m_Dragging = true;
		}
	}
	else if (ev->type == SDL_MOUSEBUTTONUP && ev->button.button == SDL_BUTTON_LEFT) m_Dragging = false;
	else if (ev->type == SDL_MOUSEMOTION) m_Load = (int)(((ev->button.x - (m_X - (m_Width*m_Scale_x*0.5))) / (m_Width*m_Scale_x)) * 100);
	if (m_Load > 100) m_Load = 100; else if (m_Load < 0) m_Load = 0;
}

void InputBar::Update(float dt) {
	
}

void InputBar::Draw() {
	SDL_Rect dstRect = { m_X - m_Width* m_Scale_x * 0.5, m_Y - m_Height*m_Scale_y*0.5,  (int)((float)m_Width*(float)m_Scale_x*((float)m_Load / 100.0f)), m_Height*m_Scale_y };
	SDL_RenderSetClipRect(Engine::GetInstance()->GetRenderer(), &dstRect);
	TextureManager::GetInstance()->Draw(m_TextureID, m_X - m_Width*m_Scale_x*0.5, m_Y - m_Height*m_Scale_y*0.5, m_Width, m_Height, m_Scale_x, m_Scale_y, m_Angle, m_Flip); //Icône
	SDL_RenderSetClipRect(Engine::GetInstance()->GetRenderer(), NULL);
}

void InputBar::Clean() {
	TextureManager::GetInstance()->Clean();
}