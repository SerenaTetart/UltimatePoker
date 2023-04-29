#ifndef BUTTON_H
#define BUTTON_H

#include "GameObject.h"
#include "TextFrame.h"

class Button : public GameObject {
public:
	Button(Properties props, void(*functionPtr)(), int size = 28, int police = 0, std::string text = "", SDL_Color color = {255,255,255}, bool keepColor=false, bool active=true);
	~Button();

	virtual void Events(SDL_Event* ev);
	virtual void Update(float dt);
	virtual void Draw();
	virtual void Clean();

	void Refresh();
	inline void ChangeFunction(void(*functionPtr)()) { m_FunctionPtr = functionPtr; };
	inline void SetKeepColor(bool keep) { m_KeepColor = keep; if(keep) m_Frame->SetText(m_Size, m_Police, m_Text, {255, 0, 0}); else m_Frame->SetText(m_Size, m_Police, m_Text, m_Color); };
	inline void SetActive(bool active) { m_Active = active; }
	inline bool GetActive() { return m_Active; }

private:
	bool m_IsSelected, m_KeepColor;
	TextFrame* m_Frame;
	void (*m_FunctionPtr)();
	//....//
	int m_Size, m_Police;
	std::string m_Text;
	SDL_Color m_Color;
	bool m_Active;
	//....//
};

#endif