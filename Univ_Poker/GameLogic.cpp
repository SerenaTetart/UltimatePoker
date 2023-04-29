#include "Game.h"

#include "TextureManager.h"

#include <random>

void Game::Bet(int playerNbr,int bet, bool ok) {
	int gold = m_Player[playerNbr]->GetMoney();
	int betGold = m_Player[playerNbr]->GetBetMoney();
	m_Player[playerNbr]->SetMoney(gold + betGold - bet);
	m_Player[playerNbr]->SetBetMoney(bet);
	m_LastBet = bet;
	if(!ok) m_Player[playerNbr]->SetBetted(true);
	if(playerNbr != 0) SDL_Delay(500);
}

void Game::AdjustBet(int lowestBet) {
	//Egalise la somme des enchéres par rapport à la plus petite mise
	for (int i = 0; i < m_NbrPlayer; i++) {
		int gold = m_Player[i]->GetMoney();
		int bet = m_Player[i]->GetBetMoney();
		if (bet > lowestBet) {
			m_Player[i]->SetBetMoney(lowestBet);
			m_Player[i]->SetMoney(gold+bet-lowestBet);
		}
	}
}

void Game::StartRound(int start_pos) {
	TextureManager::GetInstance()->Drop("showdowntxt");
	//=== Mélanger les cartes ===//
	srand((unsigned)time(NULL));
	std::vector<int> listCard = {};
	for (int i = 0; i < NBRCARDTYPE * NBRTYPE; i++) {
		listCard.insert(listCard.begin()+i, i+1);
	}
	for(int i = 0; i < 5; i++) {
		std::vector<int>::iterator ptr; int randomNbr;
		do {
			randomNbr = rand() % 52;
			ptr = std::find(listCard.begin(), listCard.end(), randomNbr + 1);
		} while (ptr == listCard.end());
		listCard.erase(ptr);
		int num_card = (randomNbr % NBRCARDTYPE)+1;
		int type_card = (randomNbr / NBRCARDTYPE)+1;
		m_Board[i]->SetValue(num_card, type_card);
	}
	for (int i = 0; i < m_NbrPlayer; i++) {
		for (int y = 0; y < 2; y++) {
			std::vector<int>::iterator ptr; int randomNbr;
			do {
				randomNbr = rand() % 52;
				ptr = std::find(listCard.begin(), listCard.end(), randomNbr + 1);
			} while (ptr == listCard.end());
			listCard.erase(ptr);
			int num_card = (randomNbr % NBRCARDTYPE) + 1;
			int type_card = (randomNbr / NBRCARDTYPE) + 1;
			m_Player[i]->SetCardValue(y, num_card, type_card);
		}
	}
	//===========================//
	if (!m_Player[0]->GetSurrender()) m_Player[0]->MaskCards(false); //Révéle les cartes du joueur
	int inc = 0;
	for (inc = start_pos; inc < m_NbrPlayer+start_pos; inc++) {
		int tmp = inc % m_NbrPlayer;
		if (!m_Player[m_PlayerOrder[tmp]]->GetSurrender()) {
			Bet(m_PlayerOrder[tmp], m_BB / 2, true); //Mise du SB
			break;
		}
	}
	for (inc++; inc < m_NbrPlayer + start_pos; inc++) {
		int tmp = inc % m_NbrPlayer;
		if (!m_Player[m_PlayerOrder[tmp]]->GetSurrender()) {
			Bet(m_PlayerOrder[tmp], m_BB, true); //Mise du BB
			break;
		}
	}
	m_GameTurnFrame->SetText(28, 0, "Debut du round", { 255, 215, 0 });
	m_Start_pos = (inc + 1) % m_NbrPlayer;
	std::cout << "=== Debut round ! ===\n";
	Game::BetPhase(m_Start_pos);
}

