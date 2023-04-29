#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <iostream>

#include "Engine.h"

struct Properties {
public:
	Properties(std::string textureID, int x, int y, int width, int height, double scale_x = 1.0, double scale_y = 1.0, double angle = 0.0, SDL_RendererFlip flip = SDL_FLIP_NONE) {
		TextureID = textureID, X = x; Y = y; Width = width; Height = height; Scale_x = scale_x; Scale_y = scale_y; Angle = angle; Flip = flip;
	}

	std::string TextureID;
	int X, Y, Width, Height;
	double Scale_x, Scale_y, Angle;
	SDL_RendererFlip Flip;
};

class GameObject {
public:
	GameObject(Properties prop):
		m_TextureID(prop.TextureID)
		, m_X(prop.X), m_Y(prop.Y), m_Width(prop.Width), m_Height(prop.Height)
		, m_Angle(prop.Angle), m_Scale_x(prop.Scale_x), m_Scale_y(prop.Scale_y)
		, m_Flip(prop.Flip) {};

	virtual void Events(SDL_Event* ev)=0;
	virtual void Update(float dt)=0;
	virtual void Draw()=0;
	virtual void Clean()=0;

protected:
	std::string m_TextureID;
	int m_X, m_Y, m_Width, m_Height;
	double m_Angle, m_Scale_x, m_Scale_y;
	SDL_RendererFlip m_Flip;
};

#endif