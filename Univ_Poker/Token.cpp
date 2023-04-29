#include "Token.h"
#include "TextureManager.h"

Token::Token(Properties props, int value) : GameObject(props) {
	m_Value = value;
	m_TextureID = "token_"+std::to_string(value);
}

void Token::SetValue(int value) {
	m_Value = value;
	m_TextureID = "token_" + std::to_string(value);
}

void Token::Events(SDL_Event* ev) {

}

void Token::Update(float dt) {

}

void Token::Draw() {
	TextureManager::GetInstance()->Draw(m_TextureID, m_X, m_Y, m_Width, m_Height, m_Scale_x, m_Scale_y, m_Angle, m_Flip);
}

void Token::Clean() {
	TextureManager::GetInstance()->Clean();
}