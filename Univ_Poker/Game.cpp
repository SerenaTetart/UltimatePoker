#include "Game.h"
#include "TextureManager.h"

Game* Game::s_Instance = nullptr;

Game::Game() {
	//=== Menu ===//
	m_ButtonAIGame = new Button(Properties("txtframe_menu", (SCREEN_WIDTH - 236) * 0.5, SCREEN_HEIGHT * 0.3 - 93 * 0.5, 236, 93, 0.9, 0.8), [](){Game::GetInstance()->SetGameState(STATE_MENU_IA);}, 26, 1, "Jouer contre l'IA", { 255, 215, 0 });
	m_ButtonJoinGame = new Button(Properties("txtframe_menu", SCREEN_WIDTH * 0.4 - (236 * 1.1) * 0.5, SCREEN_HEIGHT * 0.4 - 93 * 0.5, 236, 93, 0.9, 0.8), [](){}, 22, 1, "Rejoindre une partie", { 255, 215, 0 });
	m_ButtonHostGame = new Button(Properties("txtframe_menu", SCREEN_WIDTH * 0.6 - (236*1.1) * 0.5, SCREEN_HEIGHT * 0.4 - 93 * 0.5, 236, 93, 0.9, 0.8), []() {}, 22, 1, "Heberger une partie", { 255, 215, 0 });
	m_ButtonAchievements = new Button(Properties("txtframe_menu", (SCREEN_WIDTH - 236) * 0.5, SCREEN_HEIGHT * 0.5 - 93 * 0.5, 236, 93, 0.9, 0.8), []() {Game::GetInstance()->SetGameState(STATE_ACHIEVEMENTS); }, 26, 1, "Hauts-faits", { 255, 215, 0 });
	m_ButtonOptions = new Button(Properties("txtframe_menu", (SCREEN_WIDTH - 236) * 0.5, SCREEN_HEIGHT * 0.6 - 93 * 0.5, 236, 93, 0.9, 0.8), []() {Game::GetInstance()->SetGameState(STATE_OPTIONS);}, 26, 1, "Options", { 255, 215, 0 });
	m_ButtonExit = new Button(Properties("txtframe_menu", (SCREEN_WIDTH - 236) * 0.5, SCREEN_HEIGHT * 0.8 - 93 * 0.5, 236, 93, 0.9, 0.8), []() {Engine::GetInstance()->Quit();}, 26, 1, "Quitter le jeu", { 255, 215, 0 });
	TextureManager::GetInstance()->LoadTTF("title_menu", "ULTIMATE POKER", { 255, 215, 0 }, 50, 1);
	//=== Options ===//
	TextureManager::GetInstance()->LoadTTF("title_options", "Options", { 255, 215, 0 }, 50, 1);
	m_ButtonSetBasicStyle = new Button(Properties("inputbox_menu", SCREEN_WIDTH*0.4 - 111* 1.3 * 0.5, SCREEN_HEIGHT * 0.59 - 112 * 0.8 * 0.5, 111, 112, 1.3, 0.8), []() {Game::GetInstance()->SetGameStyle("basic");}, 26, 1, "Basique", { 255, 215, 0 }, true);
	m_ButtonSetCosmicStyle = new Button(Properties("inputbox_menu", SCREEN_WIDTH*0.6 - 111 * 1.3 *0.5, SCREEN_HEIGHT * 0.59 - 112*0.8 * 0.5, 111, 112, 1.3, 0.8), []() {Game::GetInstance()->SetGameStyle("cosmic"); }, 26, 1, "Cosmique", { 255, 215, 0 });
	m_ButtonOptionsMenu = new Button(Properties("txtframe_menu", (SCREEN_WIDTH - 236) * 0.5, SCREEN_HEIGHT * 0.8 - 93 * 0.5, 236, 93, 0.9, 0.8), []() {Game::GetInstance()->SetGameState(STATE_MENU); }, 26, 1, "Valider", { 255, 215, 0 });
	TextureManager::GetInstance()->LoadTTF("option_style", "Apparence du plateau de jeu:", { 255, 215, 0 }, 40, 1);
	TextureManager::GetInstance()->LoadTTF("option_volume_title", "Volume:", { 255, 215, 0 }, 40, 1);
	TextureManager::GetInstance()->Load("option_volume", "assets/UI/volume.png");
	TextureManager::GetInstance()->Load("option_volume_line", "assets/UI/volume_line.png");
	m_VolumeOption = new InputBar(Properties("option_volume", SCREEN_WIDTH * 0.45, SCREEN_HEIGHT * 0.35, 439, 49, 0.6));
	m_VolumeInput = new InputBox(Properties("inputbox_menu", SCREEN_WIDTH * 0.57, SCREEN_HEIGHT * 0.35 - (112*0.75*0.5) -10, 111, 112, 0.75, 0.75), 26, 1, "100", { 255, 215, 0 }, 3, true);
	//=== Achievements ===//
	TextureManager::GetInstance()->Load("crown_achievement", "assets/achievements/crown.png");
	TextureManager::GetInstance()->Load("crown_red_achievement", "assets/achievements/crown_red.png");
	TextureManager::GetInstance()->Load("straight_flush_achievement", "assets/achievements/straight_flush.png");
	TextureManager::GetInstance()->Load("four_kind_achievement", "assets/achievements/four_kind.png");
	TextureManager::GetInstance()->Load("full_house_achievement", "assets/achievements/full_house.png");
	TextureManager::GetInstance()->Load("flush_achievement", "assets/achievements/flush.png");
	TextureManager::GetInstance()->Load("straight_achievement", "assets/achievements/straight.png");
	m_Winner = new Achievement("crown_achievement", "Vainqueur", "Gagner une partie quelconque.");
	m_Victorious = new Achievement("crown_red_achievement", "Victorieux", "Gagner une partie contre 7 IA.");
	m_StraightFlush = new Achievement("straight_flush_achievement", "Quinte Flush", "Jouer une quinte flush au moins une fois.");
	m_FourOfAKind = new Achievement("four_kind_achievement", "Carré", "Jouer un carré au moins une fois.");
	m_FullHouse = new Achievement("full_house_achievement", "Full", "Jouer un full au moins une fois.");
	m_Flush = new Achievement("flush_achievement", "Couleur", "Jouer une couleur au moins une fois.");
	m_Straight = new Achievement("straight_achievement", "Quinte", "Jouer une quinte au moins une fois.");
	m_AchievementTab = new AchievementTab(Properties("", SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.2, 0, 0));
	m_AchievementTab->AddAchievement(m_Winner);
	m_AchievementTab->AddAchievement(m_Victorious);
	m_AchievementTab->AddAchievement(m_StraightFlush);
	m_AchievementTab->AddAchievement(m_FourOfAKind);
	m_AchievementTab->AddAchievement(m_FullHouse);
	m_AchievementTab->AddAchievement(m_Flush);
	m_AchievementTab->AddAchievement(m_Straight);
	m_AchievementTab->Refresh();
	TextureManager::GetInstance()->LoadTTF("title_achievements", "Hauts-faits", { 255, 215, 0 }, 50, 1);
	m_AchievementAnimation = new AchievementAnimation(Properties("", SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5, 0, 0, 1.0, 1.0));
	m_ButtonAchievementsMenu = new Button(Properties("txtframe_menu", (SCREEN_WIDTH - 236) * 0.5, SCREEN_HEIGHT * 0.8 - 93 * 0.5, 236, 93, 0.9, 0.8), []() {Game::GetInstance()->SetGameState(STATE_MENU); }, 26, 1, "Retour au menu", { 255, 215, 0 });

	//=== Menu I.A ===//
	TextureManager::GetInstance()->LoadTTF("title_menuAI", "Jouer contre l'I.A", { 255, 215, 0 }, 50, 1);
	TextureManager::GetInstance()->LoadTTF("StartGold_menuAI", "Somme de départ:", { 255, 215, 0 }, 35, 1);
	m_InputBoxStartGold = new InputBox(Properties("inputbox_menu", SCREEN_WIDTH * 0.6 - (111 * 0.9) * 0.5, SCREEN_HEIGHT * 0.3 - (112 * 0.75) * 0.5, 111, 112, 0.9, 0.75), 26, 1, "1000", { 255, 215, 0 }, 6, true);
	TextureManager::GetInstance()->LoadTTF("BB_menuAI", "Montant Big Blind:", { 255, 215, 0 }, 35, 1);
	m_InputBoxBB = new InputBox(Properties("inputbox_menu", SCREEN_WIDTH * 0.6 - (111 *0.75) * 0.5, SCREEN_HEIGHT * 0.4 - (112 * 0.75) * 0.5, 111, 112, 0.75, 0.75), 26, 1, "50", {255, 215, 0}, 4, true);
	TextureManager::GetInstance()->LoadTTF("nbrAI_menuAI", "Nombre d'I.A:", { 255, 215, 0 }, 35, 1);
	m_InputBoxNbrAI = new InputBox(Properties("inputbox_menu", SCREEN_WIDTH * 0.6 - (111*0.75) * 0.5, SCREEN_HEIGHT * 0.5 - (112*0.75) * 0.5, 111, 112, 0.75, 0.75), 26, 1, std::to_string(MAXPLAYER-1), {255, 215, 0}, 1, true);
	m_ButtonMenuAILaunch = new Button(Properties("txtframe_menu", SCREEN_WIDTH * 0.6 - 236 * 0.5, SCREEN_HEIGHT * 0.8 - 93 * 0.5, 236, 93, 0.9, 0.8), []() {
		int startGold = atoi(Game::GetInstance()->m_InputBoxStartGold->GetText().c_str());
		int bigBlind = atoi(Game::GetInstance()->m_InputBoxBB->GetText().c_str());
		if (startGold%25 == 0 && bigBlind%50 == 0 && bigBlind < startGold) {
			Game::GetInstance()->m_BB = bigBlind;
			Game::GetInstance()->Init(Game::GetInstance()->GetGameStyle(), startGold); Game::GetInstance()->SetGameState(STATE_GAME);
		}
		else {
			if (startGold % 25 != 0) {
				startGold = startGold - startGold % 25;
				Game::GetInstance()->m_InputBoxStartGold->SetText(std::to_string(startGold));
			}
			if (bigBlind % 50 != 0) {
				bigBlind = bigBlind - bigBlind % 50;
				Game::GetInstance()->m_InputBoxBB->SetText(std::to_string(bigBlind));
			}
			if (bigBlind > startGold) {
				Game::GetInstance()->m_InputBoxBB->SetText("50");
			}
		}
	}, 26, 1, "Lancer la partie", { 255, 215, 0 });
	m_ButtonMenuAIMenu = new Button(Properties("txtframe_menu", SCREEN_WIDTH * 0.4 - 236 * 0.5, SCREEN_HEIGHT * 0.8 - 93 * 0.5, 236, 93, 0.9, 0.8), []() {Game::GetInstance()->SetGameState(STATE_MENU);}, 26, 1, "Retour menu", { 255, 215, 0 });
	//=== Pause ===/
	m_ButtonResumeGame = new Button(Properties("txtframe_menu", (SCREEN_WIDTH - 236) * 0.5, SCREEN_HEIGHT * 0.3 - 93 * 0.5, 236, 93, 0.9, 0.8), []() {Game::GetInstance()->SetGameState(STATE_GAME);}, 22, 1, "Reprendre la partie", { 255, 215, 0 });
	m_ButtonQuitGame = new Button(Properties("txtframe_menu", (SCREEN_WIDTH - 236) * 0.5, SCREEN_HEIGHT * 0.4 - 93 * 0.5, 236, 93, 0.9, 0.8), []() {Game::GetInstance()->ResetGame(); Game::GetInstance()->SetGameState(STATE_MENU);}, 24, 1, "Quitter la partie", { 255, 215, 0 });
	TextureManager::GetInstance()->LoadTTF("title_pause", "Pause", { 255, 215, 0 }, 50, 1);
	m_GameState = STATE_MENU;
	for (int i = 0; i < MAXPLAYER; i++) {
		m_Player[i] = nullptr;
	}
	for (int i = 0; i < NBRBOARD; i++) {
		m_Board[i] = nullptr;
	}
	m_BoardFrame = nullptr;
	m_PotGold = 0; m_NbrPlayer = MAXPLAYER;
	m_PlayerTurn = false;
	m_GameStyle = "basic";
	m_lastID = 0;
};

