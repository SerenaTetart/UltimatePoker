#include "Player.h"
#include "TextureManager.h"

Player::Player(std::string name, int position, int money) {
	m_Money = money; m_BetMoney = 0;
	m_Position = position;
	m_Name = name;
	//=== Charge les cartes du joueur ===//
	int pos_x = 0, pos_y = 0; double angle = 0.0;
	if (m_Position == 1) { pos_x = 558; pos_y = 705; }
	else if (m_Position == 2) { pos_x = 992; pos_y = 370; angle = 270; }
	else if (m_Position == 3) { pos_x = 558; pos_y = 140; angle = 180; }
	else if (m_Position == 4) { pos_x = 210; pos_y = 380; angle = 90; }
	else if (m_Position == 5) { pos_x = 886; pos_y = 619; angle = 330; }
	else if (m_Position == 6) { pos_x = 810; pos_y = 175; angle = 210; }
	else if (m_Position == 7) { pos_x = 323; pos_y = 210; angle = 152; }
	else if (m_Position == 8) { pos_x = 388; pos_y = 670; angle = 30; }
	m_Hand[0] = new Card(Properties("", pos_x, pos_y, 80, 117, 1.0, 1.0, angle), 1, 1, true);
	if (m_Position == 1) { pos_x = 642; pos_y = 705; }
	else if (m_Position == 2) { pos_x = 992; pos_y = 455; }
	else if (m_Position == 3) { pos_x = 642; pos_y = 140; }
	else if (m_Position == 4) { pos_x = 210; pos_y = 465; }
	else if (m_Position == 5) { pos_x = 815; pos_y = 660; }
	else if (m_Position == 6) { pos_x = 883; pos_y = 218; }
	else if (m_Position == 7) { pos_x = 396; pos_y = 170; }
	else if (m_Position == 8) { pos_x = 315; pos_y = 626; }
	m_Hand[1] = new Card(Properties("", pos_x, pos_y, 80, 117, 1.0, 1.0, angle), 1, 1, true);
	SetMoney(money);
	m_Folded = false; m_Surrendered = false;
}

Player::~Player() {
	delete(m_Hand[0]);
	delete(m_Hand[1]);
	delete(m_PlayerInfo);
}

void Player::SetCardValue(int card, int value, int type) {
	if(card >= 0 && card < 2) m_Hand[card]->SetValue(value, type);
}

void Player::SetBetMoney(int money) {
	m_BetMoney = money;
	for (int i = 0; i < MAXTOKEN; i++) {
		if (m_BetToken[i] != nullptr) delete(m_BetToken[i]);
	}
	//=== Charge les jetons du joueur ===//
	for (int i = 0; i < MAXTOKEN; i++) {
		m_BetToken[i] = nullptr;
	}
	int tmpMoney = m_BetMoney, nbrToken[5] = { 0 }, nbrTokenTotal = 0;
	while (tmpMoney > 0) {
		if (tmpMoney - 5000 >= 0) { tmpMoney -= 5000; nbrToken[0]++; nbrTokenTotal++; }
		else if (tmpMoney - 1000 >= 0) { tmpMoney -= 1000; nbrToken[1]++; nbrTokenTotal++; }
		else if (tmpMoney - 500 >= 0) { tmpMoney -= 500; nbrToken[2]++; nbrTokenTotal++; }
		else if (tmpMoney - 100 >= 0) { tmpMoney -= 100; nbrToken[3]++; nbrTokenTotal++; }
		else if (tmpMoney - 25 >= 0) { tmpMoney -= 25; nbrToken[4]++; nbrTokenTotal++; }
	}
	if (nbrTokenTotal > MAXTOKEN) nbrTokenTotal = MAXTOKEN;
	int value, inc = 0;
	for (int i = 0; i < 5; i++) {
		if (i == 0) value = 5000;
		else if (i == 1) value = 1000;
		else if (i == 2) value = 500;
		else if (i == 3) value = 100;
		else value = 25;
		for (int y = 0; y < nbrToken[i] && inc < MAXTOKEN; y++) {
			if (m_Position == 1) m_BetToken[inc] = new Token(Properties("", 610 - (7 * (nbrTokenTotal - 1)) + inc * 14, 635, 115, 115, 0.5, 0.5), value);
			else if (m_Position == 2) m_BetToken[inc] = new Token(Properties("", 905, 445 - (7 * (nbrTokenTotal - 1)) + inc * 14, 115, 115, 0.5, 0.5), value);
			else if (m_Position == 3) m_BetToken[inc] = new Token(Properties("", 610 + (7 * (nbrTokenTotal - 1)) - inc * 14, 270, 115, 115, 0.5, 0.5), value);
			else if (m_Position == 4) m_BetToken[inc] = new Token(Properties("", 320, 450 + (7 * (nbrTokenTotal - 1)) - inc * 14, 115, 115, 0.5, 0.5), value);
			else if (m_Position == 5) m_BetToken[inc] = new Token(Properties("", 810 - (4 * (nbrTokenTotal - 1)) + inc * 11, 585 + (3 * (nbrTokenTotal - 1)) - inc * 6, 115, 115, 0.5, 0.5), value);
			else if (m_Position == 6) m_BetToken[inc] = new Token(Properties("", 805 - (4 * (nbrTokenTotal - 1)) + inc * 11, 308 - (3 * (nbrTokenTotal - 1)) + inc * 6, 115, 115, 0.5, 0.5), value);
			else if (m_Position == 7) m_BetToken[inc] = new Token(Properties("", 415 - (4 * (nbrTokenTotal - 1)) + inc * 11, 305 + (2.5 * (nbrTokenTotal - 1)) - inc * 6.5, 115, 115, 0.5, 0.5), value);
			else if (m_Position == 8) m_BetToken[inc] = new Token(Properties("", 415 - (4.5 * (nbrTokenTotal - 1)) + inc * 11, 600 - (3 * (nbrTokenTotal - 1)) + inc * 6.5, 115, 115, 0.5, 0.5), value);
			inc++;
		}
	}
}

