#include <stdafx.h>
#include <camera_component.h>
#include <game_object.h>
#include <application.h>


ssuge::CameraComponent::CameraComponent(ssuge::GameObject * p) : ssuge::Component(p)
{
	mCamera = APPLICATION->getSceneManager()->createCamera(p->getName() + "_camera");
	p->getSceneNode()->attachObject(mCamera);
}

ssuge::CameraComponent::~CameraComponent()
{
	if (mCamera)
	{
		getOwner()->getSceneNode()->detachObject(mCamera);
		APPLICATION->getSceneManager()->destroyCamera(mCamera);
		mCamera = nullptr;
	}

	CAMERA_COMPONENT_FACTORY->releasePtr(this);
}



void ssuge::CameraComponent::setAspectRatio(float ar)
{
	if (ar == 0.0f)
		mCamera->setAutoAspectRatio(true);
	else
		mCamera->setAspectRatio(ar);
}