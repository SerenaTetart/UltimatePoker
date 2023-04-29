#include "Card.h"
#include "TextureManager.h"

Card::Card(Properties props, int Value, int Type, bool masked, bool visible): GameObject(props) {
	m_Value = Value; m_Type = Type;
	m_Visible = visible; m_Masked = masked;
	ChangeTexture();
}

void Card::ChangeTexture() {
	if (m_Visible && !m_Masked) m_TextureID = std::to_string(m_Value) + "_" + std::to_string(m_Type);
	else if (m_Visible && m_Masked) m_TextureID = "0_0";
	else m_TextureID = "";
}

void Card::SetVisible(bool visible) {
	m_Visible = visible;
	ChangeTexture();
}

void Card::SetMasked(bool masked) {
	m_Masked = masked;
	ChangeTexture();
}

void Card::SetValue(int value, int type) {
	m_Value = value; m_Type = type;
	ChangeTexture();
}

void Card::Events(SDL_Event* ev) {

}

void Card::Update(float dt) {

}

void Card::Draw() {
	TextureManager::GetInstance()->Draw(m_TextureID, m_X, m_Y, m_Width, m_Height, m_Scale_x, m_Scale_y, m_Angle, m_Flip);
}

void Card::Clean() {
	TextureManager::GetInstance()->Clean();
}