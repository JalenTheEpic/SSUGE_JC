#include <stdafx.h>
#include <light_component.h>
#include <game_object.h>
#include <application.h>



ssuge::LightComponent::LightComponent(ssuge::GameObject * p) : ssuge::Component(p)
{
	mLight = APPLICATION->getSceneManager()->createLight(p->getName() + "_light");
	p->getSceneNode()->attachObject(mLight);
}

ssuge::LightComponent::~LightComponent()
{
	if (mLight)
	{
		getOwner()->getSceneNode()->detachObject(mLight);
		APPLICATION->getSceneManager()->destroyLight(mLight);
		mLight = nullptr;
	}

	LIGHT_COMPONENT_FACTORY->releasePtr(this);
}