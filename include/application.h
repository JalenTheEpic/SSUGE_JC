#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <stdafx.h>
#include <log_manager.h>
#include <singleton.h>
#include <mesh_component.h>
#include <mesh_component_factory.h>
#include <light_component_factory.h>
#include <camera_component_factory.h>
#include <physics_component_factory.h>
#include <game_object.h>
#include <input_manager.h>
#include <game_object_manager.h>
#include <script_manager.h>
#include <physics_manager.h>
#include <gui_manager.h>


// "Convenience macro" for accessing the singleton
#define APPLICATION ssuge::Application::getSingletonPtr()

namespace ssuge
{
	/// This enum is used by the Application to indicate which state we're currently in.
	enum class ApplicationState { UNINITIALIZED, RUNNING, SHUTTING_DOWN };

	/// The Application is the hub class.  It contains all top-level objects and is 
	/// responsible for creating / destroying all of these objects.
	class Application : public Singleton<Application>
	{
	// @@@@@ MEMBER VARIABLES @@@@@ //
	protected:
		/// The LogManager instance (the only one in the entire project)
		LogManager * mLogManager;

		/// The Script manager instance (the only one in the entire project)
		ScriptManager * mScriptManager;

		/// The Physics manager instance (the only one in the entire project)
		PhysicsManager * mPhysicsManager;

		///Gui manager instance
		GuiManager * mGuiManager;

		/// The SDL window handle
		SDL_Window * mSDLWindow;
		
		/// The current application state
		ApplicationState mState;

		/// The Ogre root object
		Ogre::Root * mOgreRoot;

		/// The Ogre Render Window
		Ogre::RenderWindow * mRenderWindow;

		/// The InputManager singleton
		InputManager * mInputManager;

		/// The Ogre Scene Manager
		Ogre::SceneManager * mSceneManager;

		/// The main camera (To-do: make this more flexible (e.g. allow more than one camera for oculus))
		Ogre::Camera * mCamera;	

		/// The MeshComponent factory
		MeshComponentFactory * mMeshComponentFactory;

		/// The CameraComponent factory
		CameraComponentFactory * mCameraComponentFactory;

		/// The LightComponent factory
		LightComponentFactory * mLightComponentFactory;

		PhysicsComponentFactory * mPhysicsComponentFactory;

		/// The manager for all game objects
		GameObjectManager * mGameObjectManager;

		/// Delta time for each render pass
		float mDeltaTime;

	// @@@@@ CONSTRUCTORS / DESTRUCTORS @@@@@ //
	public:
		/// Default constructor
		Application();

		/// Destructor
		virtual ~Application();

	// @@@@@ GETTERS / SETTERS @@@@@ //
	public:
		/// Gets the scene manager
		Ogre::SceneManager * getSceneManager() { return mSceneManager; }

		/// Gets the main viewport
		virtual Ogre::Viewport* getViewport() { return mRenderWindow->getViewport(0); }

		/// Gets the active camera
		Ogre::Camera * getActiveCamera() { return mRenderWindow->getViewport(0)->getCamera(); }

		/// Sets the active camera
		virtual void setActiveCamera(CameraComponent * c);

		///Returns delta time at current frame
		float getDeltaTime();

	// @@@@@ METHODS @@@@@ //
	public:
		/// Starts the main loop.  This function won't exit until the program is ready to shut down.
		void run();

		/// Triggers a shutdown in the very near future
		void quit();

	protected:
		/// Initializes the application.  This method is called by the constructor and 
		/// should be re-callable (after shutdown has been called)
		int initialize();

		/// Creates the scene
		int createScene();

		/// Shuts down all application objects.  This method is called by the destructor and
		/// should be callable while the program is running (this feature is currently untested, though)
		void shutdown();
	};
}

#endif
