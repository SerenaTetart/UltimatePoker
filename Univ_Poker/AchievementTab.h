#ifndef ACHIEVEMENTTAB_H
#define ACHIEVEMENTTAB_H

#include <vector>
#include "Achievement.h"
#include "GameObject.h"

class AchievementTab : public GameObject {
public:
	AchievementTab(Properties props);
	~AchievementTab();

	virtual void Events(SDL_Event* ev);
	virtual void Update(float dt);
	virtual void Draw();
	virtual void Clean();

	void Refresh();
	inline void AddAchievement(Achievement* ach) { m_AchievementVec.push_back(ach); }

private:
	std::vector<Achievement*> m_AchievementVec;
	SDL_Texture* m_TextureTab;
	float m_Scroll_Y; bool m_Selected;
};

#endif