#ifndef CARD_H
#define CARD_H

#include "GameObject.h"

class Card : public GameObject {
	public:
		Card(Properties props, int Value, int Type, bool masked=false, bool Visible = true);

		virtual void Events(SDL_Event* ev);
		virtual void Update(float dt);
		virtual void Draw();
		virtual void Clean();

		void SetVisible(bool visible);
		void SetMasked(bool masked);
		inline bool GetMasked() { return m_Masked; }
		void SetValue(int Value, int Type);
		inline int GetValue() { if (m_Value == 1) return 14; else return m_Value; }
		inline int GetType() { return m_Type; }

	private:
		void ChangeTexture();
		bool m_Visible, m_Masked;
		int m_Value, m_Type; //Value = As; Type = Spade...
};

#endif