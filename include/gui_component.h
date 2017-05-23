#ifndef _GUI_COMPONENT_H_
#define _GUI_COMPONENT_H_
#include <stdafx.h>
#include <Ogre.h>
#include <Overlay\OgreOverlay.h>
#include <Overlay\OgreOverlayManager.h>
#include <Overlay\OgreOverlaySystem.h>
#include <Overlay\OgreOverlayContainer.h>
#include <Overlay\OgreOverlayElement.h>
#include <Overlay\OgreTextAreaOverlayElement.h>
#include <Overlay\OgreFont.h>


namespace ssuge 
{
	class GuiManager;
	///Wrapper for ogre overlay elements
	class GuiComponent 
	{
		friend GuiManager;
	// @@@@@ ATTRIBUTES @@@@@ //
	protected:
		/// Name of GUI Component
		std::string mName;

		/// Ogre overlay element GuiComponent wraps
		Ogre::OverlayContainer* mOverlayElement;

		/// Optional Text Area Overlay Element (right now this only allows for one per container)
		Ogre::TextAreaOverlayElement* mTextElement= nullptr;

		/// Pointer to the material of the Overlay Element
		Ogre::Material * mMat;

		/// Pointer to the pass of the material. 
		/// Rendering can be repeated with many passes for more complex effects. Each pass is either a 
		/// fixed-function pass (meaning it does not use a vertex or fragment program) or a programmable pass 
		/// (meaning it does use either a vertex and fragment program, or both).
		/// this is a programmable pass
		Ogre::Pass * mPass;

		///Fields for lua scripts to get/set
		std::map<std::string, std::string> mFields;

	// @@@@@ CONSTRUCTORS / DESTRUCTORS @@@@@ //
	public:
		/// Initializer for a GUI Component
		/// This must be added through the overlaymanger after creation
		/// Defaults to the upper left corner of the screen
		/// Dimentions are set to 10% of the screen w/h(0.1f) by default
		/// Elem type is not actually used for now
		GuiComponent(std::string elemType, std::string elemName, float posX=0.f, float posY=0.f, float dimX=0.1f, float dimY=0.1f);

		/// Cleans up the GUI Component on destroy
		~GuiComponent();
		
	// @@@@@ GETTERS / SETTERS @@@@@ //
	public:
		/// Sets the position of the GUI Component
		/// BE CAREFUL: The values range from 0-1, anything past that is off the screen. 
		void setPosition(float posX, float posY) { mOverlayElement->setPosition(posX, posY); }

		/// Sets the dimensions of the GUI Component
		/// BE CAREFUL: The values range from 0-1, anything past that is off the screen. 
		void setDimensions(float dimX, float dimY) { mOverlayElement->setDimensions(dimX, dimY); }

		/// Sets the material of the GUI Component. Can either be a 
		/// pre made material or a programmed material
		void setMaterial(std::string material) { mOverlayElement->setMaterialName(material); }

		/// Sets the color of the GUI Component
		/// <remarks>This does not seem to do anything.</remarks>  
		void setColor(float r, float g, float b, float a) { mOverlayElement->setColour(Ogre::ColourValue(r, g, b, a));}
		 
		/// Sets the text of the GUI Component
		/// <remarks>NOT IMPLEMENTED</remarks> 
		void setText(std::string text) { mTextElement->setCaption(text); }

		/// Sets the font of the GUI Component 
		void setFont(std::string font);

		/// Sets the text height of the GUI Component
		void setTextHeight(float height) { mTextElement->setCharHeight(height); }

		/// Sets the pos of text in the GUI Component
		void setTextPos(float posX, float posY) { mTextElement->setPosition(posX, posY); }

		/// Sets the dimensions of text in the GUI Component 
		void setTextDimensions(float dimX, float dimY) { mTextElement->setDimensions(dimX, dimY); }


		///sets the value at specified key. For lua
		void setField(std::string key, std::string val);

		///Gets the value at specified field. For lua
		std::string getField(std::string field);



	// @@@@@ METHODS @@@@@ //
	public:
		/// Shows the GUI Component 
		void show() { mOverlayElement->show(); }

		/// Hides the GUI Component 
		void hide() { mOverlayElement->hide(); }

		/// Sets an Image for the GUI Component 
		void setImage(std::string fname);

		/// Sets an solid color for the GUI Component 
		void setColorSolid(float r, float g, float b);

		/// Applys a color filter to an image.
		/// this only works properly if there is an image
		/// attached to the GUI Component
		void setImageColorFilter(float r, float g, float b);

		/// adds a text element to the GUI Component
		/// <remarks>NOT IMPLEMENTED</remarks>
		int addText(std::string textName, std::string textContent, std::string font="Arial.ttf", float height=0.5f, float posX = 0.1f, float posY = 0.1f, float dimX = 0.1f, float dimY = 0.1f);

	};

}



#endif // !_GUI_COMPONENT_H_

