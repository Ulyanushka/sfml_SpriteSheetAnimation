#pragma once
#include <SFML/Graphics.hpp>
#include "EventManager.h"
#include "State_Intro.h"
#include "State_MainMenu.h"
#include "State_Game.h"
#include "State_Paused.h"
#include <vector>
#include <unordered_map>
#include "SharedContext.h"

using StateContainer = std::vector<
	std::pair<StateType, BaseState* >>;
using TypeContainer = std::vector<StateType>;
using StateFactory = std::unordered_map<
	StateType, std::function<BaseState* (void)>>;

enum class StateType
{
	INTRO = 1, MAINMENU, GAME, PAUSED, GAMEOVER, CREDITS
};

class StateManager
{
public:
	StateManager(SharedContext* l_shared);
	~StateManager();

	void Update(const sf::Time& l_time);
	void Draw();

	//checks if the states from awaiting store are not in use
	//after checking - deleting happens
	void ProcessRequests();

	SharedContext* GetContext();
	bool HasState(const StateType& l_type);

	//changes the current state to l_type:
	void SwitchTo(const StateType& l_type);
	//removes a state from the state stack:
	void Remove(const StateType& l_type);

private:
	//methods
	void CreateState(const StateType& l_type);
	//takes care of actual resource de-allocation:
	void RemoveState(const StateType& l_type);

	template<class T>
	void RegisterState(const StateType& l_type)
	{
		m_stateFactory[l_type] = [this]()->BaseState*
		{
			return new T(this);
		};
	}

	//members
	SharedContext* m_shared;
	StateContainer m_states;
	//store for states we want to remove:
	//(they await for the proper time for dying)
	TypeContainer m_toRemove;
	StateFactory m_stateFactory;
};