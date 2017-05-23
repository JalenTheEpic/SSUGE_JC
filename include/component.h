#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <stdafx.h>


namespace ssuge
{

	// A forward-reference to the GameObject class
	class GameObject;

	/// The various types of GameObject components
	enum class ComponentType
	{
		MESH,
		LIGHT,
		CAMERA,
		PHYSICS
	};

	/// This is the base class for all components.  We're using this class
	/// kind of like a Java interface class.  We force derived classes to define
	/// certain methods.  We also use this as the base class so we can store
	/// all Components in a game object via C++'s polymorphism functionality.
	class Component
	{
	// @@@@@ ATTRIBUTES @@@@@ //
	protected:
		/// The containing game object.
		GameObject * mOwner;

	// @@@@@ CONSTRUCTORS / DESTRUCTORS @@@@@ //
	public:
		/// The main constructor -- we need to get the parent game object.
		Component(GameObject * p) : mOwner(p) { }

		/// The destructor.
		virtual ~Component() {}

	// @@@@@ GETTERS / SETTERS @@@@@ //
	public:

		/// The pure virtual getType method -- all derived classes must define this or they
		/// will also be abstract classes.
		virtual ComponentType getType() = 0;

		/// Gets the parent game object
		virtual GameObject * getOwner() { return mOwner; }

		/// Updates the component.  
		virtual void update(float dt) { }

		/// This method will be called by the game object if the game object's
		/// internal scene node is modified (moved, attached, detached, etc.)
		virtual void nodeUpdated(const Ogre::Node * n) { }
		
	};

}

#endif
