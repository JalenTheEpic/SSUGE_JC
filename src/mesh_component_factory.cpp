#include <stdafx.h>
#include <mesh_component_factory.h>
#include <mesh_component.h>
#include <game_object.h>

// A template-specialization of the msSingleton attribute for the MeshComponentFactory class
template<> ssuge::MeshComponentFactory * ssuge::Singleton<ssuge::MeshComponentFactory>::msSingleton = nullptr;

ssuge::MeshComponent * ssuge::MeshComponentFactory::allocate(ssuge::GameObject * gobj, std::string fname)
{
	MeshComponent * mc = new MeshComponent(gobj, fname);
	gobj->addComponent(mc);
	storePtr(mc);
	return mc;
}