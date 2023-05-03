#ifndef INPUTBOX_H
#define INPUTBOX_H

#include "GameObject.h"
#include "TextFrame.h"

class InputBox : public GameObject {
public:
	InputBox(Properties props, int size = 28, int police = 0, std::string text = "", SDL_Color color = {255,255,255}, int limit = 10, bool number_only = false);
	~InputBox();

	virtual void Events(SDL_Event* ev);
	virtual void Update(float dt);
	virtual void Draw();
	virtual void Clean();

	inline std::string GetText() { return m_Text; }
	void SetText(std::string string);
	void Refresh();

private:
	bool m_IsSelected, m_NumberOnly;
	TextFrame* m_Frame;
	//....//
	int m_Size, m_Police, m_LimitChar;
	std::string m_Text;
	SDL_Color m_Color;
	//....//
};

#endif