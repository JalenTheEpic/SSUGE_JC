#include <stdafx.h>
#include <camera_component_factory.h>

// A template-specialization of the msSingleton attribute for the CameraComponentFactory class
template<> ssuge::CameraComponentFactory * ssuge::Singleton<ssuge::CameraComponentFactory>::msSingleton = nullptr;