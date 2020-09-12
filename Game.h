#pragma once
#include "Window.h"
#include "StateManager.h"
#include "EventManager.h"
#include "SharedContext.h"
#include <iostream>

class Game
{
public:
	Game();
	~Game();

	void Update();
	void Render();
	Window* GetWindow();
	sf::Time GetElapsed();
	void LateUpdate();

private:
	Window m_window;
	void RestartClock();
	SharedContext m_context;
	sf::Time m_elapsed;
	sf::Clock m_clock;
	StateManager m_stateManager;
};