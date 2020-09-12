#pragma once
#include <SFML/Graphics.hpp>
class StateManager;

class BaseState
{
	friend class StateManager;
public:
	BaseState(StateManager* l_stateManager) :
		m_stateMgr(l_stateManager), 
		m_transparent(false),
		m_transcendent(false) {}

	virtual ~BaseState() {}

	//the state is created and pushed on the stack:
	virtual void OnCreate() = 0;
	//the state is removed from the stack:
	virtual void OnDestroy() = 0;

	//the state is moved to the top of the stack:
	virtual void Activate() = 0;
	//the state is removed from the top of the stack
	virtual void Deactivate() = 0;

	virtual void Update(const sf::Time& l_time) = 0;
	virtual void Draw() = 0;

	void SetTransparent(const bool& l_transparent)
	{
		m_transparent = l_transparent;
	}
	bool IsTransparent()const { return m_transparent; }
	void SetTranscedent(const bool& l_transcendence)
	{
		m_transcendent = l_transcendence;
	}
	bool isTranscendent()const { return m_transcendent; }
	StateManager* GetStateManager() { return m_stateMgr; }

	sf::View& GetView() { return m_view; }

protected:
	StateManager* m_stateMgr;
	bool m_transparent; //прозрачность?? draw
	bool m_transcendent; //update

	sf::View m_view;
};