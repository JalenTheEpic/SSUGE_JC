#include <stdafx.h>
#include <input_manager.h>

// A template-specialization of the msSingleton attribute for the LogManager class
template<>ssuge::InputManager * ssuge::Singleton<ssuge::InputManager>::msSingleton = nullptr;


ssuge::InputManager::InputManager()
{

}



ssuge::InputManager::~InputManager()
{

}



bool ssuge::InputManager::isActionActive(std::string action)
{
	std::map<std::string, int>::iterator it = mActiveActionCounts.find(action);
	if (it != mActiveActionCounts.end())
		return it->second > 0;
	else
		THROW_EXCEPTION("Invalid action '" + action + "'");
}



float ssuge::InputManager::getAxisValue(std::string axisName)
{
	std::map<std::string, float>::iterator it =  mAxisValues.find(axisName);
	if (it != mAxisValues.end())
		return it->second;
	else
		THROW_EXCEPTION("Invalid axisName '" + axisName + "'");
}


void ssuge::InputManager::update()
{
	SDL_Event evt;

	if (SDL_PollEvent(&evt))
	{
		switch (evt.type)
		{
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			if (evt.key.repeat == 0)
				handleKeyEvent(&evt);
			break;
		}
	}
}



void ssuge::InputManager::createAction(std::string name)
{
	mAvailableActions.insert(name);
	mActiveActionCounts[name] = 0;
}



void ssuge::InputManager::destroyAction(std::string name)
{
	std::set<std::string>::iterator it = mAvailableActions.find(name);
	if (it != mAvailableActions.end())
		mAvailableActions.erase(it);

	// Also clear out any bindings that used this name
	std::map<SDL_Keycode, std::string>::iterator it2 = mKeyActionBindings.begin();
	while (it2 != mKeyActionBindings.end())
	{
		if (it2->second == name)
			it2 = mKeyActionBindings.erase(it2);
		else
			++it2;
	}
}



void ssuge::InputManager::createAxis(std::string name)
{
	mAvailableAxes.insert(name);
	mAxisValues[name] = 0.0f;
}



void ssuge::InputManager::destroyAxis(std::string name)
{
	std::set<std::string>::iterator it = mAvailableAxes.find(name);
	if (it != mAvailableAxes.end())
		mAvailableAxes.erase(it);

	// Also clear out any bindings that used this name
	std::map<SDL_Keycode, std::pair<std::string, float>>::iterator it2 = mKeyAxisBindings.begin();
	while (it2 != mKeyAxisBindings.end())
	{
		if (it2->second.first == name)
			it2 = mKeyAxisBindings.erase(it2);
		else
			++it2;
	}
}



void ssuge::InputManager::addListener(GameObject * gobj)
{
	bool found = false;
	for (unsigned int i = 0; i < mListeners.size(); i++)
	{
		if (mListeners[i] == gobj)
		{
			found = true;
			break;
		}
	}

	if (!found)
		mListeners.push_back(gobj);
}

void ssuge::InputManager::removeListener(GameObject * gobj)
{
	for (unsigned int i = 0; i < mListeners.size(); i++)
	{
		if (mListeners[i] == gobj)
		{
			mListeners.erase(mListeners.begin() + i);
			break;
		}
	}
}


void ssuge::InputManager::addKeyActionBinding(SDL_Keycode k, std::string action)
{

	std::set<std::string>::iterator it = mAvailableActions.find(action);
	if (it == mAvailableActions.end())
		THROW_EXCEPTION("Invalid action '" + action + "'");

	mKeyActionBindings[k] = action;
}



void ssuge::InputManager::removeKeyActionBinding(SDL_Keycode k)
{
	std::map<SDL_Keycode, std::string>::iterator it = mKeyActionBindings.find(k);
	if (it != mKeyActionBindings.end())
		mKeyActionBindings.erase(it);
}


void ssuge::InputManager::addKeyAxisBinding(SDL_Keycode k, std::string axisName, float axisValue)
{

	std::set<std::string>::iterator it = mAvailableAxes.find(axisName);
	if (it == mAvailableAxes.end())
		THROW_EXCEPTION("Invalid axis '" + axisName + "'");

	mKeyAxisBindings[k] = std::make_pair(axisName, axisValue);
}



void ssuge::InputManager::removeKeyAxisBinding(SDL_Keycode k)
{
	std::map<SDL_Keycode, std::pair<std::string, float>>::iterator it = mKeyAxisBindings.find(k);
	if (it != mKeyAxisBindings.end())
		mKeyAxisBindings.erase(it);
}



void ssuge::InputManager::handleKeyEvent(SDL_Event * evt)
{
	std::map<SDL_Keycode, std::string>::iterator it = mKeyActionBindings.find(evt->key.keysym.sym);
	if (it != mKeyActionBindings.end())
	{
		std::map<std::string, int>::iterator sit = mActiveActionCounts.find(it->second);
		if (evt->type == SDL_KEYDOWN)	
			sit->second++;
		else
			sit->second--;

		broadcastAction(it->second, evt->type == SDL_KEYDOWN);
	}

	// Update the axis values, if appropriate
	std::map<SDL_Keycode, std::pair<std::string, float>>::iterator ax_it = mKeyAxisBindings.find(evt->key.keysym.sym);
	if (ax_it != mKeyAxisBindings.end())
	{
		if (evt->type == SDL_KEYDOWN)
			mAxisValues[ax_it->second.first] += ax_it->second.second;
		else
			mAxisValues[ax_it->second.first] -= ax_it->second.second;
	}
}



void ssuge::InputManager::broadcastAction(std::string action, bool starting)
{
	for (unsigned int i = 0; i < mListeners.size(); i++)
	{
		mListeners[i]->handleAction(action, starting);
		mListeners[i]->callScriptMethod("onAction");
	}
}