void Player::SetMoney(int money) {
	m_Money = money;
	for (int i = 0; i < MAXTOKEN; i++) {
		if (m_Token[i] != nullptr) delete(m_Token[i]);
	}
	//=== Charge les jetons du joueur ===//
	for (int i = 0; i < MAXTOKEN; i++) {
		m_Token[i] = nullptr;
	}
	int tmpMoney = m_Money, nbrToken[5] = { 0 }, nbrTokenTotal = 0;
	while (tmpMoney > 0) {
		if (tmpMoney - 5000 >= 0) { tmpMoney -= 5000; nbrToken[0]++; nbrTokenTotal++; }
		else if (tmpMoney - 1000 >= 0) { tmpMoney -= 1000; nbrToken[1]++; nbrTokenTotal++; }
		else if (tmpMoney - 500 >= 0) { tmpMoney -= 500; nbrToken[2]++; nbrTokenTotal++; }
		else if (tmpMoney - 100 >= 0) { tmpMoney -= 100; nbrToken[3]++; nbrTokenTotal++; }
		else if (tmpMoney - 25 >= 0) { tmpMoney -= 25; nbrToken[4]++; nbrTokenTotal++; }
	}
	if (nbrTokenTotal > MAXTOKEN) nbrTokenTotal = MAXTOKEN;
	int value, inc = 0;
	for (int i = 0; i < 5; i++) {
		if (i == 0) value = 5000;
		else if (i == 1) value = 1000;
		else if (i == 2) value = 500;
		else if (i == 3) value = 100;
		else value = 25;
		for (int y = 0; y < nbrToken[i] && inc < MAXTOKEN; y++) {
			if (m_Position == 1) m_Token[inc] = new Token(Properties("", 642 - 145, 705 + 45 - (12 * (nbrTokenTotal - 1)) + inc * 14, 115, 115, 0.5, 0.5), value);
			else if (m_Position == 2) m_Token[inc] = new Token(Properties("", 992 + 20 - (12 * (nbrTokenTotal - 1)) + inc * 14, 455 + 102, 115, 115, 0.5, 0.5), value);
			else if (m_Position == 3) m_Token[inc] = new Token(Properties("", 642 + 85, 140 + 25 + (12 * (nbrTokenTotal - 1)) - inc * 14, 115, 115, 0.5, 0.5), value);
			else if (m_Position == 4) m_Token[inc] = new Token(Properties("", 210 + 15 + (12 * (nbrTokenTotal - 1)) - inc * 14, 465 - 128, 115, 115, 0.5, 0.5), value);
			else if (m_Position == 5) m_Token[inc] = new Token(Properties("", 815 - 50 - (4 * (nbrTokenTotal - 1)) + inc * 5.5, 660 + 65 - (6 * (nbrTokenTotal - 1)) + inc * 9, 115, 115, 0.5, 0.5), value);
			else if (m_Position == 6) m_Token[inc] = new Token(Properties("", 883 + 75 - (4 * (nbrTokenTotal - 1)) + inc * 5.5, 218 + 70 + (6 * (nbrTokenTotal - 1)) - inc * 9, 115, 115, 0.5, 0.5), value);
			else if (m_Position == 7) m_Token[inc] = new Token(Properties("", 396 + 75 + (3.5 * (nbrTokenTotal - 1)) - inc * 4.5, 170 + (6 * (nbrTokenTotal - 1)) - inc * 9, 115, 115, 0.5, 0.5), value);
			else if (m_Position == 8) m_Token[inc] = new Token(Properties("", 315 - 50 + (3.5 * (nbrTokenTotal - 1)) - inc * 5.5, 626 - 5 - (7 * (nbrTokenTotal - 1)) + inc * 10, 115, 115, 0.5, 0.5), value);
			inc++;
		}
	}
	//=== Charge le texte du joueur ===//
	int pos_x = 0, pos_y = 0; double angle = 0.0; SDL_Color color = { 255, 255, 255 };
	if (m_Position == 1) { pos_x = 490; pos_y = 835; angle = 0; color = { 255, 0, 0 }; }
	else if (m_Position == 2) { pos_x = 998; pos_y = 455; angle = 90; color = { 0, 225, 0 }; }
	else if (m_Position == 3) { pos_x = 500; pos_y = 70; angle = 0; color = { 0, 50, 225 }; }
	else if (m_Position == 4) { pos_x = -3; pos_y = 450; angle = 270; color = { 255, 125, 0 }; }
	else if (m_Position == 5) { pos_x = 785; pos_y = 764; angle = 330; color = { 255, 0, 255 }; }
	else if (m_Position == 6) { pos_x = 800; pos_y = 150; angle = 30; color = { 0, 255, 255 }; }
	else if (m_Position == 7) { pos_x = 210; pos_y = 140; angle = 330; color = { 125, 75, 50 };}
	else if (m_Position == 8) { pos_x = 190; pos_y = 753; angle = 30; color = { 125, 0, 125 };}
	if(m_PlayerInfo == nullptr) m_PlayerInfo = new TextFrame(Properties("txtframe_1", pos_x, pos_y, 236, 93, 1.2, 0.6, angle), 28, 0, m_Name + ": ", color, std::to_string(m_Money) + " Or", { 255, 215, 0 });
	else m_PlayerInfo->SetText(28, 0, m_Name + ": ", color, std::to_string(m_Money) + " Or", { 255, 215, 0 });
}

