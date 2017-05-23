#ifndef _PHYSICS_MANAGER_H_
#define _PHYSICS_MANAGER_H_

#include <stdafx.h>
#include <singleton.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <Ogre.h>



// "Convenience macro" for accessing the singleton
#define PHYSICS_MANAGER ssuge::PhysicsManager::getSingletonPtr()

namespace ssuge
{
	/// The goal of the Physics Manager simulate physics in the game
	class PhysicsManager : public Singleton<PhysicsManager>
	{
	/***** ATTRIBUTES *****/
	protected:

		///Pointer to the bullet dynamics world. Needed to add physics to the scene and step simulation
		btDiscreteDynamicsWorld* mDynamicsWorld;

	/***** CONSTRUCTORS / DESTRUCTORS *****/
	public:
		/// sets up the physics manager
		PhysicsManager();

		/// Shuts down the physics manager
		~PhysicsManager();

	/***** METHODS *****/
	public:
		///Sets gravity for the world
		void setGravity(float x, float y, float z);

		///Converts bullet vector 3 to ogre vector 3
		Ogre::Vector3 bulletToOgreVec(btVector3 & v);

		///Converts ogre vector 3 to bullet vector 3
		btVector3 ogreToBulletVec(Ogre::Vector3 & v);

		///Converts bullet Quaternion to ogre Quaternion
		Ogre::Quaternion bulletToOgreQuaternion(btQuaternion & q);
		
		///Converts ogre Quaternion to bullet Quaternion
		btQuaternion ogreToBulletQuaternion(Ogre::Quaternion & q);

		///steps the physics simulation forward
		void update(float dt);

		///adds rigid body to the world for calculation
		void addRigidBody(btRigidBody * bod);

		///removes rigid body from the world
		void removeRigidBody(btRigidBody * bod);
	};
}

#endif