bool Game::ResetGame() {
	m_StopThread = true;
	m_GameLogic.join();
	m_PlayerTurn = false;
	for (int i = 1; i <= NBRTYPE; i++) {
		for (int y = 1; y <= NBRCARDTYPE; y++) {
			TextureManager::GetInstance()->Drop(std::to_string(y) + "_" + std::to_string(i));
		}
	}
	TextureManager::GetInstance()->Drop("token_25");
	TextureManager::GetInstance()->Drop("token_100");
	TextureManager::GetInstance()->Drop("token_500");
	TextureManager::GetInstance()->Drop("token_1000");
	TextureManager::GetInstance()->Drop("token_5000");
	TextureManager::GetInstance()->Drop("mise");
	TextureManager::GetInstance()->Drop("showdowntxt");
	for (int i = 0; i < MAXPLAYER; i++) {
		if (m_Player[i] != nullptr) {
			m_Player[i]->Surrender(false);
			m_Player[i]->Fold(false);
			delete(m_Player[i]);
			m_Player[i] = nullptr;
		}
	}
	for (int i = 0; i < NBRBOARD; i++) {
		if (m_Board[i] != nullptr) {
			delete(m_Board[i]);
			m_Board[i] = nullptr;
		}
	}
	delete(m_BoardFrame); m_BoardFrame = nullptr;
	delete(m_ButtonMenu); m_ButtonMenu = nullptr;
	delete(m_InputBoxBet); m_InputBoxBet = nullptr;
	delete(m_ButtonBet); m_ButtonBet = nullptr;
	delete(m_ButtonCall); m_ButtonCall = nullptr;
	delete(m_ButtonCheck); m_ButtonCheck = nullptr;
	delete(m_ButtonFold); m_ButtonFold = nullptr;
	delete(m_ButtonShowdown); m_ButtonShowdown = nullptr;
	m_ButtonOptionsMenu->ChangeFunction([]() {Game::GetInstance()->SetGameState(STATE_MENU); });
	m_ButtonAchievementsMenu->ChangeFunction([]() {Game::GetInstance()->SetGameState(STATE_MENU); });
	m_PotGold = 0; m_LastBet = 0; m_Start_pos = 1;
	return true;
}

