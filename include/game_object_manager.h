#ifndef _GAME_OBJECT_MANAGER_H_
#define _GAME_OBJECT_MANAGER_H_

#include <stdafx.h>
#include <singleton.h>

#define GAME_OBJECT_MANAGER ssuge::GameObjectManager::getSingletonPtr()

namespace ssuge
{
	// Forward reference to the GameObject class
	class GameObject;

	/// The manager of all GameObjects.  All GameObjects are broken down into 
	/// named groups.  Each group can be batch destroyed / hid / etc.
	class GameObjectManager : public Singleton<GameObjectManager>
	{
	// @@@@@ ATTRIBUTES @@@@@ //
	protected:
		/// The set of all group names
		std::set<std::string> mGroupNames;

		/// The set of all game objects in each group.  This is the "MASTER"
		/// list of all game objects in the game.
		std::map<std::string, std::vector<GameObject*>> mResourceGroups;

		/// A reverse-mapping: GameObject => group name.  I'm not 100% convinced that the
		/// extra storage / book-keeping for this is worth it...
		std::map<GameObject*, std::string> mGameObjectGroups;

	// @@@@@ CONSTRUCTORS / DESTRUCTORS @@@@@ //
	public:
		/// Default Constructor
		GameObjectManager();

		/// Destructor
		virtual ~GameObjectManager();

	// @@@@@ GETTERS / SETTERS @@@@@ //
	public:
		/// Gets a game object (this version is slightly more efficient because we don't have to
		/// search for the containing group).  This method returns nullptr if the GO isn't found.
		GameObject * findGameObject(std::string groupName, std::string gameObjectName);

		/// Finds the given game object.  Returns nullptr if it's not found.  If possible, call
		/// the other method by the same name -- it's more efficient because we don't have to 
		/// search for the group this game object belongs to.
		GameObject * findGameObject(std::string gameObjectName);

	// @@@@@ METHODS @@@@@ //
	public:
		/// Create a new resource group name.  For now, it's an error to use a group name that
		/// hasn't been created in this manner.
		virtual void createGroupName(std::string groupName);

		/// Adds an existing game object not created through the GOM.  The GOM will 
		/// control (and destroy) the memory owned by this object
		virtual void addExisting(std::string groupName, GameObject* go);

		/// Destroys a single game object
		virtual void removeGameObject(std::string groupName, GameObject* go);

		/// Creates a new GameObject
		virtual GameObject* createGameObject(std::string groupName, std::string objectName, 
						unsigned int tag, GameObject * parent = nullptr);

		/// Updates all game objects under control of the GOM
		virtual void update(float dt);

		/// Hides / Shows all game objects within the given group
		virtual void setVisible(std::string groupName, bool isVisible);

		/// Destroys all game objects in the given group.  If destroyGroupName is 
		/// true, the group name itself will also be destroyed.
		virtual void destroy(std::string groupName, bool destroyGroupName);
	
	};
}

#endif
