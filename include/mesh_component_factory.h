#ifndef _MESH_COMPONENT_FACTORY_H_
#define _MESH_COMPONENT_FACTORY_H_

#include <stdafx.h>
#include <singleton.h>
#include <component_factory.h>

#define MESH_COMPONENT_FACTORY ssuge::MeshComponentFactory::getSingletonPtr()

namespace ssuge
{
	class MeshComponent;

	/// The MeshComponentFactory is used to create instances of MeshComponents.
	class MeshComponentFactory : public ComponentFactory<MeshComponent>, public Singleton<MeshComponentFactory>
	{
		// @@@@@ CONSTRUCTORS / DESTRUCTORS @@@@@ //
	public:
		/// Creates the mesh component factory with the given number of "slots"
		MeshComponentFactory(unsigned int max_size) : ComponentFactory<MeshComponent>(max_size) { }

		// @@@@@ METHODS @@@@@ //
	public:
		/// We override this method because I want the MeshComponentFactory
		/// to take the file name as well as the game object
		MeshComponent * allocate(GameObject * gobj, std::string fname);
	};
}

#endif
