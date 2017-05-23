#include <stdafx.h>
#include <game_object.h>
#include <application.h>
#include <script_manager.h>
#include <exception.h>

ssuge::GameObject::GameObject(unsigned int tag, std::string name, GameObject * parent ) : mTag(tag), mName(name)
{
	if (parent)
		mSceneNode = parent->getSceneNode()->createChildSceneNode(name);
	else
		mSceneNode = APPLICATION->getSceneManager()->getRootSceneNode()->createChildSceneNode(name);
	
	// Register this scene node as a Node::Listener
	mSceneNode->setListener(this);

	// A little trick (I wasn't expecting you to uncover this on your own).  Here I'm attaching
	// our GameObject* for this node as a "user-field" in a dictionary stored internally in the
	// scene node object.
	Ogre::UserObjectBindings & bindings = mSceneNode->getUserObjectBindings();
	bindings.setUserAny("game_object", Ogre::Any(this));
	
	//mSceneNode->showBoundingBox(true);
	
}


ssuge::GameObject::~GameObject()
{
	// Remove ourself as a listener to this node
	mSceneNode->setListener(NULL);

	// Destroy our components
	std::map<ComponentType, Component*>::iterator iter = mComponents.begin();
	while (iter != mComponents.end())
	{
		delete iter->second;
		iter->second = nullptr;
		++iter;
	}
	mComponents.clear();

	// Detach our scene node from its parent.  We might not have one if an ancestor
	// node was destroyed before us.  Not an error...(I think)
	if (mSceneNode->getParentSceneNode())
	{
		mSceneNode->getParentSceneNode()->removeChild(mSceneNode);
	}
	APPLICATION->getSceneManager()->destroySceneNode(mSceneNode);
	mSceneNode = NULL;
}



void ssuge::GameObject::removeComponent(ssuge::ComponentType t)
{
	std::map<ComponentType, Component*>::iterator iter = mComponents.find(t);
	if (iter != mComponents.end())
		mComponents.erase(iter);
}


void ssuge::GameObject::addComponent(ssuge::Component * c)
{
	// Delete the existing component of that type, if there is one
	std::map<ComponentType, Component*>::iterator iter = mComponents.find(c->getType());
	if (iter != mComponents.end())
		delete iter->second;
	mComponents[c->getType()] = c;
}


ssuge::Component* ssuge::GameObject::getComponent(ssuge::ComponentType t)
{
	// Make sure we have one of those types -- otherwise, we'll actually create a garbage pointer.
	std::map<ComponentType, Component*>::iterator it = mComponents.find(t);
	if (it != mComponents.end())
	{
		return it->second;
	}
	else
		return nullptr;
}


void ssuge::GameObject::nodeUpdated(const Ogre::Node * n) 
{
	std::map<ComponentType, Component*>::iterator iter = mComponents.begin();
	while (iter != mComponents.end())
	{
		iter->second->nodeUpdated(n);
		++iter;
	}
}



void ssuge::GameObject::update(float dt) 
{
	std::map<ComponentType, Component*>::iterator it = mComponents.begin();
	while (it != mComponents.end())
	{
		it->second->update(dt);
		++it;
	}
	callScriptMethod("onUpdate");
}



void ssuge::GameObject::setVisible(bool isVisible)
{
	mSceneNode->setVisible(isVisible, true);
}


#pragma region lua functionality
void ssuge::GameObject::setField(std::string key , std::string val)
{
	mFields.insert(std::pair<std::string, std::string>(key, val));
}

std::string ssuge::GameObject::getField(std::string field)
{
	for (auto& m: mFields)
	{
		if (!std::strcmp(m.first.c_str() ,field.c_str()))
		{
			return m.second;
		}
	}

	
	//LOG_MANAGER->log("Field does not exist", LL_WARNING);
	//THROW_EXCEPTION("Attemp to access a non existant field");
	return "";
}

void ssuge::GameObject::setScriptName(std::string scriptname)
{
	mScript = scriptname;
}

void ssuge::GameObject::callScriptMethod(std::string methodname)
{
	//std::cout << mScript << std::endl;
	if(mScript!= "")
		SCRIPT_MANAGER->executeScriptMethod(mScript,methodname, mName,getField("group"));
}
#pragma endregion
