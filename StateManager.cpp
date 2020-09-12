#include "StateManager.h"

StateManager::StateManager(SharedContext* l_shared) :
	m_shared(l_shared)
{
	RegisterState<State_Intro>(StateType::INTRO);
	RegisterState<State_MainMenu>(StateType::MAINMENU);
	RegisterState<State_Game>(StateType::GAME);
	RegisterState<State_Paused>(StateType::PAUSED);
}

StateManager::~StateManager()
{
	for (auto& itr : m_states)
	{
		itr.second->OnDestroy();
		delete itr.second;
	}
}

void StateManager::Draw()
{
	if (m_states.empty()) { return; }

	//.back() is a reference to the last element of the vector
	if (m_states.back().second->IsTransparent()
		&& m_states.size() > 1)
	{
		auto itr = m_states.end();
		//generally, we search for the latest itr that's not transparent
		//begin with him, we draw other itrs
		while (itr != m_states.begin())
		{
			if (itr != m_states.end())
			{
				if (!itr->second->IsTransparent())
					break;
			}	
			--itr;
		}

		for (; itr != m_states.end(); ++itr)
		{
			m_shared->m_wind->GetRenderWindow()->
				SetView(itr->second->GetView());
			itr->second->Draw();
		}
	}
	else
		m_states.back().second->Draw();
}

void StateManager::Update(const sf::Time& l_time)
{
	if (m_states.empty()) { return; }
	if (m_states.back().second->isTranscendent()
		&& m_states.size() > 1)
	{
		auto itr = m_states.end();
		while (itr != m_states.begin())
		{
			if (itr != m_states.end())
			{
				if (!itr->second->isTranscendent())
					break;
			}
			--itr;
		}

		for (; itr != m_states.end(); ++itr)
			itr->second->Update(l_time);
	}
	else
		m_states.back().second->Update(l_time);
}

SharedContext* StateManager::GetContext() { return m_shared; }

bool StateManager::HasState(const StateType& l_type)
{
	for (auto itr = m_states.begin();
		itr != m_states.end(); ++itr)
	{
		if (itr->first == l_type)
		{
			auto removed = std::find(m_toRemove.begin(),
				m_toRemove.end(), l_type);
			if (removed == m_toRemove.end()) { return true; }
			return false;
		}
	}
	return false;
}

void StateManager::Remove(const StateType& l_type)
{
	m_toRemove.push_back(l_type);
}

void StateManager::ProcessRequests()
{
	while (m_toRemove.begin() != m_toRemove.end())
	{
		RemoveState(*m_toRemove.begin());
		m_toRemove.erase(m_toRemove.begin());
	}
}

void StateManager::SwitchTo(const StateType& l_type)
{
	//scs modifies an internal data members of the em 
	//and keeps track of which state the game is in
	m_shared->m_eventManager->SetCurrentState(l_type);
	for (auto itr = m_states.begin();
		itr != m_states.end(); ++itr)
		if (itr->first == l_type)
		{
			m_states.back().second->Deactivate();
			StateType tmp_type = itr->first;
			BaseState* tmp_state = itr->second;
			m_states.erase(itr);
			m_states.emplace_back(tmp_type, tmp_state);
			tmp_state->Activate();
			m_shared->m_wind->GetRenderWindow()->
				SetView(tmp_state->GetView());
			return;
		}

	//if a state with l_type wasn't found
	if (!m_states.empty()) { m_states.back().second->Deactivate(); }
	CreateState(l_type);
	m_states.back().second->Activate();
	m_shared->m_wind->GetRenderWindow()->SetView(
		m_states.back().second->GetView());
}

void StateManager::CreateState(const StateType& l_type)
{
	auto newState = m_stateFactory.find(l_type);
	if (newState == m_stateFactory.end()) { return; }
	BaseState* state = newState->second();
	state->m_view = m_shared->m_wind->
		GetRenderWindow()->getDefaultView();
	m_states.emplace_back(l_type, state);
	state->OnCreate();
}

void StateManager::RemoveState(const StateType& l_type)
{
	for (auto itr = m_states.begin();
		itr != m_states.end(); ++itr)
		if (itr->first == l_type)
		{
			itr->second->OnDestroy();
			delete itr->second;
			m_states.erase(itr);
			return;
		}
}