bool Game::Init(std::string style, int startGold) {
	//=== Chargement des 52 cartes du jeu dans la mémoire ===//
	for (int i = 1; i <= NBRTYPE; i++) {
		for (int y = 1; y <= NBRCARDTYPE; y++) {
			TextureManager::GetInstance()->Load(std::to_string(y) + "_" + std::to_string(i), "assets/cards/" + std::to_string(y) + "_" + std::to_string(i) + ".png");
		}
	}
	//=== Chargement des jetons de casino ===//
	TextureManager::GetInstance()->Load("token_25", "assets/tokens/token_25.png");
	TextureManager::GetInstance()->Load("token_100", "assets/tokens/token_100.png");
	TextureManager::GetInstance()->Load("token_500", "assets/tokens/token_500.png");
	TextureManager::GetInstance()->Load("token_1000", "assets/tokens/token_1000.png");
	TextureManager::GetInstance()->Load("token_5000", "assets/tokens/token_5000.png");
	//=== Chargement des joueurs ===//
	for (int i = 0; i < m_NbrPlayer; i++) {
		m_Player[i] = new Player("Joueur"+std::to_string(i+1), i + 1, startGold);
	}
	//=== Chargement du board ===//
	for (int i = 0; i < NBRBOARD; i++) {
		m_Board[i] = new Card(Properties("", SCREEN_WIDTH * 0.235 + 100 * (i + 1), SCREEN_HEIGHT * 0.44, 80, 117, 1.0, 1.0), i + 9, (i % NBRTYPE) + 1, true);
	}
	m_BoardFrame = new TextFrame(Properties("txtframe_2", 500, 360, 235, 92, 1.2, 0.6), 28, 0, "Pot: ", { 128, 128, 128 }, std::to_string(m_PotGold) + " Or", { 255, 215, 0 });
	m_ButtonMenu = new Button(Properties("menu", SCREEN_WIDTH*0.95 - 48*0.9, SCREEN_HEIGHT * 0.05 - 37 * 0.5, 48, 37), []() {Game::GetInstance()->SetGameState(STATE_PAUSE);});
	//=== Chargement de l'interface ===//
	m_ButtonShowdown = new Button(Properties("txtframe_2", SCREEN_WIDTH * 0.5 - 235 * 0.6 * 0.5, SCREEN_HEIGHT * 0.645 - 92 * 0.4 * 0.5, 235, 92, 0.6, 0.4), []() {
		Game::GetInstance()->m_ButtonShowdown->SetActive(false);
		Game::GetInstance()->m_PotGold = 0;
		Game::GetInstance()->m_BoardFrame->SetText(28, 0, "Pot: ", { 128, 128, 128 }, std::to_string(Game::GetInstance()->m_PotGold) + " Or", { 255, 215, 0 });
		Game::GetInstance()->m_LastBet = 0;
		Game::GetInstance()->m_Start_pos = (Game::GetInstance()->m_Start_pos - 1);
		if (Game::GetInstance()->m_Start_pos < 0) Game::GetInstance()->m_Start_pos = Game::GetInstance()->m_NbrPlayer - 1;
		for (int i = 0; i < 5; i++) {
			Game::GetInstance()->m_Board[i]->SetMasked(true);
		}
		int nbrSurrender = 0;
		for (int i = 0; i < Game::GetInstance()->m_NbrPlayer; i++) {
			Game::GetInstance()->m_Player[i]->SetBetMoney(0);
			Game::GetInstance()->m_Player[i]->SetBetted(false);
			if (Game::GetInstance()->m_Player[i]->GetMoney() < Game::GetInstance()->m_BB && !Game::GetInstance()->m_Player[i]->GetSurrender()) {
				Game::GetInstance()->m_Player[i]->Surrender(true);
				nbrSurrender++;
			}
			else if (!Game::GetInstance()->m_Player[i]->GetSurrender()) {
				Game::GetInstance()->m_Player[i]->Fold(false);
				Game::GetInstance()->m_Player[i]->MaskCards(true);
			}
			else nbrSurrender++;
		}
		if (Game::GetInstance()->m_Player[0]->GetSurrender()) { //Vous avez perdu
			Game::GetInstance()->ResetGame(); Game::GetInstance()->SetGameState(STATE_MENU);
		}
		else if (nbrSurrender >= Game::GetInstance()->m_NbrPlayer - 1) { //Victoire !
			if (!Game::GetInstance()->m_Winner->GetValidated()) {
				Game::GetInstance()->m_Winner->SetValidated(true);
				Game::GetInstance()->m_AchievementTab->Refresh();
				Game::GetInstance()->m_AchievementAnimation->PlayAchievement(Game::GetInstance()->m_Winner);
			}
			if (nbrSurrender == 7 && !Game::GetInstance()->m_Victorious->GetValidated()) {
				Game::GetInstance()->m_Victorious->SetValidated(true);
				Game::GetInstance()->m_AchievementTab->Refresh();
				Game::GetInstance()->m_AchievementAnimation->PlayAchievement(Game::GetInstance()->m_Victorious);
			}
			Game::GetInstance()->ResetGame(); Game::GetInstance()->SetGameState(STATE_MENU);
		}
		else {
			Game::GetInstance()->m_GameLogic.join();
			Game::GetInstance()->m_StopThread = false;
			std::thread Game_Logic([]() {Game::GetInstance()->StartRound(Game::GetInstance()->m_Start_pos); });
			Game_Logic.swap(Game::GetInstance()->m_GameLogic);
		}
	}, 24, 0, "Valider", { 255, 215, 0 }, false, false);
	m_GameTurnFrame = new TextFrame(Properties("txtframe_2", 10, 10, 235, 92, 1.2, 0.6), 28, 0, "Chargement...", { 128, 128, 128 });
	m_InputBoxBet = new InputBox(Properties("inputbox_2", SCREEN_WIDTH - (236 * 1.6) + 70 + 20, SCREEN_HEIGHT - (93 * 1.5) + 23, 280, 40, 0.59, 0.8), 22, 0, "0", {255, 215, 0}, 10, true);
	m_ButtonBet = new Button(Properties("txtframe_2", SCREEN_WIDTH - (236 * 1.6) + 240 + 20, SCREEN_HEIGHT - (93 * 1.5) + 20, 236, 93, 0.45, 0.45), []() {
		int bet = atoi(Game::GetInstance()->m_InputBoxBet->GetText().c_str());
		int playerGold = Game::GetInstance()->m_Player[0]->GetMoney();
		int lastBet = Game::GetInstance()->m_LastBet;
		bool tmpOK = true;
		if (bet % 25 != 0) {
			bet = bet - bet % 25;
			Game::GetInstance()->m_InputBoxBet->SetText(std::to_string(bet));
			tmpOK = false;
		}
		if (bet > playerGold) {
			bet = playerGold;
			Game::GetInstance()->m_InputBoxBet->SetText(std::to_string(playerGold));
			tmpOK = false;
		}
		if (bet < lastBet && playerGold >= lastBet) {
			bet = lastBet;
			Game::GetInstance()->m_InputBoxBet->SetText(std::to_string(lastBet));
			tmpOK = false;
		}
		else if (bet < lastBet && bet != playerGold) {
			bet = playerGold;
			Game::GetInstance()->m_InputBoxBet->SetText(std::to_string(playerGold));
			tmpOK = false;
		}
		if (tmpOK && Game::GetInstance()->m_PlayerTurn) {
			Game::GetInstance()->Bet(0, bet);
			Game::GetInstance()->m_PlayerTurn = false;
		}
	}, 22, 0, "Miser");
	m_ButtonFold = new Button(Properties("txtframe_2", SCREEN_WIDTH - (236 * 1.6) + 15, SCREEN_HEIGHT - (93 * 1.5) + 80, 236, 93, 0.55, 0.5), []() {
		if (Game::GetInstance()->m_PlayerTurn) {
			Game::GetInstance()->Fold(0);
			Game::GetInstance()->m_PlayerTurn = false;
		}
	}, 22, 0, "Coucher");
	m_ButtonCall = new Button(Properties("txtframe_2", SCREEN_WIDTH - (236 * 1.6) + (236 * 0.55) + 20, SCREEN_HEIGHT - (93 * 1.5) + 80, 236, 93, 0.45, 0.5), []() {
		int playerGold = Game::GetInstance()->m_Player[0]->GetMoney();
		int lastBet = Game::GetInstance()->m_LastBet;
		int bet = lastBet; if (bet > playerGold) bet = playerGold;
		if(atoi(Game::GetInstance()->m_InputBoxBet->GetText().c_str()) != bet) Game::GetInstance()->m_InputBoxBet->SetText(std::to_string(bet));
		else if (Game::GetInstance()->m_PlayerTurn) {
			Game::GetInstance()->Bet(0, bet);
			Game::GetInstance()->m_PlayerTurn = false;
		}
	}, 22, 0, "Suivre");
	m_ButtonCheck = new Button(Properties("txtframe_2", SCREEN_WIDTH - (236 * 1.6) + (236 * 0.55) + (236 * 0.45) + 25, SCREEN_HEIGHT - (93 * 1.5) + 80, 236, 93, 0.45, 0.5), []() {}, 22, 0, "Passer");
	m_ButtonOptionsMenu->ChangeFunction([]() {Game::GetInstance()->SetGameState(STATE_PAUSE); });
	m_ButtonAchievementsMenu->ChangeFunction([]() {Game::GetInstance()->SetGameState(STATE_PAUSE); });
	int inc = 0; int tmp[MAXPLAYER] = { 0, 7, 3, 6, 2, 5, 1, 4 };
	for (int i = 0; i < MAXPLAYER; i++) {
		if (tmp[i] < m_NbrPlayer) { m_PlayerOrder[inc] = tmp[i]; inc++; }
	}
	m_GameWonText = new Text(Properties("", SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.58, 1, 1), 26, 1, "", {255, 215, 0});
	SetGameStyle(style);
	m_Start_pos = 1; m_LastBet = 0;
	//Thread operations
	Game::GetInstance()->m_StopThread = false;
	std::thread Game_Logic([]() {Game::GetInstance()->StartRound(Game::GetInstance()->m_Start_pos); });
	Game_Logic.swap(Game::GetInstance()->m_GameLogic);
	return true;
}

