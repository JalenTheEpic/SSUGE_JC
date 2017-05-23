#include <stdafx.h>
#include <gui_manager.h>



// A template-specialization of the msSingleton attribute for the GuiManager class
template<> ssuge::GuiManager * ssuge::Singleton<ssuge::GuiManager>::msSingleton = nullptr;

ssuge::GuiManager::GuiManager()
{
	
	// Create the main overlay
	mOverlay = Ogre::OverlayManager::getSingleton().create("MainOverlay");
	ssuge::GuiComponent* panel = new GuiComponent("ddd","nameeeee");
	this->addComponent(panel);

	//THIS IS FOR TESTING CHANGE MEEE
	panel->setColorSolid(1.f,0.f,0.f);
	panel->addText("don't care", "This is example text \n lol","Arial.ttf",0.1f);
	panel->setTextDimensions(0.1f, 0.1f);
	panel->setTextPos(0.1f,0.1f);

	mOverlay->show();

	
}

ssuge::GuiManager::~GuiManager()
{
	delete mOverlay;
}

void ssuge::GuiManager::update(float dt)
{

	Ogre::OverlayContainer * panel = mOverlay->getChild("nameeeee");
}

void ssuge::GuiManager::addComponent(GuiComponent * c)
{
	mOverlay->add2D(c->mOverlayElement);
}


void ssuge::GuiManager::removeComponent(GuiComponent * c)
{
	mOverlay->remove2D(c->mOverlayElement);
}

