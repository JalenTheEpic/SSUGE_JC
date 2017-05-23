#ifndef _CAMERA_COMPONENT_H_
#define _CAMERA_COMPONENT_H_

#include <stdafx.h>
#include <component.h>


namespace ssuge
{
	// Forward reference
	template<class T> class ComponentFactory;

	/// This component simply attaches a camera object to the containing game
	/// object.
	class CameraComponent : public Component
	{
	// @@@@@ MEMBER VARIABLES @@@@@ //
	protected:
		/// The underlying ogre camera
		Ogre::Camera * mCamera;

	// @@@@@ CONSTRUCTORS / DESTRUCTORS @@@@@ //
	protected:
		/// Creates a CameraComponent -- Note: I made this protected to force the user
		/// to go through the CameraComponentFactory (below)
		CameraComponent(GameObject * p);

		/// The destructor
		virtual ~CameraComponent();

	// @@@@@ GETTERS / SETTERS @@@@@ //
	public:
		/// Returns the type of this component (ComponentType::CAMERA)
		ComponentType getType() { return ComponentType::CAMERA; }

		/// Gets the ogre camera (is this a good idea?)
		Ogre::Camera * getCamera() { return mCamera; }

		/// Sets the near clip distance
		void setNearClipDistance(float n) { mCamera->setNearClipDistance(n); }

		/// Sets the far clip distance
		void setFarClipDistance(float f) { mCamera->setFarClipDistance(f); }

		/// Sets the aspect ratio (viewport width divided by height).  If 0.0 (default) is passed
		/// the aspect ratio is set to auto.
		void setAspectRatio(float ar = 0.0f);

		/// Attaches the camera contained in this component to the given viewport.
		void attachTo(Ogre::Viewport * v) { v->setCamera(mCamera); }

	// @@@@@ FRIENDS @@@@@ //
		/// I make the factory a friend so it can access the protected constructor.
		friend class ComponentFactory<CameraComponent>;
	};
}

#endif
