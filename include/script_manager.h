#ifndef _SCRIPT_MANAGER_H_
#define _SCRIPT_MANAGER_H_

#include <stdafx.h>
#include <singleton.h>
#include <lua.hpp>

#define SCRIPT_MANAGER ssuge::ScriptManager::getSingletonPtr()

namespace ssuge
{

	/// The manager of all GameObjects.  All GameObjects are broken down into 
	/// named groups.  Each group can be batch destroyed / hid / etc.
	class ScriptManager : public Singleton<ScriptManager>
	{
		// @@@@@ ATTRIBUTES @@@@@ //
	protected:
		///The current lua state
		lua_State * mLuaState;

		// @@@@@ CONSTRUCTORS / DESTRUCTORS @@@@@ //
	public:
		/// Default Constructor
		ScriptManager();

		/// Destructor
		virtual ~ScriptManager();

		// @@@@@ METHODS @@@@@ //
	public:
		///Runs a lua file
		void runFile(std::string filename, bool hasReturn = false);

		///Executes a script method on a gameobject. The method must be stored in the golabl state
		void executeScriptMethod(std::string script, std::string methodname, std::string gameobject, std::string group);

		///Makes sure the lua function being called has the proper amount of arguements
		void checkLuaArgs(lua_State * L, int expectedMin, int expectedMax);

	};
}


#endif // _SCRIPT_MANAGER_H_
