#include <stdafx.h>
#include <game_object_manager.h>
#include <game_object.h>

// A template-specialization of the msSingleton attribute for the GameObjectManager class
template<> ssuge::GameObjectManager * ssuge::Singleton<ssuge::GameObjectManager>::msSingleton = nullptr;

ssuge::GameObjectManager::GameObjectManager()
{

}


ssuge::GameObjectManager::~GameObjectManager()
{
	// Destroy the game objects and all groups
	std::map<std::string, std::vector<GameObject*>>::iterator it = mResourceGroups.begin();
	while (it != mResourceGroups.end())
	{
		for (unsigned int i = 0; i < it->second.size(); ++i)
		{
			if (it->second[i])
				delete it->second[i];
			it->second[i] = nullptr;
		}
		it->second.clear();
		++it;
	}
	mResourceGroups.clear();

	// Not necessary, but I like to be explicit...
	mGameObjectGroups.clear();
	mGroupNames.clear();
}



ssuge::GameObject * ssuge::GameObjectManager::findGameObject(std::string groupName, std::string gameObjectName)
{
	std::map<std::string, std::vector<GameObject*>>::iterator it = mResourceGroups.find(groupName);
	if (it == mResourceGroups.end())
		THROW_EXCEPTION("Invalid group name '" + groupName + "'");
	else
	{
		for (unsigned int i = 0; i < it->second.size(); ++i)
		{
			if (it->second[i]->getName() == gameObjectName)
				return it->second[i];
		}
	}

	// We didn't find the game object we were after.
	return nullptr;
}



ssuge::GameObject * ssuge::GameObjectManager::findGameObject(std::string gameObjectName)
{
	// Iterate through all groups
	std::map<std::string, std::vector<GameObject*>>::iterator it = mResourceGroups.begin();
	while (it != mResourceGroups.end())
	{
		for (unsigned int i = 0; i < it->second.size(); ++i)
		{
			if (it->second[i]->getName() == gameObjectName)
				return it->second[i];
		}
		++it;
	}

	// We didn't find the game object we were after
	return nullptr;
}



void ssuge::GameObjectManager::createGroupName(std::string groupName)
{
	std::set<std::string>::iterator it = mGroupNames.find(groupName);
	if (it == mGroupNames.end())
	{
		// Put it in the allowable group names.
		mGroupNames.insert(groupName);

		// Create the vector of game objects
		mResourceGroups[groupName] = std::vector<GameObject*>();
	}
}




void ssuge::GameObjectManager::addExisting(std::string groupName, GameObject* go)
{
	std::set<std::string>::iterator it = mGroupNames.find(groupName);
	if (it == mGroupNames.end())
		THROW_EXCEPTION("Invalid group name '" + groupName + "'");
	else
	{
		// Make sure we don't already have that game object in another group -- if so, 
		// move them here.
		std::map<GameObject*, std::string>::iterator it2 = mGameObjectGroups.find(go);
		if (it2 != mGameObjectGroups.end())
			removeGameObject(it2->second, go);

		// Add them to this group.
		mResourceGroups[groupName].push_back(go);

		// Add a reverse-lookup entry
		mGameObjectGroups[go] = groupName;
	}
}


void ssuge::GameObjectManager::removeGameObject(std::string groupName, GameObject* go)
{
	std::set<std::string>::iterator it = mGroupNames.find(groupName);
	if (it == mGroupNames.end())
		THROW_EXCEPTION("Invalid group name '" + groupName + "'");
	else
	{
		// Remove them from this resource group
		std::map<std::string, std::vector<GameObject*>>::iterator it2 = mResourceGroups.find(groupName);
		for (unsigned int i = 0; i < it2->second.size(); ++i)
		{
			if (it2->second[i] == go)
			{
				it2->second.erase(it2->second.begin() + i);
				break;
			}
		}

		// Remove their entry from the reverse lookup map.
		std::map<GameObject*, std::string>::iterator it3 = mGameObjectGroups.find(go);
		if (it3 != mGameObjectGroups.end())
			mGameObjectGroups.erase(it3);
	}
}


ssuge::GameObject* ssuge::GameObjectManager::createGameObject(std::string groupName, std::string objectName,
	unsigned int tag, GameObject * parent)
{
	GameObject * go = new GameObject(tag, objectName, parent);
	addExisting(groupName, go);
	
	return go;
}


void ssuge::GameObjectManager::update(float dt)
{
	std::map<std::string, std::vector<GameObject*>>::iterator it = mResourceGroups.begin();
	while (it != mResourceGroups.end())
	{
		for (unsigned int i = 0; i < it->second.size(); i++)
			it->second[i]->update(dt);
		++it;
	}
}



void ssuge::GameObjectManager::setVisible(std::string groupName, bool isVisible)
{
	std::map<std::string, std::vector<GameObject*>>::iterator it = mResourceGroups.find(groupName);
	if (it == mResourceGroups.end())
		THROW_EXCEPTION("Invalid group name '" + groupName + "'");
	else
	{
		for (unsigned int i = 0; i < it->second.size(); ++i)
			it->second[i]->setVisibility(isVisible);
	}
}


void ssuge::GameObjectManager::destroy(std::string groupName, bool destroyGroupName)
{
	std::map<std::string, std::vector<GameObject*>>::iterator it = mResourceGroups.find(groupName);
	if (it == mResourceGroups.end())
		THROW_EXCEPTION("Invalid group name '" + groupName + "'");
	else
	{
		// Destroy all game objects in this group.
		for (unsigned int i = 0; i < it->second.size(); ++i)
			this->removeGameObject(groupName, it->second[i]);

		// Destroy the group in the master group listing
		mResourceGroups.erase(it);

		// Destroy the group name if requested
		if (destroyGroupName)
			mGroupNames.erase(groupName);
	}
}