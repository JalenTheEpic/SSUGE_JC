#include <stdafx.h>
#include <application.h>
#include <script_manager.h>
// A template-specialization of the msSingleton attribute for the Application class
template<> ssuge::Application * ssuge::Singleton<ssuge::Application>::msSingleton = nullptr;


ssuge::Application::Application() : mSDLWindow(nullptr), mState(ssuge::ApplicationState::UNINITIALIZED), mLogManager(nullptr),
									mOgreRoot(nullptr), mRenderWindow(nullptr), mInputManager(nullptr), mSceneManager(nullptr), mCamera(nullptr),
									mMeshComponentFactory(nullptr), mCameraComponentFactory(nullptr), mLightComponentFactory(nullptr),
									mGameObjectManager(nullptr),mScriptManager(nullptr),mPhysicsManager(nullptr),mGuiManager(nullptr)
{

	initialize();

	
	
}


ssuge::Application::~Application()
{
	shutdown();
}



void ssuge::Application::setActiveCamera(CameraComponent * c)
{
	mRenderWindow->getViewport(0)->setCamera(c->getCamera());
}



int ssuge::Application::initialize()
{
	// Create the LogManager instance.  Note this IS the singleton instance.
	mLogManager = new LogManager("ssuge_log.txt");
	if (!mLogManager)
		return 1;

	mPhysicsManager = new PhysicsManager();
	if (!mPhysicsManager)
		return 1;



	// Initialize the SDL sub-systems we're going to use
	if (SDL_Init(SDL_INIT_VIDEO))
	{
		LOG_MANAGER->log("Error initializing SDL", LL_ERROR);
		return 1;
	}
	else
		LOG_MANAGER->log("Initialized SDL", LL_NORMAL);


	// Initialize the ogre root
#if _DEBUG
	mOgreRoot = new Ogre::Root("plugins_d.cfg");
#else
	mOgreRoot = new Ogre::Root("plugins.cfg");
#endif
	if (!mOgreRoot)
	{
		LOG_MANAGER->log("Error creating ogre root", LL_ERROR);
		return 1;
	}
	else
		LOG_MANAGER->log("Created ogre root", LL_NORMAL);

	// Show the config dialog to pick the render system / resolution / etc.
	if (!mOgreRoot->showConfigDialog())
	{
		LOG_MANAGER->log("User cancelled ogre window settings");
		return 1;
	}
	

	

	// Create the render window and attach the SDL leech
	mRenderWindow = mOgreRoot->initialise(true, "Lab 2_JC");
#if _WIN32
	unsigned long hWnd = 0;
	mRenderWindow->getCustomAttribute("WINDOW", &hWnd);
	mSDLWindow = SDL_CreateWindowFrom((void*)hWnd);
#endif

	// Create the (main) viewport -- this used to be in createScene
	Ogre::Viewport * vp = mRenderWindow->addViewport(NULL);
	vp->setBackgroundColour(Ogre::ColourValue(0.4f, 0.4f, 0.4f));

	// Create the input manager
	mInputManager = new InputManager();

	// Add our resource locations.  TODO: Make this more flexible -- maybe a script of some sort?
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("..\\media", "FileSystem", "General");

	// Load / parse scripts
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// Create a scene manager.  I'm using 1 thread for now -- easier to debug...
	mSceneManager = mOgreRoot->createSceneManager(Ogre::ST_GENERIC, "MySceneManager");

	//this allows us to use the Ogre:Overlay things
	Ogre::OverlaySystem *overlay = new Ogre::OverlaySystem();

	mSceneManager->addRenderQueueListener(overlay);
	
	// Create the main camera
	mCamera = mSceneManager->createCamera("SsugeMainCamera");



	// Create the mesh component factory
	mMeshComponentFactory = new MeshComponentFactory(2000);

	// Create the camera component factory
	mCameraComponentFactory = new CameraComponentFactory(20);

	// Create the light component factory
	mLightComponentFactory = new LightComponentFactory(50);

	// Create the light component factory
	mPhysicsComponentFactory = new PhysicsComponentFactory(2000);

	// Create the GOM
	mGameObjectManager = new GameObjectManager();

	mGuiManager = new GuiManager();
	if (!mGuiManager)
		return 1;

	mScriptManager = new ScriptManager();
	if (!mScriptManager)
		return 1;

	// Setup the scene
	createScene();




	// Set the application state to running to indicate that everything went OK
	// (and allow the main loop to run)
	mState = ApplicationState::RUNNING;

	return 0;
}





