#ifndef _CAMERA_COMPONENT_FACTORY_H_
#define _CAMERA_COMPONENT_FACTORY_H_

#include <stdafx.h>
#include <camera_component.h>
#include <component_factory.h>

#define CAMERA_COMPONENT_FACTORY ssuge::CameraComponentFactory::getSingletonPtr()

namespace ssuge
{
	/// This is the factory to create camera components.  It looks kind of bare -- I am 
	/// getting the functionality from the two base classes.  I need this declaration, though, 
	/// in order to "mix" the two base classes into one.
	class CameraComponentFactory : public ComponentFactory<CameraComponent>, public Singleton<CameraComponentFactory>
	{
		// @@@@@ CONSTRUCTORS / DESTRUCTORS @@@@@ //
	public:
		CameraComponentFactory(unsigned int max_size) : ComponentFactory<CameraComponent>(max_size) { }
	};
}

#endif
