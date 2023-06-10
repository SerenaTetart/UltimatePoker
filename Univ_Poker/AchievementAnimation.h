#ifndef ACHIEVEMENTANIMATION_H
#define ACHIEVEMENTANIMATION_H

#include <vector>
#include "Achievement.h"
#include "GameObject.h"

class AchievementAnimation : public GameObject {
public:
	AchievementAnimation(Properties props);
	~AchievementAnimation();

	virtual void Events(SDL_Event* ev);
	virtual void Update(float dt);
	virtual void Draw();
	virtual void Clean();

	void Refresh();
	inline void PlayAchievement(Achievement* ach) { m_AchievementVec.push_back(ach); }

private:
	std::vector<Achievement*> m_AchievementVec;
	SDL_Texture* m_CurrentAchievement;
	Uint64 m_Timer;
};

#endif