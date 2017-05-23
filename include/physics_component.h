#ifndef _PHYSICS_COMPONENT_H_
#define _PHYSICS_COMPONENT_H_

#include <stdafx.h>
#include <component.h>
#include <physics_manager.h>

namespace ssuge
{
	///Enum class to represent collider types
	enum class ColliderType
	{
		BOX,
		SPEHRE
	};



	/// The Physics component is an instance of a bullet physics rigid body
	class PhysicsComponent : public Component
	{
		// @@@@@ ATTRIBUTES @@@@@ //
	protected:
		///pointer to the rigid body
		btRigidBody* mRigidBody;

		///pointer to the gameobject this component is attached to
		GameObject * mGameObject;

		///what the game object is getting controlled by true:bullet 
		bool mControl = true;
		

		// @@@@@ CONSTRUCTORS / DESTRUCTORS @@@@@ //
	protected:
		/// Creates a PhysicsComponent -- Note: I made this protected to force the user
		/// to go through the PhysicsComponentFactory (below)
		PhysicsComponent(GameObject * p, ColliderType ctype, float mass =0);

		/// The destructor
		virtual ~PhysicsComponent();

		// @@@@@ GETTERS / SETTERS @@@@@ //
	public:
		/// Gets the type of this component (ComponentType::PHYSICS)
		ComponentType getType() { return ComponentType::PHYSICS; }

		/// Sets what is controlling the objects position. True is bullet, and false is ogre.
		void setControl(bool c) { mControl = c; }

		// @@@@@ METHODS @@@@@ //
	public:
		
		///updates the players position to match the physics model
		void update(float dt) override;

		// @@@@@ FRIENDS @@@@@ //
		/// The PhysicsComponentFactory is a friend so they can access our private
		/// constructor.
		friend class PhysicsComponentFactory;
	};
}

#endif
