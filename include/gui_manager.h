#ifndef _GUI_MANAGER_H_
#define _GUI_MANAGER_H_

#include <stdafx.h>	
#include <singleton.h>
#include <Ogre.h>
#include <Overlay\OgreOverlay.h>
#include <Overlay\OgreOverlayManager.h>
#include <Overlay\OgreOverlaySystem.h>
#include <Overlay\OgreOverlayContainer.h>
#include <input_manager.h>
#include <gui_component.h>
#define GUI_MANAGER ssuge::GuiManager::getSingletonPtr()

namespace ssuge 
{

	class GuiManager : public Singleton<GuiManager>
	{
	
	// @@@@@ ATTRIBUTES @@@@@ //
	protected:
		/// The main Overlay controlled by the GuiManager
		Ogre::Overlay* mOverlay;

	// @@@@@ CONSTRUCTORS / DESTRUCTORS @@@@@ //
	public:

		/// Creates a gui manager singleton
		GuiManager();

		/// Destroys the gui manager and cleans up gui elements
		~GuiManager();

	// @@@@@ METHODS @@@@@ //
	public:
	
		/// Updates the gui
		/// <remarks> currently not in use </remarks>
		void update(float dt);

		/// Adds a component to the default gui overlay
		void addComponent(GuiComponent * c);

		/// Removes a components from the default gui overlay
		void removeComponent(GuiComponent * c);


	};


}



#endif