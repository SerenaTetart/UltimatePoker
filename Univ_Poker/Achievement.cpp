#include "Achievement.h"
#include "Game.h"
#include "TextureManager.h"

Achievement::Achievement(std::string iconID, std::string name, std::string description, bool validated) {
	m_IconID = iconID; m_Name = name; m_Description = description; m_Validated = validated;
	Refresh();
}

Achievement::~Achievement() {
	SDL_DestroyTexture(m_TextureAchievement);
	m_TextureAchievement = nullptr;
}

void Achievement::Refresh() {
	if (m_TextureAchievement != nullptr) {
		SDL_DestroyTexture(m_TextureAchievement);
		m_TextureAchievement = nullptr;
	}
	int widthTarget = 236, heightTarget = 93; float scaleWidth = 4.0, scaleHeight = 1.5;
	SDL_Texture* targetTexture = SDL_CreateTexture(Engine::GetInstance()->GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET
		, widthTarget * scaleWidth, heightTarget * scaleHeight);
	if (targetTexture != nullptr) {
		SDL_Texture* TextureName;
		TextureManager::GetInstance()->LoadTTF(&TextureName, m_Name, { 255, 215, 0 }, 28, 1);
		int widthName, heightName;
		SDL_QueryTexture(TextureName, NULL, NULL, &widthName, &heightName);

		SDL_Texture* TextureDesc;
		TextureManager::GetInstance()->LoadTTF_Wrapped(&TextureDesc, m_Description, { 235, 195, 0 }, 22, 1, widthTarget * scaleWidth * 0.75);
		int widthDesc, heightDesc;
		SDL_QueryTexture(TextureDesc, NULL, NULL, &widthDesc, &heightDesc);

		SDL_SetRenderTarget(Engine::GetInstance()->GetRenderer(), targetTexture);
		SDL_SetRenderDrawColor(Engine::GetInstance()->GetRenderer(), 53, 101, 77, 255);
		SDL_RenderClear(Engine::GetInstance()->GetRenderer());

		std::string FrameID = "achievement_frame_off"; if (m_Validated == true) FrameID = "achievement_frame";
		TextureManager::GetInstance()->Draw(FrameID, 0, 0, widthTarget, heightTarget, scaleWidth, scaleHeight); //Cadre
		TextureManager::GetInstance()->Draw(FrameID, 50, heightTarget * scaleHeight * 0.5 - heightTarget * 0.5, widthTarget, heightTarget, 0.3941, 1.0); //Cadre icône
		if(m_Validated == false) TextureManager::GetInstance()->ModulateTexture(m_IconID, 50, 50, 50);
		else TextureManager::GetInstance()->ModulateTexture(m_IconID, 255, 255, 255);
		TextureManager::GetInstance()->Draw(m_IconID, 53, (heightTarget * scaleHeight * 0.5) - (92 * 0.95 * 0.5), 92, 92, 0.95, 0.95); //Icône

		SDL_Rect dstRect = { widthTarget * scaleWidth * 0.5 - widthName * 0.5, heightTarget * scaleHeight * 0.2 - heightName * 0.5, widthName, heightName };
		SDL_RenderCopy(Engine::GetInstance()->GetRenderer(), TextureName, NULL, &dstRect);
		dstRect = { int((widthTarget * scaleWidth * 0.5) - (widthDesc * 0.5)), int((heightTarget * scaleHeight * 0.6) - (heightDesc * 0.5)), widthDesc, heightDesc };
		SDL_RenderCopy(Engine::GetInstance()->GetRenderer(), TextureDesc, NULL, &dstRect);

		SDL_SetRenderTarget(Engine::GetInstance()->GetRenderer(), NULL);
		SDL_DestroyTexture(TextureName);
		SDL_DestroyTexture(TextureDesc);
		m_TextureAchievement = targetTexture;
	}
}