bool Game::Clean() {
	for (int i = 0; i < MAXPLAYER; i++) {
		if (m_Player[i] != nullptr) delete(m_Player[i]);
	}
	for (int i = 0; i < NBRBOARD; i++) {
		if (m_Board[i] != nullptr) delete(m_Board[i]);
	}
	if (m_BoardFrame != nullptr) delete(m_BoardFrame);
	return true;
}

void Game::Quit() {
	
}

void Game::SetGameStyle(std::string style) {
	m_GameStyle = style;
	if (style == "basic") {
		m_ButtonSetBasicStyle->SetKeepColor(true);
		m_ButtonSetCosmicStyle->SetKeepColor(false);
	}
	else {
		m_ButtonSetBasicStyle->SetKeepColor(false);
		m_ButtonSetCosmicStyle->SetKeepColor(true);
	}
	//=== Drop anciennes textures ===//
	TextureManager::GetInstance()->Drop("0_0");
	TextureManager::GetInstance()->Drop("background");
	TextureManager::GetInstance()->Drop("menu");
	TextureManager::GetInstance()->Drop("txtframe_1");
	TextureManager::GetInstance()->Drop("txtframe_2");
	TextureManager::GetInstance()->Drop("inputbox_2");
	TextureManager::GetInstance()->Drop("mise");
	//=== Chargement des textures ===//
	TextureManager::GetInstance()->Load("0_0", "assets/cards/" + m_GameStyle + ".png"); //Dos de carte
	TextureManager::GetInstance()->Load("background", "assets/background/" + m_GameStyle + ".png"); //Arrière plan
	TextureManager::GetInstance()->Load("menu", "assets/UI/" + m_GameStyle + "_menu.png"); //Bouton menu
	TextureManager::GetInstance()->Load("inputbox_2", "assets/txtframes/"+m_GameStyle+"_input2.png");
	//=== Chargement des cadres de texte ===//
	TextureManager::GetInstance()->Load("txtframe_1", "assets/txtframes/" + m_GameStyle + "_1.png");
	if(style != "basic") TextureManager::GetInstance()->Load("txtframe_2", "assets/txtframes/" + m_GameStyle + "_2.png");
	else TextureManager::GetInstance()->Load("txtframe_2", "assets/txtframes/" + m_GameStyle + "_1.png");
	if (style == "cosmic") TextureManager::GetInstance()->LoadTTF("mise", "Mise:", { 8, 251, 245 }, 28, 0);
	else TextureManager::GetInstance()->LoadTTF("mise", "Mise:", { 255, 215, 0 }, 30, 0);
	for (int i = 0; i < MAXPLAYER; i++) {
		if(m_Player[i] != nullptr) m_Player[i]->RefreshFrame();
	}
	if(m_InputBoxBet != nullptr) m_InputBoxBet->Refresh();
	if (m_ButtonBet != nullptr) m_ButtonBet->Refresh();
	if (m_ButtonCall != nullptr) m_ButtonCall->Refresh();
	if (m_ButtonCheck != nullptr) m_ButtonCheck->Refresh();
	if (m_ButtonFold != nullptr) m_ButtonFold->Refresh();
	if (m_GameTurnFrame != nullptr) m_GameTurnFrame->Refresh();
	if (m_ButtonShowdown != nullptr) m_ButtonShowdown->Refresh();
	if(m_BoardFrame != nullptr) m_BoardFrame->SetText(28, 0, "Pot: ", { 128, 128, 128 }, std::to_string(m_PotGold) + " Or", { 255, 215, 0 });
}

