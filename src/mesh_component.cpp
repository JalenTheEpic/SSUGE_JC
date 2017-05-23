#include <stdafx.h>
#include <mesh_component.h>
#include <game_object.h>
#include <application.h>


ssuge::MeshComponent::MeshComponent(ssuge::GameObject * p, std::string fname) : ssuge::Component(p), mEntity(nullptr),
							mCurrentAnimation(nullptr)
{
	mEntity = APPLICATION->getSceneManager()->createEntity(p->getName() + "_mesh", fname);
	p->getSceneNode()->attachObject(mEntity);
}

ssuge::MeshComponent::~MeshComponent()
{
	if (mEntity)
	{
		getOwner()->getSceneNode()->detachObject(mEntity);
		APPLICATION->getSceneManager()->destroyEntity(mEntity);
		mEntity = nullptr;
	}

	MESH_COMPONENT_FACTORY->releasePtr(this);
}



void ssuge::MeshComponent::update(float dt) 
{
	if (mCurrentAnimation)
	{
		if (mCurrentAnimation->getTimePosition() + dt >= mCurrentAnimation->getLength())
		{
			mCurrentAnimation->setEnabled(false);
			mCurrentAnimation = nullptr;
		}
		else
			mCurrentAnimation->addTime(dt);
	}
}



void ssuge::MeshComponent::startAnimation(std::string animName)
{
	if (mCurrentAnimation != nullptr)
	{
		mCurrentAnimation->setEnabled(false);
	}
	mCurrentAnimation = mEntity->getAnimationState(animName);
	if (mCurrentAnimation != nullptr)
	{
		mCurrentAnimation->setEnabled(true);
		mCurrentAnimation->setTimePosition(0.0f);
	}
}