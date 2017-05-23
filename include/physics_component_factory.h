#ifndef _PHYSICS_COMPONENT_FACTORY_H_
#define _PHYSICS_COMPONENT_FACTORY_H_

#include <stdafx.h>
#include <singleton.h>
#include <component_factory.h>
#include <physics_component.h>

// "Convenience macro" for accessing the singleton
#define PHYSICS_COMPONENT_FACTORY ssuge::PhysicsComponentFactory::getSingletonPtr()

namespace ssuge
{
	
	/// The PhysicsComponentFactory is used to create instances of PhysicsComponents.
	class PhysicsComponentFactory : public ComponentFactory<PhysicsComponent>, public Singleton<PhysicsComponentFactory>
	{
		// @@@@@ CONSTRUCTORS / DESTRUCTORS @@@@@ //
	public:
		/// Creates the Physics component factory with the given number of "slots"
		PhysicsComponentFactory(unsigned int max_size) : ComponentFactory<PhysicsComponent>(max_size) { }

		// @@@@@ METHODS @@@@@ //
	public:
		/// We override this method because I want the PhysicsComponentFactory
		/// to take the collider type, mass, and control.
		PhysicsComponent * allocate(GameObject * gobj, ColliderType ctype,float mass=0.0f,bool control = true);
	};
}

#endif
