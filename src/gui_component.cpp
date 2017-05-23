#include <stdafx.h>
#include <gui_component.h>
#include <gui_manager.h>
#include <Overlay\OgreTextAreaOverlayElement.h>
#include <Overlay\OgreFont.h>

ssuge::GuiComponent::GuiComponent(std::string elemType, std::string elemName, float posX, float posY, float dimX, float dimY)
{
	mName = elemName;
	mOverlayElement = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", elemName));

	mOverlayElement->setPosition(posX, posY);
	mOverlayElement->setDimensions(dimX, dimY);


	mMat = Ogre::MaterialManager::getSingleton().create(elemName, "General").getPointer();
	mPass = mMat->getTechnique(0)->getPass(0);
	mPass->createTextureUnitState();

	
	mPass->setCullingMode(Ogre::CULL_NONE);                // No culling (triangles can be seen from both sides)
	mPass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);  // Allow transparency from alpha channel (tga, png, ...)
	mMat->setLightingEnabled(false);   // Disable lighting
	mMat->setDepthCheckEnabled(false); // No depth check
	

	this->setMaterial(elemName);
	//GUI_MANAGER->addComponent(this);

}

ssuge::GuiComponent::~GuiComponent()
{

	delete mOverlayElement;

	delete mTextElement;

	delete mMat;

	delete  mPass;
}
//TODO::MAKE IT SO IT DOES NOT LOAD IMAGES ALEADY MADE
void ssuge::GuiComponent::setImage(std::string fname)
{
	Ogre::TexturePtr mTex;
	try 
	{
		mTex = Ogre::TextureManager::getSingleton().load(fname, "General");
	}
	catch (Ogre::FileNotFoundException)
	{
		//ERROR
	}
	if(!mTex.isNull())
		mPass->getTextureUnitState(0)->setTextureName(mTex->getName());
	else 
	{
		//ERROR
	}
}

void ssuge::GuiComponent::setColorSolid(float r, float g, float b)
{
	mPass->getTextureUnitState(0)->setColourOperationEx(Ogre::LayerBlendOperationEx::LBX_SOURCE2, Ogre::LayerBlendSource::LBS_TEXTURE, Ogre::LayerBlendSource::LBS_MANUAL,Ogre::ColourValue::White ,Ogre::ColourValue(r, g, b, 1.f));
}

void ssuge::GuiComponent::setImageColorFilter(float r, float g, float b)
{
	mPass->getTextureUnitState(0)->setColourOperationEx(Ogre::LayerBlendOperationEx::LBX_MODULATE, Ogre::LayerBlendSource::LBS_TEXTURE, Ogre::LayerBlendSource::LBS_MANUAL, Ogre::ColourValue(),Ogre::ColourValue(1.f, 0.f, 0.f, 1.f));
}

int ssuge::GuiComponent::addText(std::string textName, std::string textContent, std::string font, float height, float posX, float posY, float dimX, float dimY)
{
	if (mTextElement != nullptr)
	{
		return 1;
	}

	mTextElement= static_cast<Ogre::TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea", textName));

	this->setText(textContent);
	this->setFont(font);
	this->setTextHeight(height);
	this->setTextPos(posX, posY);
	this->setTextDimensions(dimX, dimY);

	mTextElement->setMetricsMode(Ogre::GMM_RELATIVE);

	mOverlayElement->addChild(mTextElement);

	return 0;
}

//TODO::MAKE IT SO IT DOES NOT LOAD FONTS ALEADY MADE
void ssuge::GuiComponent::setFont(std::string font)
{
	Ogre::ResourcePtr  fontptr = Ogre::FontManager::getSingleton().create(font, "General");

	fontptr->setParameter("type", "truetype");
	// set the .ttf file name
	fontptr->setParameter("source", font);
	// set the size
	fontptr->setParameter("size", "36");
	// set the dpi
	fontptr->setParameter("resolution", "96");

	try
	{
		// load the ttf
		fontptr->load();
		mTextElement->setFontName(fontptr->getName());
	}
	catch (Ogre::FileNotFoundException)
	{
		std::cout << "Font not found" << std::endl;
	}
}

void ssuge::GuiComponent::setField(std::string key, std::string val)
{
	mFields.insert(std::pair<std::string, std::string>(key, val));
}

std::string ssuge::GuiComponent::getField(std::string field)
{
	for (auto& m : mFields)
	{
		if (!std::strcmp(m.first.c_str(), field.c_str()))
		{
			return m.second;
		}
	}

	return "";
}