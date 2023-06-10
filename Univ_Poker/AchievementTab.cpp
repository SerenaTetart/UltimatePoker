#include "AchievementTab.h"
#include "TextureManager.h"

AchievementTab::AchievementTab(Properties props) : GameObject(props) {
	m_Scroll_Y = 0; m_Selected = false; m_TextureTab = nullptr;
}

AchievementTab::~AchievementTab() {
	SDL_DestroyTexture(m_TextureTab);
	m_TextureTab = nullptr;
}

void AchievementTab::Refresh() {
	if (m_TextureTab != nullptr) {
		SDL_DestroyTexture(m_TextureTab);
		m_TextureTab = nullptr;
	}
	int widthTarget = 236, heightTarget = 93, gap = 20; float scaleWidth = 4.0, scaleHeight = 1.5;
	m_Width = widthTarget * scaleWidth;
	m_Height = ((heightTarget * scaleHeight) + gap) * m_AchievementVec.size();
	SDL_Texture* targetTexture = SDL_CreateTexture(Engine::GetInstance()->GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_Width, m_Height);
	if (targetTexture != NULL) {
		SDL_SetRenderTarget(Engine::GetInstance()->GetRenderer(), targetTexture);
		SDL_SetRenderDrawColor(Engine::GetInstance()->GetRenderer(), 53, 101, 77, 255);
		SDL_RenderClear(Engine::GetInstance()->GetRenderer());

		for (int i = 0; i < m_AchievementVec.size(); i++) {
			SDL_Rect dstRect = { 0, ((heightTarget * scaleHeight) + gap) * i, (widthTarget * scaleWidth), (heightTarget * scaleHeight) };
			SDL_RenderCopy(Engine::GetInstance()->GetRenderer(), *m_AchievementVec[i]->GetTexture(), NULL, &dstRect);
		}

		SDL_SetRenderTarget(Engine::GetInstance()->GetRenderer(), NULL);
		m_TextureTab = targetTexture;
	}
}

void AchievementTab::Events(SDL_Event* ev) {
	if (ev->type == SDL_MOUSEMOTION && ev->button.x >= m_X && ev->button.y >= m_Y && ev->button.x <= m_X + m_Width * m_Scale_x && ev->button.y <= m_Y + ((93 * 1.5) + 20) * 3) {
		m_Selected = true;
	}
	else if (ev->type == SDL_MOUSEMOTION) m_Selected = false;
	else if (ev->type == SDL_MOUSEWHEEL && m_Selected) {
		if (ev->wheel.y > 0) m_Scroll_Y = m_Scroll_Y + 35;
		else if (ev->wheel.y < 0) m_Scroll_Y = m_Scroll_Y - 35;
		if (m_Scroll_Y > 0) m_Scroll_Y = 0;
		if (-m_Scroll_Y > m_Height - (((93 * 1.5) + 20) * 3)) m_Scroll_Y = -(m_Height - (((93 * 1.5) + 20) * 3));
	}
}

void AchievementTab::Update(float dt) {

}

void AchievementTab::Draw() {
	SDL_Rect dstRect = { m_X - m_Width*0.5, m_Y,  m_Width, ((93 * 1.5) + 20) * 3};
	SDL_RenderSetClipRect(Engine::GetInstance()->GetRenderer(), &dstRect);
	TextureManager::GetInstance()->Draw(&m_TextureTab, m_X - m_Width*0.5, m_Y + m_Scroll_Y, m_Width, m_Height, m_Scale_x, m_Scale_y, m_Angle, m_Flip);
	SDL_RenderSetClipRect(Engine::GetInstance()->GetRenderer(), NULL);
}

void AchievementTab::Clean() {
	TextureManager::GetInstance()->Clean();
}