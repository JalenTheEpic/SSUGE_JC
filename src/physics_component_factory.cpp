#include <stdafx.h>
#include <physics_component_factory.h>
#include <game_object.h>

// A template-specialization of the msSingleton attribute for the MeshComponentFactory class
template<> ssuge::PhysicsComponentFactory * ssuge::Singleton<ssuge::PhysicsComponentFactory>::msSingleton = nullptr;

ssuge::PhysicsComponent * ssuge::PhysicsComponentFactory::allocate(ssuge::GameObject * gobj, ColliderType ctype,float mass,bool c )
{
	PhysicsComponent * pc = new PhysicsComponent(gobj, ctype,mass);
	pc->setControl(c);
	gobj->addComponent(pc);
	
	storePtr(pc);
	return pc;
}