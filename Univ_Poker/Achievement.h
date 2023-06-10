#ifndef ACHIEVEMENT_H
#define ACHIEVEMENT_H

#include <vector>
#include "GameObject.h"
#include "Text.h"

class Achievement {
public:
	Achievement(std::string icon, std::string name, std::string description, bool validated=false);
	~Achievement();

	inline bool GetValidated() { return m_Validated; }
	inline void SetValidated(bool valid) { m_Validated = valid; Refresh(); }
	inline SDL_Texture** GetTexture() { return &m_TextureAchievement; }

private:
	void Refresh();
	std::string m_IconID, m_Name, m_Description;
	bool m_Validated;
	SDL_Texture* m_TextureAchievement;
};

#endif