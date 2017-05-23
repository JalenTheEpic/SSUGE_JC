#ifndef _LIGHT_COMPONENT_H_
#define _LIGHT_COMPONENT_H_

#include <stdafx.h>
#include <component.h>


namespace ssuge
{
	// Forward reference
	template<class T> class ComponentFactory;

	/// Adds a light source to the containing game object
	class LightComponent : public Component
	{
	// @@@@@ ATTRIBUTES @@@@@ //
	protected:
		/// The underlying ogre light object
		Ogre::Light * mLight;

	// @@@@ CONSTRUCTORS / DESTRUCTORS @@@@@ //
	protected:
		/// Creates a LightComponent -- Note: I made this protected to force the user
		/// to go through the MeshComponentFactory (below)
		LightComponent(GameObject * p);

		/// The destructor
		virtual ~LightComponent();

	// @@@@@ GETTERS / SETTERS @@@@@ //
	public:
		/// Gets the type of this component (ComponentType::LIGHT)
		ComponentType getType() { return ComponentType::LIGHT; }

		/// Sets the light type (directional, point, or spot)
		void setType(Ogre::Light::LightTypes lt) { mLight->setType(lt); }

		/// Sets the diffuse color (where each component is in the range 0.0...1.0
		void setDiffuseColour(float r, float g, float b, float a = 1.0f) { mLight->setDiffuseColour(Ogre::ColourValue(r, g, b, a)); }
		
		/// Sets the specular color (where each component is in the range 0.0...1.0
		void setSpecularColour(float r, float g, float b, float a = 1.0f) { mLight->setSpecularColour(Ogre::ColourValue(r, g, b, a)); }
		
		/// Sets whether or not this light casts shadows (use these sparingly).  Not all 
		/// light sources cast shadows -- the scene managers shadow technique plays a big role.
		void setCastShadows(bool s) { mLight->setCastShadows(s); }

	// @@@@@ FRIENDS @@@@@ //
		/// The LightComponentFactory is a friend so they can access the hidden constructor in this class
		friend class ComponentFactory<LightComponent>;
	};
}

#endif