void Player::RefreshFrame() {
	//=== Charge le texte du joueur ===//
	int pos_x = 0, pos_y = 0; double angle = 0.0; SDL_Color color = { 255, 255, 255 };
	if (m_Position == 1) { pos_x = 490; pos_y = 835; angle = 0; color = { 255, 0, 0 }; }
	else if (m_Position == 2) { pos_x = 998; pos_y = 455; angle = 90; color = { 0, 225, 0 }; }
	else if (m_Position == 3) { pos_x = 500; pos_y = 70; angle = 0; color = { 0, 50, 225 }; }
	else if (m_Position == 4) { pos_x = -3; pos_y = 450; angle = 270; color = { 255, 125, 0 }; }
	else if (m_Position == 5) { pos_x = 785; pos_y = 764; angle = 330; color = { 255, 0, 255 }; }
	else if (m_Position == 6) { pos_x = 800; pos_y = 150; angle = 30; color = { 0, 255, 255 }; }
	else if (m_Position == 7) { pos_x = 210; pos_y = 140; angle = 330; color = { 125, 75, 50 }; }
	else if (m_Position == 8) { pos_x = 190; pos_y = 753; angle = 30; color = { 125, 0, 125 }; }
	m_PlayerInfo->SetText(28, 0, "Joueur" + std::to_string(m_Position) + ": ", color, std::to_string(m_Money) + " Or", { 255, 215, 0 });
}

void Player::Draw() {
	m_Hand[0]->Draw(); m_Hand[1]->Draw();
	for (int i = 0; i < MAXTOKEN; i++) {
		if (m_Token[i] != nullptr) m_Token[i]->Draw();
	}
	for (int i = 0; i < MAXTOKEN; i++) {
		if (m_BetToken[i] != nullptr) m_BetToken[i]->Draw();
	}
	m_PlayerInfo->Draw();
}