void Game::Events(SDL_Event* ev) {
	if (m_GameState == STATE_GAME) {
		m_ButtonMenu->Events(ev);
		m_ButtonBet->Events(ev);
		m_InputBoxBet->Events(ev);
		m_ButtonFold->Events(ev);
		m_ButtonCall->Events(ev);
		m_ButtonCheck->Events(ev);
		m_ButtonShowdown->Events(ev);
	}
	else if (m_GameState == STATE_MENU) {
		m_ButtonAIGame->Events(ev);
		m_ButtonJoinGame->Events(ev);
		m_ButtonHostGame->Events(ev);
		m_ButtonAchievements->Events(ev);
		m_ButtonOptions->Events(ev);
		m_ButtonExit->Events(ev);
	}
	else if (m_GameState == STATE_PAUSE) {
		m_ButtonResumeGame->Events(ev);
		m_ButtonQuitGame->Events(ev);
		m_ButtonAchievements->Events(ev);
		m_ButtonOptions->Events(ev);
		m_ButtonExit->Events(ev);
	}
	else if (m_GameState == STATE_OPTIONS) {
		m_ButtonOptionsMenu->Events(ev);
		m_ButtonSetBasicStyle->Events(ev);
		m_ButtonSetCosmicStyle->Events(ev);
		m_VolumeOption->Events(ev);
		m_VolumeInput->Events(ev);
		if (ev->type == SDL_KEYDOWN) {
			int tmp = atoi(m_VolumeInput->GetText().c_str());
			if (tmp > 100) tmp = 100;
			else if (tmp < 0) tmp = 0;
			Engine::GetInstance()->SetSoundVolume(tmp);
			m_VolumeOption->SetLoad(tmp);
		}
		else {
			Engine::GetInstance()->SetSoundVolume(m_VolumeOption->GetLoad());
			m_VolumeInput->SetText(std::to_string(m_VolumeOption->GetLoad()));
		}
	}
	else if (m_GameState == STATE_MENU_IA) {
		m_InputBoxNbrAI->Events(ev);
		m_ButtonMenuAILaunch->Events(ev);
		m_ButtonMenuAIMenu->Events(ev);
		m_InputBoxBB->Events(ev);
		m_InputBoxStartGold->Events(ev);
		if (ev->type == SDL_KEYDOWN) {
			int tmp = atoi(m_InputBoxNbrAI->GetText().c_str());
			if (tmp >= 1) {
				if (tmp+1 > MAXPLAYER) tmp = 7;
				m_NbrPlayer = tmp + 1;
				m_InputBoxNbrAI->SetText(std::to_string(tmp));
			}
		}
	}
	else if (m_GameState == STATE_ACHIEVEMENTS) {
		m_AchievementTab->Events(ev);
		m_ButtonAchievementsMenu->Events(ev);
	}
}