int ssuge::Application::createScene()
{
	// Enable shadow
	mSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
	mSceneManager->setAmbientLight(Ogre::ColourValue(0.3f, 0.3f, 0.3f));

	// Setup the (simple) keybindings for this program
	INPUT_MANAGER->createAction("cancel");
	INPUT_MANAGER->createAction("fire");
	INPUT_MANAGER->createAction("attack");
	INPUT_MANAGER->createAction("camera_swap");
	INPUT_MANAGER->createAction("hideAndSeek");
	INPUT_MANAGER->createAction("destroy");
	INPUT_MANAGER->addKeyActionBinding(SDLK_ESCAPE, "cancel");
	INPUT_MANAGER->addKeyActionBinding(SDLK_SPACE, "fire");
	INPUT_MANAGER->addKeyActionBinding(SDLK_LCTRL, "attack");
	INPUT_MANAGER->addKeyActionBinding(SDLK_c, "camera_swap");
	INPUT_MANAGER->addKeyActionBinding(SDLK_h, "hideAndSeek");
	INPUT_MANAGER->addKeyActionBinding(SDLK_d, "destroy");

	INPUT_MANAGER->createAxis("horizontal");
	INPUT_MANAGER->createAxis("vertical");
	INPUT_MANAGER->addKeyAxisBinding(SDLK_a, "horizontal", -1.0f);
	INPUT_MANAGER->addKeyAxisBinding(SDLK_d, "horizontal", 1.0f);
	INPUT_MANAGER->addKeyAxisBinding(SDLK_w, "vertical", 1.0f);
	INPUT_MANAGER->addKeyAxisBinding(SDLK_s, "vertical", -1.0f);
	

	// Create a few game objects using our new GOM functionality
	// ... set up the allowable group names
	std::string simpleGroup = "Test";
	GAME_OBJECT_MANAGER->createGroupName(simpleGroup);
	// ... Create an empty game object with a camera component
	GameObject * cam_gobj = GAME_OBJECT_MANAGER->createGameObject(simpleGroup, "camera", 1);
	CameraComponent * camC = CAMERA_COMPONENT_FACTORY->allocate(cam_gobj);
	cam_gobj->setPosition(0, 500, 3000);
	cam_gobj->lookAt(0, 0, 0);
	camC->setNearClipDistance(1.0f);
	camC->setFarClipDistance(5000.0f);
	camC->setAspectRatio();
	camC->attachTo(getViewport());
	// ... Create the floor object
	GameObject * floor_gobj = GAME_OBJECT_MANAGER->createGameObject(simpleGroup, "floor", 0);
	MeshComponent * meshC = MESH_COMPONENT_FACTORY->allocate(floor_gobj, "floor.mesh");
	floor_gobj->setScale(1500, 1, 1500);
	PhysicsComponent * phyC = PHYSICS_COMPONENT_FACTORY->allocate(floor_gobj, ColliderType::BOX);


	GameObject * crate = GAME_OBJECT_MANAGER->createGameObject(simpleGroup, "crate1", 4);
	MeshComponent * meshC2 = MESH_COMPONENT_FACTORY->allocate(crate, "crate.mesh");
	crate->setPosition(0, 1000, 0);
	crate->setScale(100, 100, 100);
	PhysicsComponent * phyC2 = PHYSICS_COMPONENT_FACTORY->allocate(crate, ColliderType::BOX, 1.0f);
	

	return 0;
}




void ssuge::Application::shutdown()
{
	// Destroy the GOM
	if (mGameObjectManager)
		delete mGameObjectManager;
	mGameObjectManager = nullptr;


	if (mPhysicsManager)
		delete mPhysicsManager;
	mPhysicsManager = nullptr;

	if (mPhysicsComponentFactory)
		delete mPhysicsComponentFactory;
	mPhysicsComponentFactory = nullptr;

	// Destroy the LightComponentFactory
	if (mLightComponentFactory)
		delete mLightComponentFactory;
	mLightComponentFactory = nullptr;

	// Destroy the CameraComponentFactory
	if (mCameraComponentFactory)
		delete mCameraComponentFactory;
	mCameraComponentFactory = nullptr;

	// Destroy the MeshComponentFactory
	if (mMeshComponentFactory)
		delete mMeshComponentFactory;
	mMeshComponentFactory = nullptr;

	// Destroy the input manager
	if (mInputManager)
		delete mInputManager;
	mInputManager = nullptr;

	// Destroy everything ogre-related
	if (mOgreRoot)
		delete mOgreRoot;
	mOgreRoot = nullptr;

	// Shutdown SDL
	if (mSDLWindow)
	{
		SDL_DestroyWindow(mSDLWindow);
		if (mLogManager)
			LOG_MANAGER->log("Destroying SDL window", LL_NORMAL);
	}

	// Shut down SDL
	if (mLogManager)
		LOG_MANAGER->log("Shutting down SDL", LL_NORMAL);
	SDL_Quit();

	// Destroy the log manager (no further messages after this obviously)
	if (mLogManager)
	{
		LOG_MANAGER->log("Destroying LogManager...this is our last log:-)", LL_NORMAL);
		delete mLogManager;
	}
}



void ssuge::Application::run()
{
	// If the application state isn't running, something went wrong in initialize -- bail out.
	if (mState != ApplicationState::RUNNING)
		return;

	// The main game loop
	int stime = SDL_GetTicks();
	while (mState == ApplicationState::RUNNING)
	{
		// Update the input manager
		mInputManager->update();

		// Calcuate the delta-time (since the game started or since the last update)
		mDeltaTime= (SDL_GetTicks() - stime) / 1000.0f;
		stime = SDL_GetTicks();
		
		//UPDATE PHYSICS
		mPhysicsManager->update(mDeltaTime);

		// Update all game objects
		mGameObjectManager->update(mDeltaTime);

		GUI_MANAGER->update(mDeltaTime);

		// Render the scene
		mOgreRoot->renderOneFrame();

		// If the window was closed (by clicking on it), set mode to shutting down
		if (mRenderWindow->isClosed())
			mState = ApplicationState::SHUTTING_DOWN;
	}

	LOG_MANAGER->log("Ending main loop", LL_NORMAL);
}


float ssuge::Application::getDeltaTime()
{
	return mDeltaTime;
}

void ssuge::Application::quit()
{
	mState = ApplicationState::SHUTTING_DOWN;
}