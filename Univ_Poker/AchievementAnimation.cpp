#include "AchievementAnimation.h"
#include "TextureManager.h"

AchievementAnimation::AchievementAnimation(Properties props) : GameObject(props) {
	m_Timer = 0; m_CurrentAchievement = nullptr;
}

AchievementAnimation::~AchievementAnimation() {
	
}

void AchievementAnimation::Events(SDL_Event* ev) {

}

void AchievementAnimation::Update(float dt) {
	if (m_AchievementVec.size() > 0) {
		Uint64 currentTime = SDL_GetTicks64();
		if (m_Timer == 0) { //Lancer l'animation et copie la texture
			if (m_CurrentAchievement != nullptr) {
				SDL_DestroyTexture(m_CurrentAchievement);
				m_CurrentAchievement = nullptr;
			}
			SDL_QueryTexture(*m_AchievementVec[0]->GetTexture(), NULL, NULL, &m_Width, &m_Height);
			m_CurrentAchievement = SDL_CreateTexture(Engine::GetInstance()->GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_Width, m_Height);
			if (m_CurrentAchievement != NULL) {
				SDL_SetRenderTarget(Engine::GetInstance()->GetRenderer(), m_CurrentAchievement);
				SDL_SetRenderDrawColor(Engine::GetInstance()->GetRenderer(), 53, 101, 77, 255);
				SDL_RenderClear(Engine::GetInstance()->GetRenderer());

				SDL_Rect dstRect = { 0, 0, m_Width, m_Height };
				SDL_RenderCopy(Engine::GetInstance()->GetRenderer(), *m_AchievementVec[0]->GetTexture(), NULL, &dstRect);

				SDL_SetRenderTarget(Engine::GetInstance()->GetRenderer(), NULL);
				SDL_SetTextureBlendMode(m_CurrentAchievement, SDL_BLENDMODE_BLEND);
				SDL_SetTextureAlphaMod(m_CurrentAchievement, 0);
			}
			m_Timer = currentTime;
		}
		else if ((currentTime - m_Timer) > 5000) { //Après 5 secondes supprimer
			m_Timer = 0;
			m_AchievementVec.erase(m_AchievementVec.begin());
		}
		else if ((currentTime - m_Timer) > 0 && (currentTime - m_Timer) < 1000) { //Fade in
			Uint8 alpha = 0; SDL_GetTextureAlphaMod(m_CurrentAchievement, &alpha);
			if ((int)alpha + 5 > 255) alpha = (Uint8)255; else alpha += 5;
			SDL_SetTextureAlphaMod(m_CurrentAchievement, alpha);
		}
		else if ((currentTime - m_Timer) > 4000) { //Fade out
			Uint8 alpha = 0; SDL_GetTextureAlphaMod(m_CurrentAchievement, &alpha);
			if ((int)alpha - 5 < 0) alpha = (Uint8)0; else alpha -= 5;
			SDL_SetTextureAlphaMod(m_CurrentAchievement, alpha);
		}
	}
}

void AchievementAnimation::Draw() {
	if (m_AchievementVec.size() > 0) {
		if (m_Timer > 0 && (SDL_GetTicks64() - m_Timer) > 0) { //Jouer l'animation
			TextureManager::GetInstance()->Draw(&m_CurrentAchievement, m_X - (m_Width*m_Scale_x * 0.5), m_Y - (m_Height*m_Scale_y*0.5), m_Width, m_Height, m_Scale_x, m_Scale_y, m_Angle, m_Flip);
		}
	}
}

void AchievementAnimation::Clean() {
	TextureManager::GetInstance()->Clean();
}