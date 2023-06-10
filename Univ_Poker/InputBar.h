#ifndef INPUTBAR_H
#define INPUTBAR_H

#include "GameObject.h"
#include "TextFrame.h"

class InputBar : public GameObject {
public:
	InputBar(Properties props);
	~InputBar();

	virtual void Events(SDL_Event* ev);
	virtual void Update(float dt);
	virtual void Draw();
	virtual void Clean();

	inline int GetLoad() { return m_Load; }
	inline void SetLoad(int load) { m_Load = load; }

private:
	int m_Load;
	bool m_Selected, m_Dragging;
};

#endif