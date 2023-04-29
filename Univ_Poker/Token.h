#ifndef TOKEN_H
#define TOKEN_H

#include "GameObject.h"

class Token: public GameObject {
public:
	Token(Properties props, int Value);

	virtual void Events(SDL_Event* ev);
	virtual void Update(float dt);
	virtual void Draw();
	virtual void Clean();

	void SetValue(int Value);

private:
	int m_Value;
};

#endif