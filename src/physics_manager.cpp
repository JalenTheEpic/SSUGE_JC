#include <stdafx.h>
#include <physics_manager.h>

// A template-specialization of the msSingleton attribute for the LogManager class
template<> ssuge::PhysicsManager * ssuge::Singleton<ssuge::PhysicsManager>::msSingleton = nullptr;



ssuge::PhysicsManager::PhysicsManager()
{

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	btDefaultCollisionConfiguration* CollisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	btCollisionDispatcher* Dispatcher = new	btCollisionDispatcher(CollisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	btBroadphaseInterface * OverlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* Solver = new btSequentialImpulseConstraintSolver;

	mDynamicsWorld = new btDiscreteDynamicsWorld(Dispatcher, OverlappingPairCache, Solver, CollisionConfiguration);

	mDynamicsWorld->setGravity(btVector3(0, -11, 0));
	
}

ssuge::PhysicsManager::~PhysicsManager()
{
	delete mDynamicsWorld;
}

void ssuge::PhysicsManager::setGravity(float x, float y, float z)
{
	mDynamicsWorld->setGravity(btVector3(x, y, z));
}

Ogre::Vector3 ssuge::PhysicsManager::bulletToOgreVec(btVector3& v)
{
	return Ogre::Vector3(v.getX(), v.getY(), v.getZ());
}

btVector3 ssuge::PhysicsManager::ogreToBulletVec(Ogre::Vector3& v)
{
	return btVector3(v.x,v.y,v.z);
}

Ogre::Quaternion ssuge::PhysicsManager::bulletToOgreQuaternion(btQuaternion & q)
{
	return Ogre::Quaternion(q.getW(), q.getX(), q.getY(), q.getZ());
}

btQuaternion ssuge::PhysicsManager::ogreToBulletQuaternion(Ogre::Quaternion & q)
{
	return btQuaternion(q.x,q.y,q.z,q.w);
}

void ssuge::PhysicsManager::update(float dt)
{
	mDynamicsWorld->stepSimulation(dt,10);
	
}

void ssuge::PhysicsManager::addRigidBody(btRigidBody * bod)
{
	mDynamicsWorld->addRigidBody(bod);
	
}

void ssuge::PhysicsManager::removeRigidBody(btRigidBody * bod)
{
	mDynamicsWorld->removeRigidBody(bod);
}

