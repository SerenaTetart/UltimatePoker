#ifndef PLAYER_H
#define PLAYER_H

#include <string>

#include "Card.h"
#include "Token.h"
#include "TextFrame.h"

#define MAXTOKEN 12

class Player {
public:
	Player(std::string name, int position, int money);
	~Player();

	void Draw();
	void RefreshFrame();

	inline int GetMoney() { return m_Money; }
	void SetMoney(int money);
	inline int GetBetMoney() { return m_BetMoney; }
	void SetBetMoney(int money);
	void SetCardValue(int card, int value, int type);
	inline void MaskCards(bool turn) { m_Hand[0]->SetMasked(turn); m_Hand[1]->SetMasked(turn); }
	inline void HideCards(bool hide) { m_Hand[0]->SetVisible(!hide); m_Hand[1]->SetVisible(!hide); }
	inline void Fold(bool fold) { SetBetted(fold); m_Folded = fold; HideCards(fold); }
	inline bool GetFold() { return m_Folded; }
	inline void Surrender(bool surrender) { m_Surrendered = surrender; HideCards(surrender);};
	inline bool GetSurrender() { return m_Surrendered; }
	inline void SetBetted(bool betted) { m_Betted = betted; }
	inline bool HasBetted() { return m_Betted; }
	inline int GetCardValue(int card) { return m_Hand[card]->GetValue(); }
	inline int GetCardType(int card) { return m_Hand[card]->GetType(); }
	inline std::string GetName() { return m_Name; }

private:
	Card* m_Hand[2];
	Token* m_Token[MAXTOKEN]; Token* m_BetToken[MAXTOKEN];
	TextFrame* m_PlayerInfo;
	int m_Money, m_BetMoney;
	int m_Position;
	std::string m_Name;
	bool m_Folded, m_Surrendered, m_Betted;
};

#endif