void Game::BetPhase(int pos) {
	if (m_StopThread) return;
	for (int i = 0; i < m_NbrPlayer; i++) {
		if (m_Player[i]->GetMoney() == 0 && !m_Player[i]->GetSurrender() && !m_Player[i]->HasBetted()) {
			GatherPot(); //Si tapis -> Gather()
			return;
		}
	}
	if (pos == m_Start_pos && m_Player[m_PlayerOrder[pos]]->HasBetted()) GatherPot(); //Si on a fait le tour -> Gather()
	else if (!m_Player[m_PlayerOrder[pos]]->GetSurrender() && !m_Player[m_PlayerOrder[pos]]->GetFold()) {
		std::cout << m_PlayerOrder[pos] + 1 << " play\n";
		bool everyoneFold = true;
		for (int i = pos + 1; i < m_NbrPlayer + pos; i++) {
			int tmp = i % m_NbrPlayer;
			if (!m_Player[m_PlayerOrder[tmp]]->GetFold() && !m_Player[m_PlayerOrder[tmp]]->GetSurrender()) everyoneFold = false;
		}
		if (everyoneFold) { GatherPot(); return; }
		if (pos == 0) {
			//Joueur
			m_PlayerTurn = true;
			//Attendre que le joueur est appuyé sur un bouton
			while (m_PlayerTurn == true) {
				SDL_Delay(500);
				if (m_StopThread) return;
			}
			Game::GetInstance()->BetPhase(1);
		}
		else {
			//I.A débile: 50% Check ou tapis || 25% Raise ou tapis || 25% Fold
			int randomNbr = rand() % 4;
			bool lastPlayer = true;
			for (int i = pos; ((i+1) % m_NbrPlayer) != m_Start_pos; i++) {
				if (!m_Player[m_PlayerOrder[i % m_NbrPlayer]]->GetSurrender() && !m_Player[m_PlayerOrder[i % m_NbrPlayer]]->GetFold()) {
					lastPlayer = false;
					break;
				}
			}
			if (lastPlayer) { //Inutile d'augmenter la mise si on est dernier
				while(randomNbr == 2) { randomNbr = rand() % 4; }
			}
			if(randomNbr == 3 && m_LastBet == 0) randomNbr = rand() % 3; //Inutile de se coucher si la mise précédente est nulle
			if (randomNbr == 0 || randomNbr == 1) {
				//Check
				int gold = m_Player[m_PlayerOrder[pos]]->GetMoney();
				int bet = m_LastBet; if (gold < m_LastBet) bet = gold;
				Bet(m_PlayerOrder[pos], bet);
				if (bet < m_LastBet) AdjustBet(bet);
			}
			else if (randomNbr == 2) {
				//Raise par une valeur random
				int gold = m_Player[m_PlayerOrder[pos]]->GetMoney(); int bet;
				if (gold < m_LastBet) bet = gold; //Tapis
				else {
					srand((unsigned)time(NULL));
					bet = ((rand() % 6) + 1) * 25 + m_LastBet; //Jusqu'à +150
					if (bet > gold) bet = gold; //Tapis
				}
				Bet(m_PlayerOrder[pos], bet);
				if (bet < m_LastBet) AdjustBet(bet);
			}
			else {
				std::cout << "fold !\n";
				m_Player[m_PlayerOrder[pos]]->Fold(true); //Fold
				SDL_Delay(500);
			}
			BetPhase((pos + 1) % m_NbrPlayer);
		}
	}
	else {
		std::cout << m_PlayerOrder[pos]+1 << " pass\n";
		m_Player[m_PlayerOrder[pos]]->SetBetted(true);
		BetPhase((pos + 1) % m_NbrPlayer);
	}
}

void Game::GatherPot() {
	if (m_StopThread) return;
	SDL_Delay(500);
	//Rassemble l'argent des enchéres dans le pot
	m_PlayerTurn = false;
	srand((unsigned)time(NULL));
	for (int i = 0; i < m_NbrPlayer; i++) {
		int bet = m_Player[i]->GetBetMoney();
		m_PotGold += bet;
		m_BoardFrame->SetText(28, 0, "Pot: ", { 128, 128, 128 }, std::to_string(m_PotGold) + " Or", { 255, 215, 0 });
		m_Player[i]->SetBetMoney(0);
		m_LastBet = 0;
		m_Player[i]->SetBetted(false);
	}
	if (m_Board[0]->GetMasked() == true) {
		//Phase flop
		std::cout << "=== Phase Flop ===\n";
		m_Board[0]->SetMasked(false);
		m_Board[1]->SetMasked(false);
		m_Board[2]->SetMasked(false);
		m_GameTurnFrame->SetText(28, 0, "Phase Flop", { 255, 215, 0 });
		BetPhase(m_Start_pos);
	}
	else if (m_Board[3]->GetMasked() == true) {
		//Phase river
		std::cout << "=== Phase River ===\n";
		m_Board[3]->SetMasked(false);
		m_GameTurnFrame->SetText(28, 0, "Phase River", { 255, 215, 0 });
		BetPhase(m_Start_pos);
	}
	else if (m_Board[4]->GetMasked() == true) {
		//Phase turn
		std::cout << "=== Phase Turn ===\n";
		m_Board[4]->SetMasked(false);
		m_GameTurnFrame->SetText(28, 0, "Phase Turn", { 255, 215, 0 });
		BetPhase(m_Start_pos);
	}
	else Showdown();
}

