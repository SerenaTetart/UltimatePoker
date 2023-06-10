#ifndef TEXT_H
#define TEXT_H

#include "GameObject.h"

class Text : public GameObject {
public:
	Text(Properties props, int size, int police, std::string text, SDL_Color colorText, std::string text2 = "", SDL_Color colorText2 = { 255, 255, 255 }, bool Floating = false);
	~Text();

	virtual void Events(SDL_Event* ev);
	virtual void Update(float dt);
	virtual void Draw();
	virtual void Clean();

	void Refresh();
	void SetText(int size, int police, std::string text, SDL_Color colorText, std::string text2 = "", SDL_Color colorText2 = { 255, 255, 255 });
	inline bool IsFloating() { return m_Floating; }
	inline Uint64 GetTimer() { return m_Timer; }

private:
	int m_Size, m_Police; Uint64 m_Timer;
	std::string m_Text, m_Text2;
	SDL_Color m_ColorText, m_ColorText2;
	SDL_Texture* m_TextureText;
	bool m_Floating, m_Refresh;
};

#endif