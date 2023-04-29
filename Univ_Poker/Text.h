#ifndef TEXT_H
#define TEXT_H

#include "GameObject.h"

class Text : public GameObject {
public:
	Text(Properties props, int size, int police, std::string text, SDL_Color colorText, std::string text2 = "", SDL_Color colorText2 = { 255, 255, 255 }, bool active = true);
	~Text();

	virtual void Events(SDL_Event* ev);
	virtual void Update(float dt);
	virtual void Draw();
	virtual void Clean();

	void Refresh();
	void SetText(int size, int police, std::string text, SDL_Color colorText, std::string text2 = "", SDL_Color colorText2 = { 255, 255, 255 });
	inline void SetActive(bool active) { m_Active = active; }
	inline bool GetActive() { return m_Active; }

private:
	int m_WidthText, m_HeightText;
	int m_Size, m_Police;
	std::string m_Text, m_Text2;
	SDL_Color m_ColorText, m_ColorText2;
	SDL_Texture* m_TextureText;
	bool m_Active, m_Refresh;
};

#endif