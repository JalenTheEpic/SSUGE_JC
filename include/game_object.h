#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include <stdafx.h>
#include <component.h>
#include <mesh_component.h>



namespace ssuge
{
	/// This is the beating heart of ssuge.  A GameObject is a "thing" in our game.  Mostly
	/// a GameObject is pretty simple.  We add functionality by creating and adding
	/// components to a GameObject.
	class GameObject : public Ogre::Node::Listener
	{
	// @@@@@ ATTRIBUTES @@@@@ //
	protected:
		/// The Ogre SceneNode.  This stores / modifies positional and hierarchy data
		Ogre::SceneNode * mSceneNode;

		/// A user-defined tag value.  
		unsigned int mTag;

		/// The (unique) name for this game object
		std::string mName;

		/// The collection of components that this game object currently
		/// has "snapped-in"
		std::map<ComponentType, Component*> mComponents;

		///Fileds for lua scripts to get/set
		std::map<std::string, std::string> mFields;
		
		///Name of a script attached to this object
		std::string mScript;

	// @@@@@ CONSTRUCTORS / DESTRUCTORS @@@@@ //
	public:
		/// Basic constructor.  If parent is nullptr, this game object is created
		/// a child of the scene root.  Otherwise, it is created as a child of the given
		/// game object.
		GameObject(unsigned int tag, std::string name, GameObject * parent = nullptr);

		/// Destructor
		virtual ~GameObject();

	// @@@@@ GETTERS / SETTERS @@@@@ //
	public:
		/// Gets the tag of this game object
		virtual unsigned int getTag() const { return mTag; }

		/// Gets the name of this game object
		virtual std::string getName() const { return mName; }

		/// Gets our scene node
		virtual Ogre::SceneNode * getSceneNode() const { return mSceneNode; }

		/// Gets our parent-offset translation
		virtual const Ogre::Vector3 getPosition() const { return mSceneNode->getPosition(); }

		/// Gets our world-space position
		virtual const Ogre::Vector3 getWorldPosition() const { return mSceneNode->_getDerivedPosition(); }

		/// Gets our parent-offset rotation
		virtual const Ogre::Quaternion getOrientation() const { return mSceneNode->getOrientation(); }

		/// Gets our world-space orientation
		virtual const Ogre::Quaternion getWorldOrientation() const { return mSceneNode->_getDerivedOrientation(); }

		///Gets the ogre axis alligned box
		virtual const Ogre::AxisAlignedBox& getAabb()
		{
			mSceneNode->_updateBounds();
			return mSceneNode->_getWorldAABB();
		}

		/// Sets the parent-offset translation of this game object
		virtual void setPosition(float x, float y, float z) { mSceneNode->setPosition(x, y, z); }

		/// Set the parent-offset translation of this game object
		virtual void setPosition(const Ogre::Vector3& v) { mSceneNode->setPosition(v); }
		
		/// Sets the parent-offset rotation of this game object
		virtual void setOrientation(float degrees, float axisX, float axisY, float axisZ) { mSceneNode->setOrientation(Ogre::Quaternion(Ogre::Degree(degrees), Ogre::Vector3(axisX, axisY, axisZ))); }

		/// Sets the parent-offset rotation of this game object
		virtual void setOrientation(const Ogre::Quaternion & q) { mSceneNode->setOrientation(q); }
		
		/// Sets the parent-offset scale of this game object.
		virtual void setScale(float sx, float sy, float sz) { mSceneNode->setScale(sx, sy, sz); }

		/// Sets whether we're visibile or not 
		virtual void setVisibility(bool isVisible) { mSceneNode->setVisible(isVisible, true); }

#pragma region lua functionality


		void setField(std::string key, std::string val);

		std::string getField(std::string field);


		void setScriptName(std::string scriptname);

		void callScriptMethod(std::string methodname);

#pragma endregion





	// @@@@@ METHODS @@@@@ //
	public:
		/// Removes the given component from this game object (if there is one present)
		virtual void removeComponent(ComponentType t);

		/// Adds the given component to our map.  IMPORTANT: the game object is now the
		/// "owner" of this component and the GameObject should destroy that memory.  It is
		/// an error if the pointer is deallocated elsewhere.
		virtual void addComponent(Component * c);

		/// Returns the component of the type requested (or nullptr if we don't have one of those).
		/// It is up to the caller to cast it (it'd be nice if we didn't need this...)
		virtual Component* getComponent(ComponentType t);

		/// This method is called by Ogre when the containing scene node is translated / rotated / etc.
		/// We then notify all our components that this happens.
		virtual void nodeUpdated(const Ogre::Node * n) override;

		/// Makes the negative z-axis of this game object point towards the given point (in world space)
		virtual void lookAt(float x, float y, float z) { mSceneNode->lookAt(Ogre::Vector3(x, y, z), Ogre::Node::TS_WORLD); }

		/// Makes the game object rotate about its local axis
		virtual void rotateLocal(float degrees, float axisX, float axisY, float axisZ) { mSceneNode->rotate(Ogre::Quaternion(Ogre::Degree(degrees), Ogre::Vector3(axisX, axisY, axisZ))); }

		/// Makes the game object rotate about the world axes
		virtual void rotateWorld(float degrees, float axisX, float axisY, float axisZ) { mSceneNode->rotate(Ogre::Quaternion(Ogre::Degree(degrees), Ogre::Vector3(axisX, axisY, axisZ)), Ogre::Node::TS_WORLD); }

		/// Makes the game object translate relative to its local axis
		virtual void translateLocal(float x, float y, float z) { mSceneNode->translate(Ogre::Vector3(x, y, z), Ogre::Node::TS_LOCAL); }

		/// This method is called by the InputManager if this game object happens to be registered as
		/// an input listener.  Eventually, this method will trigger a script call -- for now do nothing
		/// unless we override this method in a derived class
		virtual void handleAction(std::string action, bool isStarting) {}

		/// This is another method that will eventually be handled by our script engine.  But for now, 
		/// we'll also leave it up to any derived classes to provide actual functionality
		virtual void update(float dt);

		/// Makes this game object (and child game objects), including all components not render
		/// (they still, however, exist)
		virtual void setVisible(bool isVisible);
	};
}

#endif
