#include <stdafx.h>
#include <light_component_factory.h>

// A template-specialization of the msSingleton attribute for the MeshComponentFactory class
template<> ssuge::LightComponentFactory * ssuge::Singleton<ssuge::LightComponentFactory>::msSingleton = nullptr;