#ifndef _MESH_COMPONENT_H_
#define _MESH_COMPONENT_H_

#include <stdafx.h>
#include <component.h>

namespace ssuge
{
	/// The Mesh component is an instance of a mesh -- we'll later add kinematic animation
	/// support here.
	class MeshComponent : public Component
	{
	// @@@@@ ATTRIBUTES @@@@@ //
	protected:
		/// The underlying ogre entity.
		Ogre::Entity * mEntity;

		/// The currently playing animation, or nullptr if none
		Ogre::AnimationState * mCurrentAnimation;

	// @@@@@ CONSTRUCTORS / DESTRUCTORS @@@@@ //
	protected:
		/// Creates a MeshComponent -- Note: I made this protected to force the user
		/// to go through the MeshComponentFactory (below)
		MeshComponent(GameObject * p, std::string fname);

		/// The destructor
		virtual ~MeshComponent();

	// @@@@@ GETTERS / SETTERS @@@@@ //
	public:
		/// Gets the type of this component (ComponentType::MESH)
		ComponentType getType() { return ComponentType::MESH; }

	// @@@@@ METHODS @@@@@ //
	public:
		/// Updates the component.  Overrident from Component  
		virtual void update(float dt) override;

		/// (we'll make this more flexible later).  Stop playing any other
		/// animations and begin playing this one.  
		void startAnimation(std::string animName);


	// @@@@@ FRIENDS @@@@@ //
		/// The MeshComponentFactory is a friend so they can access our private
		/// constructor.
		friend class MeshComponentFactory;
	};
}

#endif