void Game::Showdown() {
	if (m_StopThread) return;
	SDL_Delay(1000);
	std::cout << "=== Showdown ===\n";
	m_GameTurnFrame->SetText(28, 0, "Showdown !", { 255, 215, 0 });
	for (int i = 0; i < m_NbrPlayer; i++) {
		if(!m_Player[i]->GetSurrender()) m_Player[i]->MaskCards(false);
	}
	//Détermine la main la plus forte
	int score_max = 0; int ind_max = 0; int score_secour = 0;
	for (int i = 0; i < m_NbrPlayer; i++) {
		std::cout << "Joueur" << m_PlayerOrder[i]+1 << "\n";
		if (!m_Player[m_PlayerOrder[i]]->GetFold() && !m_Player[m_PlayerOrder[i]]->GetSurrender()) {
			int listCardValue[7], listCardType[7];
			for (int y = 0; y < 5; y++) {
				listCardValue[y] = m_Board[y]->GetValue();
				listCardType[y] = m_Board[y]->GetType();
			}
			for (int y = 5; y < 7; y++) {
				listCardValue[y] = m_Player[m_PlayerOrder[i]]->GetCardValue(y - 5);
				listCardType[y] = m_Player[m_PlayerOrder[i]]->GetCardType(y - 5);
			}
			//Tri à bulle
			for (int y = 0; y < (7 - 1); y++) {
				for (int j = 0; j < 7 - y - 1; j++) {
					if (listCardValue[j] > listCardValue[j + 1]) {
						int tmp = listCardValue[j];
						listCardValue[j] = listCardValue[j + 1];
						listCardValue[j + 1] = tmp;
						tmp = listCardType[j];
						listCardType[j] = listCardType[j + 1];
						listCardType[j + 1] = tmp;
					}
				}
			}
			//Quinte flush
			int inc = 0; int comb_value = 0;
			for (int y = 0; y < 7-1; y++) {
				for (int j = y + 1; j < 7 - 1; j++) {
					if (listCardValue[j] == listCardValue[y] + 1 && listCardType[j] == listCardType[y]) {
						inc++; y = j; comb_value = listCardValue[j]; break;
					}
				}
			}
			if (inc >= 4) {
				std::cout << "QUINTE FLUSH !\n";
				if (score_max < 9* (NBRCARDTYPE + 1) +comb_value) { score_max = 9 * (NBRCARDTYPE+1) + comb_value; ind_max = i; }
				continue;
			}
			//Carre
			inc = 0;
			for (int y = 0; y < (7 - 1); y++) {
				if (listCardValue[y + 1] == listCardValue[y]) { inc++; comb_value = listCardValue[y+1]; }
				else inc = 0;
				if (inc >= 3) break;
			}
			if (inc >= 3) {
				std::cout << "CARRE !\n";
				if (score_max < 8 * (NBRCARDTYPE + 1) + comb_value) { score_max = 8 * (NBRCARDTYPE + 1) + comb_value; ind_max = i; }
				else if (score_max == 8 * (NBRCARDTYPE + 1) + comb_value) {
					//Départager avec la carte forte
					if (m_Player[m_PlayerOrder[i]]->GetCardValue(0) != comb_value) {
						if (m_Player[m_PlayerOrder[ind_max]]->GetCardValue(0) != comb_value) {
							if (m_Player[m_PlayerOrder[i]]->GetCardValue(0) > m_Player[m_PlayerOrder[ind_max]]->GetCardValue(0)) {
								score_max = 8 * (NBRCARDTYPE + 1) + comb_value; ind_max = i;
							}
						}
						else if (m_Player[m_PlayerOrder[i]]->GetCardValue(0) > m_Player[m_PlayerOrder[ind_max]]->GetCardValue(1)) {
							score_max = 8 * (NBRCARDTYPE + 1) + comb_value; ind_max = i;
						}
					}
					else if (m_Player[m_PlayerOrder[ind_max]]->GetCardValue(0) != comb_value) {
						if (m_Player[m_PlayerOrder[i]]->GetCardValue(1) > m_Player[m_PlayerOrder[ind_max]]->GetCardValue(0)) {
							score_max = 8 * (NBRCARDTYPE + 1) + comb_value; ind_max = i;
						}
					}
					else if (m_Player[m_PlayerOrder[i]]->GetCardValue(1) > m_Player[m_PlayerOrder[ind_max]]->GetCardValue(1)) {
						score_max = 8 * (NBRCARDTYPE + 1) + comb_value; ind_max = i;
					}
				}
				continue;
			}
			//Full (Brelan + pair)
			inc = 0; comb_value = 0;
			for (int y = 0; y < (7 - 1); y++) {
				if (listCardValue[y + 1] == listCardValue[y]) { inc++; comb_value = listCardValue[y+1]; }
				else inc = 0;
				if (inc >= 2) break;
			}
			if (inc >= 2) {
				inc = 0; int comb_value2 = 0;
				for (int y = 0; y < (7 - 1); y++) {
					if (listCardValue[y + 1] == listCardValue[y] && listCardValue[y] != comb_value) { inc++; comb_value2 = listCardValue[y + 1]; }
					else inc = 0;
					if (inc >= 1) break;
				}
				if (inc >= 1) {
					std::cout << "FULL !\n";
					if (score_max < 7 * (NBRCARDTYPE + 1) + comb_value) { score_max = 7 * (NBRCARDTYPE + 1) + comb_value; score_secour = comb_value2; ind_max = i; }
					else if (score_max == 7 * (NBRCARDTYPE + 1) + comb_value) {
						if(score_secour < comb_value2) { score_max = 7 * (NBRCARDTYPE + 1) + comb_value; score_secour = comb_value2; ind_max = i; }
						else if (comb_value2 == score_secour) {
							//Départager avec la carte forte
							if (m_Player[m_PlayerOrder[i]]->GetCardValue(0) != comb_value && m_Player[m_PlayerOrder[i]]->GetCardValue(0) != comb_value2) {
								if (m_Player[m_PlayerOrder[ind_max]]->GetCardValue(0) != comb_value && m_Player[m_PlayerOrder[ind_max]]->GetCardValue(0) != comb_value2) {
									if (m_Player[m_PlayerOrder[i]]->GetCardValue(0) > m_Player[m_PlayerOrder[ind_max]]->GetCardValue(0)) {
										score_max = 7 * (NBRCARDTYPE + 1) + comb_value; score_secour = comb_value2; ind_max = i;
									}
								}
								else if (m_Player[m_PlayerOrder[i]]->GetCardValue(0) > m_Player[m_PlayerOrder[ind_max]]->GetCardValue(1)) {
									score_max = 7 * (NBRCARDTYPE + 1) + comb_value; score_secour = comb_value2; ind_max = i;
								}
							}
							else if (m_Player[m_PlayerOrder[ind_max]]->GetCardValue(0) != comb_value && m_Player[m_PlayerOrder[ind_max]]->GetCardValue(0) != comb_value2) {
								if (m_Player[m_PlayerOrder[i]]->GetCardValue(1) > m_Player[m_PlayerOrder[ind_max]]->GetCardValue(0)) {
									score_max = 7 * (NBRCARDTYPE + 1) + comb_value; score_secour = comb_value2; ind_max = i;
								}
							}
							else if (m_Player[m_PlayerOrder[i]]->GetCardValue(1) > m_Player[m_PlayerOrder[ind_max]]->GetCardValue(1)) {
								score_max = 7 * (NBRCARDTYPE + 1) + comb_value; score_secour = comb_value2; ind_max = i;
							}
						}
					}
					continue;
				}
			}
			//Couleur
			for (int y = 0; y < 7 - 1; y++) {
				inc = 0;
				for (int j = y + 1; j < 7 - 1; j++) {
					if (listCardType[j] == listCardType[y]) { inc++; comb_value = listCardValue[j]; }
				}
				if (inc >= 4) break;
			}
			if (inc >= 4) {
				std::cout << "COULEUR !\n";
				if (score_max < 6 * (NBRCARDTYPE + 1) + comb_value) { score_max = 6 * (NBRCARDTYPE + 1) + comb_value; ind_max = i; }
				continue;
			}
			//Quinte
			inc = 0;
			for (int y = 0; y < (7 - 1); y++) {
				if (listCardValue[y + 1] == listCardValue[y] + 1) { inc++; comb_value = listCardValue[y + 1]; }
				else inc = 0;
				if (inc >= 4) break;
			}
			if (inc >= 4) {
				std::cout << "QUINTE !\n";
				if (score_max < 5 * (NBRCARDTYPE + 1) + comb_value) { score_max = 5 * (NBRCARDTYPE + 1) + comb_value; ind_max = i; }
				continue;
			}
			//Brelan
			inc = 0; int comb_value2 = 0;
			for (int y = 0; y < (7 - 1); y++) {
				if (listCardValue[y + 1] == listCardValue[y]) { inc++; comb_value = listCardValue[y + 1]; }
				else if (inc >= 2) break;
				else inc = 0;
			}
			if (inc >= 2) {
				std::cout << "BRELAN !\n";
				if (m_Player[m_PlayerOrder[i]]->GetCardValue(0) != comb_value) comb_value2 = m_Player[m_PlayerOrder[i]]->GetCardValue(0);
				else comb_value2 = m_Player[m_PlayerOrder[i]]->GetCardValue(1);
				if (score_max < 4 * (NBRCARDTYPE + 1) + comb_value) { score_max = 4 * (NBRCARDTYPE + 1) + comb_value; score_secour = comb_value2; ind_max = i; }
				else if (score_max == 4 * (NBRCARDTYPE + 1) + comb_value) {
					if(comb_value2 > score_secour) { score_max = 4 * (NBRCARDTYPE + 1) + comb_value; score_secour = comb_value2; ind_max = i; }
				}
				continue;
			}
			//Double Pair
			inc = 0; comb_value = 0;
			for (int y = 0; y < (7 - 1); y++) {
				if (listCardValue[y + 1] == listCardValue[y]) { inc++; comb_value2 = listCardValue[y]; }
				else inc = 0;
				if (inc >= 1) break;
			}
			if (inc >= 1) {
				inc = 0;
				for (int y = 0; y < (7 - 1); y++) {
					if (listCardValue[y + 1] == listCardValue[y] && listCardValue[y] != comb_value2) { inc++; comb_value = listCardValue[y + 1]; }
					else inc = 0;
					if (inc >= 1) break;
				}
				if (inc >= 1) {
					std::cout << "DOUBLE PAIR !\n";
					if (score_max < 3 * (NBRCARDTYPE + 1) + comb_value) { score_max = 3 * (NBRCARDTYPE + 1) + comb_value; score_secour = comb_value2; ind_max = i; }
					else if (score_max == 3 * (NBRCARDTYPE + 1) + comb_value) {
						if(comb_value2 > score_secour) { score_max = 3 * (NBRCARDTYPE + 1) + comb_value; score_secour = comb_value2; ind_max = i; }
						else if (comb_value2 == score_secour) {
							//Départager avec la carte forte
							if (m_Player[m_PlayerOrder[i]]->GetCardValue(0) != comb_value && m_Player[m_PlayerOrder[i]]->GetCardValue(0) != comb_value2) {
								if (m_Player[m_PlayerOrder[ind_max]]->GetCardValue(0) != comb_value && m_Player[m_PlayerOrder[ind_max]]->GetCardValue(0) != comb_value2) {
									if (m_Player[m_PlayerOrder[i]]->GetCardValue(0) > m_Player[m_PlayerOrder[ind_max]]->GetCardValue(0)) {
										score_max = 3 * (NBRCARDTYPE + 1) + comb_value; score_secour = comb_value2; ind_max = i;
									}
								}
								else if (m_Player[m_PlayerOrder[i]]->GetCardValue(0) > m_Player[m_PlayerOrder[ind_max]]->GetCardValue(1)) {
									score_max = 3 * (NBRCARDTYPE + 1) + comb_value; score_secour = comb_value2; ind_max = i;
								}
							}
							else if (m_Player[m_PlayerOrder[ind_max]]->GetCardValue(0) != comb_value && m_Player[m_PlayerOrder[ind_max]]->GetCardValue(0) != comb_value2) {
									if (m_Player[m_PlayerOrder[i]]->GetCardValue(1) > m_Player[m_PlayerOrder[ind_max]]->GetCardValue(0)) {
										score_max = 3 * (NBRCARDTYPE + 1) + comb_value; score_secour = comb_value2; ind_max = i;
									}
							}
							else if (m_Player[m_PlayerOrder[i]]->GetCardValue(1) > m_Player[m_PlayerOrder[ind_max]]->GetCardValue(1)) {
								score_max = 3 * (NBRCARDTYPE + 1) + comb_value; score_secour = comb_value2; ind_max = i;
							}
						}
					}
					continue;
				}
			}
			//Pair
			inc = 0;
			for (int y = 0; y < (7 - 1); y++) {
				if (listCardValue[y + 1] == listCardValue[y]) { inc++; comb_value = listCardValue[y + 1];}
				else inc = 0;
				if (inc >= 1) break;
			}
			if (inc >= 1) {
				std::cout << "PAIR !\n";
				if (m_Player[m_PlayerOrder[i]]->GetCardValue(0) != comb_value) {
					if(m_Player[m_PlayerOrder[i]]->GetCardValue(1) == comb_value || m_Player[m_PlayerOrder[i]]->GetCardValue(0) > m_Player[m_PlayerOrder[i]]->GetCardValue(1))
						comb_value2 = m_Player[m_PlayerOrder[i]]->GetCardValue(0);
					else if(m_Player[m_PlayerOrder[i]]->GetCardValue(1) != comb_value) comb_value2 = m_Player[m_PlayerOrder[i]]->GetCardValue(1);
				}
				else comb_value2 = m_Player[m_PlayerOrder[i]]->GetCardValue(1);
				if (score_max < 2 * (NBRCARDTYPE + 1) + comb_value) { score_max = 2 * (NBRCARDTYPE + 1) + comb_value; score_secour = comb_value2; ind_max = i; }
				else if (score_max == 2 * (NBRCARDTYPE + 1) + comb_value) {
					if (comb_value2 > score_secour) { score_max = 2 * (NBRCARDTYPE + 1) + comb_value; score_secour = comb_value2; ind_max = i; }
				}
				continue;
			}
			//Carte forte
			if (m_Player[m_PlayerOrder[i]]->GetCardValue(0) > m_Player[m_PlayerOrder[i]]->GetCardValue(1)) {
				if (score_max < m_Player[m_PlayerOrder[i]]->GetCardValue(0)) { score_max = m_Player[m_PlayerOrder[i]]->GetCardValue(0); ind_max = i; }
			}
			else {
				if (score_max < m_Player[m_PlayerOrder[i]]->GetCardValue(1)) { score_max = m_Player[m_PlayerOrder[i]]->GetCardValue(1); ind_max = i; }
			}
		}
	}
	//=============================//
	int gold = m_Player[m_PlayerOrder[ind_max]]->GetMoney();
	m_Player[m_PlayerOrder[ind_max]]->SetMoney(gold + m_PotGold);
	Game::GetInstance()->m_PotGold = 0;
	Game::GetInstance()->m_BoardFrame->SetText(28, 0, "Pot: ", { 128, 128, 128 }, std::to_string(Game::GetInstance()->m_PotGold) + " Or", { 255, 215, 0 });
	m_GameWonText->SetText(26, 1, "Le vainqueur est " + m_Player[m_PlayerOrder[ind_max]]->GetName(), {255, 215, 0});
	m_ButtonShowdown->SetActive(true);
}