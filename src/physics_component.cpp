#include <stdafx.h>
#include <physics_component.h>
#include <game_object.h>
#include <application.h>
#include <physics_component_factory.h>


ssuge::PhysicsComponent::PhysicsComponent(ssuge::GameObject * p, ColliderType ctype, float mass): ssuge::Component(p)
{
	mGameObject = p;
	btCollisionShape* shape = nullptr;
	float r = 0;
	switch (ctype)
	{
	case ssuge::ColliderType::BOX:
		shape = new btBoxShape(PHYSICS_MANAGER->ogreToBulletVec(p->getAabb().getHalfSize()));
		break;

	case ssuge::ColliderType::SPEHRE:
		r = p->getAabb().getHalfSize().x;
		shape = new btSphereShape(btScalar(r)); 
		break;
	default:
		break;
	}

	btTransform transform;
	transform.setIdentity();
	Ogre::Vector3 pos = p->getPosition();
	transform.setOrigin(PHYSICS_MANAGER->ogreToBulletVec(pos));


	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
	mRigidBody = new btRigidBody(rbInfo);

	//add the body to the dynamics world
	PHYSICS_MANAGER->addRigidBody(mRigidBody);


}

ssuge::PhysicsComponent::~PhysicsComponent()
{
	PHYSICS_COMPONENT_FACTORY->releasePtr(this);
}

void ssuge::PhysicsComponent::update(float dt)
{
	if (mControl) {
		btTransform trans;
		btQuaternion rot;
		if (mRigidBody && mRigidBody->getMotionState())
		{
			mRigidBody->getMotionState()->getWorldTransform(trans);
		
		}
		else
		{
			trans = mRigidBody->getWorldTransform();
		}
		rot = mRigidBody->getOrientation();
		mGameObject->setOrientation(PHYSICS_MANAGER->bulletToOgreQuaternion(rot));
		mGameObject->setPosition(PHYSICS_MANAGER->bulletToOgreVec(trans.getOrigin()));
	}

}




