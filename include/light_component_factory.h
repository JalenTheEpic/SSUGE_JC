#ifndef _LIGHT_COMPONENT_FACTORY_H_
#define _LIGHT_COMPONENT_FACTORY_H_

#include <stdafx.h>
#include <light_component.h>
#include <component_factory.h>

#define LIGHT_COMPONENT_FACTORY ssuge::LightComponentFactory::getSingletonPtr()

namespace ssuge
{
	/// The factory to create light components.
	class LightComponentFactory : public ComponentFactory<LightComponent>, public Singleton<LightComponentFactory>
	{
		// @@@@@ CONSTRUCTORS / DESTRUCTORS @@@@@ //
	public:
		LightComponentFactory(unsigned int max_size) : ComponentFactory<LightComponent>(max_size) { }
	};
}

#endif