void Game::Update(float dt) {
	m_AchievementAnimation->Update(dt);
	for (int i = 0; i < m_TextVector.size(); i++) {
		m_TextVector[i]->Update(dt);
		if (SDL_GetTicks64() - m_TextVector[i]->GetTimer() > 1000) {
			delete (m_TextVector[i]);
			m_TextVector.erase(m_TextVector.begin()+i);
		}
	}
}

void Game::Render() {
	SDL_SetRenderDrawColor(Engine::GetInstance()->GetRenderer(), 53, 101, 77, 255);
	if (m_GameState == STATE_GAME) {
		//Render background
		TextureManager::GetInstance()->Draw("background", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		//Render player: text, tokens and cards
		for (int i = 0; i < MAXPLAYER; i++) {
			if (m_Player[i] != nullptr) m_Player[i]->Draw();
		}
		//Render board cards
		for (int i = 0; i < NBRBOARD; i++) {
			if (m_Board[i] != nullptr) m_Board[i]->Draw();
		}
		//Render board frame
		m_BoardFrame->Draw();
		m_ButtonMenu->Draw();
		TextureManager::GetInstance()->Draw("txtframe_2", SCREEN_WIDTH - (236 * 1.6), SCREEN_HEIGHT - (93 * 1.5), 236, 93, 1.6, 1.5);
		if(m_GameStyle == "cosmic") TextureManager::GetInstance()->DrawTTF("mise", SCREEN_WIDTH - (236 * 1.6) + 12, SCREEN_HEIGHT - (93 * 1.5) + 25);
		else TextureManager::GetInstance()->DrawTTF("mise", SCREEN_WIDTH - (236 * 1.6) + 20, SCREEN_HEIGHT - (93 * 1.5) + 20);
		m_InputBoxBet->Draw();
		m_ButtonBet->Draw();
		m_ButtonFold->Draw();
		m_ButtonCall->Draw();
		m_ButtonCheck->Draw();
		m_GameTurnFrame->Draw();
		if (m_ButtonShowdown->GetActive() == true) {
			TextureManager::GetInstance()->Draw("txtframe_2", SCREEN_WIDTH * 0.5 - (236*0.5*1.6), SCREEN_HEIGHT * 0.62 - (93*0.5*1.1), 236, 93, 1.6, 1.1);
			m_GameWonText->Draw();
		}
		m_ButtonShowdown->Draw();
		for (int i = 0; i < m_TextVector.size(); i++) {
			m_TextVector[i]->Draw();
		}
	}
	else if (m_GameState == STATE_MENU) {
		TextureManager::GetInstance()->DrawTTF("title_menu", SCREEN_WIDTH * 0.5 - 200, SCREEN_HEIGHT * 0.1);
		m_ButtonAIGame->Draw();
		m_ButtonJoinGame->Draw();
		m_ButtonHostGame->Draw();
		m_ButtonAchievements->Draw();
		m_ButtonOptions->Draw();
		m_ButtonExit->Draw();
	}
	else if (m_GameState == STATE_PAUSE) {
		TextureManager::GetInstance()->DrawTTF("title_pause", SCREEN_WIDTH * 0.5 - 70, SCREEN_HEIGHT * 0.1);
		m_ButtonResumeGame->Draw();
		m_ButtonQuitGame->Draw();
		m_ButtonAchievements->Draw();
		m_ButtonOptions->Draw();
		m_ButtonExit->Draw();
	}
	else if (m_GameState == STATE_OPTIONS) {
		TextureManager::GetInstance()->DrawTTF("title_options", SCREEN_WIDTH * 0.5 - 90, SCREEN_HEIGHT * 0.1);
		TextureManager::GetInstance()->DrawTTF("option_style", SCREEN_WIDTH * 0.5 - 250, SCREEN_HEIGHT * 0.5-20);
		m_ButtonSetBasicStyle->Draw();
		m_ButtonSetCosmicStyle->Draw();
		m_ButtonOptionsMenu->Draw();
		TextureManager::GetInstance()->DrawTTF("option_volume_title", SCREEN_WIDTH * 0.5 - 85, SCREEN_HEIGHT * 0.25);
		m_VolumeOption->Draw();
		m_VolumeInput->Draw();
		TextureManager::GetInstance()->Draw("option_volume_line", SCREEN_WIDTH * 0.45 - (439*0.6*0.5), SCREEN_HEIGHT * 0.35 - (49 * 0.5), 439, 49, 0.6);
	}
	else if (m_GameState == STATE_MENU_IA) {
		TextureManager::GetInstance()->DrawTTF("title_menuAI", SCREEN_WIDTH * 0.5 - 190, SCREEN_HEIGHT * 0.1);
		TextureManager::GetInstance()->DrawTTF("StartGold_menuAI", SCREEN_WIDTH * 0.35 - 20, SCREEN_HEIGHT * 0.3 - 25);
		m_InputBoxStartGold->Draw();
		TextureManager::GetInstance()->DrawTTF("BB_menuAI", SCREEN_WIDTH * 0.35 - 20, SCREEN_HEIGHT * 0.4 - 25);
		m_InputBoxBB->Draw();
		TextureManager::GetInstance()->DrawTTF("nbrAI_menuAI", SCREEN_WIDTH * 0.35 + 60, SCREEN_HEIGHT * 0.5 - 25);
		m_InputBoxNbrAI->Draw();
		m_ButtonMenuAILaunch->Draw();
		m_ButtonMenuAIMenu->Draw();
	}
	else if (m_GameState == STATE_ACHIEVEMENTS) {
		TextureManager::GetInstance()->DrawTTF("title_achievements", SCREEN_WIDTH * 0.5 - 110, SCREEN_HEIGHT * 0.1);
		m_AchievementTab->Draw();
		m_ButtonAchievementsMenu->Draw();
	}
	m_AchievementAnimation->Draw();
}