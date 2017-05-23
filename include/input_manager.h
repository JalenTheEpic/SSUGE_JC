#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

#include <stdafx.h>
#include <singleton.h>
#include <game_object.h>

// "Convenience macro" for accessing the singleton
#define INPUT_MANAGER ssuge::InputManager::getSingletonPtr()

namespace ssuge
{
	/// This is the hub for all things input-related.  Our InputManager support event-handling
	/// (through a listener interface) and a device-polling methods.  We've abstracted the notion
	/// of input to actions (e.g. "Jump") and axes (e.g. "Horizontal") rather than allowing
	/// the user to name specific device "thingys" (e.g. the 'A' button on controller 1)
	class InputManager : public Singleton<InputManager>
	{
	// @@@@@ MEMBER VARIABLES @@@@@ //
	protected:
		/// All GameObjects that are currently observers.
		std::vector<GameObject*> mListeners;

		/// A binding between an SDL keycode and an action
		std::map<SDL_Keycode, std::string> mKeyActionBindings;

		/// A binding between an SDL keycode and a pair (the axis name and float value for this button [-1 / +1 normally])
		std::map<SDL_Keycode, std::pair<std::string, float>> mKeyAxisBindings;

		/// The currently created actions -- these are added by calling createAction.  It is
		/// an error to attempt to bind a device button to an action name not in this set.
		std::set<std::string> mAvailableActions;

		/// The currently created axis names -- these are added by calling createAxis.  It is
		/// an error to attempt to bind a device button to an action name not in this set.
		std::set<std::string> mAvailableAxes;

		/// The current axis values (normally -1...+1) for each named axis
		std::map<std::string, float> mAxisValues;

		/// The number of devices that are currently triggering this action (I do this so that
		/// each time a device stops pressing the button, we decrease by 1; the action is considered "active"
		/// if the value here is non-zero.
		std::map<std::string, int> mActiveActionCounts;


	// @@@@@ CONSTRUCTORS / DESTRUCTORS @@@@@ //
	public:
		/// The default constructor.
		InputManager();

		/// The destructor
		virtual ~InputManager();

	// @@@@@ GETTERS / SETTERS @@@@@ //
	public:
		/// [device-polling]: is this action currently active on at least one device?
		bool isActionActive(std::string action);

		/// [device-polling]: what is this current axis value?
		float getAxisValue(std::string axisName);

	// @@@@@ METHODS @@@@@ //
	public:
		/// Called once per frame by the application -- processes waiting events.
		void update();

		/// Create a new action name -- bind it by calling add???ActionBinding
		void createAction(std::string name);

		/// Destroys a previously created action name
		void destroyAction(std::string name);

		/// Create a new axis name -- bind it by calling add???AxisBinding
		void createAxis(std::string name);

		/// Destroys a previously created axis name
		void destroyAxis(std::string name);

		/// Registers a game object as a listener
		void addListener(GameObject * gobj);

		/// De-registers a game object as a listener
		void removeListener(GameObject * gobj);

		/// Adds a new binding for the given keycode to an action name
		void addKeyActionBinding(SDL_Keycode k, std::string action);

		/// Removes an existing action binding for this keycode
		void removeKeyActionBinding(SDL_Keycode k);

		/// Adds a new binding for the given keycode to an axis name.  axisValue is normally either -1 or +1
		void addKeyAxisBinding(SDL_Keycode k, std::string axisName, float axisValue);

		/// Removes an existing axis binding for this keycode
		void removeKeyAxisBinding(SDL_Keycode k);

	protected:
		/// Internal method to process a keyboard event (KEYDOWN or KEYUP)
		void handleKeyEvent(SDL_Event * evt);

		/// Sends a message to all listeners.
		void broadcastAction(std::string action, bool starting);
	};
}

#endif
