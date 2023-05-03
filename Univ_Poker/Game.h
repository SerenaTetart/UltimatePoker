#ifndef GAME_H
#define GAME_H

#include <string>
#include <thread>
#include <vector>
#include "Engine.h"

#include "Card.h"
#include "Player.h"
#include "Button.h"
#include "InputBox.h"
#include "Text.h"

#define NBRTYPE 4
#define NBRCARDTYPE 13
#define MAXPLAYER 8
#define NBRBOARD 5

enum GameStates {
	STATE_MENU,
	STATE_GAME,
	STATE_OPTIONS,
	STATE_MENU_IA,
	STATE_MENU_PLAYER,
	STATE_PAUSE
};

class Game {

public:
	static Game* GetInstance() {
		return s_Instance = (s_Instance != nullptr) ? s_Instance : new Game();
	}

	bool Init(std::string style = "basic", int startGold = 1000);
	bool Clean();
	void Quit();

	void Update(float dt);
	void Render();
	void Events(SDL_Event *ev);

	bool ResetGame();
	inline bool GetGameState() { return m_GameState; }
	inline void SetGameState(GameStates state) { m_GameState = state; }
	void SetGameStyle(std::string style);
	inline std::string GetGameStyle() { return m_GameStyle; }

	//Game logic
	void Bet(int playerNbr, int bet, bool ok=false);
	void Fold(int playerNbr);
	void AdjustBet(int lowestBet);
	void StartRound(int start_pos);
	void BetPhase(int start_pos);
	void GatherPot();
	void Showdown();
private:
	Game();

	GameStates m_GameState;
	std::string m_GameStyle;

	//I.G
	Player* m_Player[MAXPLAYER];
	Card* m_Board[NBRBOARD];
	TextFrame* m_BoardFrame; TextFrame* m_GameTurnFrame; Text* m_GameWonText;
	int m_PotGold, m_NbrPlayer, m_LastBet, m_Start_pos, m_BB, m_lastID;
	int m_PlayerOrder[MAXPLAYER]; bool m_PlayerTurn, m_StopThread;
	InputBox* m_InputBoxBet;
	Button* m_ButtonMenu; Button* m_ButtonBet; Button* m_ButtonFold; Button* m_ButtonCall; Button* m_ButtonCheck; Button* m_ButtonShowdown;
	std::thread m_GameLogic;
	std::vector<Text*> m_TextVector;
	//Title screen/Menu
	Button* m_ButtonAIGame; Button* m_ButtonJoinGame; Button* m_ButtonHostGame; Button* m_ButtonAchievements; Button* m_ButtonExit; Button* m_ButtonOptions;
	//Options
	Button* m_ButtonOptionsMenu; Button* m_ButtonSetBasicStyle; Button* m_ButtonSetCosmicStyle;
	//Menu I.A & Menu PvP
	Button* m_ButtonMenuAIMenu; Button* m_ButtonMenuAILaunch;
	InputBox* m_InputBoxNbrAI; InputBox* m_InputBoxBB; InputBox* m_InputBoxStartGold;
	//Pause
	Button* m_ButtonResumeGame; Button* m_ButtonQuitGame;

	static Game* s_Instance;
};

#endif