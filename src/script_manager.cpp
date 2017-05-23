#include <stdafx.h>
#include <script_manager.h>
#include <log_manager.h>
#include <script_game_object.h>
#include <script_functions.h>
#include <script_gui_component.h>

static void stackDump(lua_State * L)
{
	printf("/// STACK DUMP\n");
	for (int i = 1; i <= lua_gettop(L); i++)
	{
		if (lua_isnumber(L, i))
		{
			double num = lua_tonumber(L, i);
			printf("%d: number(%f)\n", i, num);
		}
		else if (lua_isstring(L, i))
		{
			const char * str = lua_tostring(L, i);
			printf("%d: string('%s')\n", i, str);
		}
		else if (lua_istable(L, i))				printf("%d: table\n", i);
		else if (lua_isfunction(L, i))			printf("%d: function\n", i);
		else if (lua_isuserdata(L, i))			printf("%d: userdata\n", i);
		else if (lua_isnil(L, i))				printf("%d: nil\n", i);
		else									printf("%d: ???\n", i);
	}
	printf("\\\\\\ end STACK DUMP\n");
}

// A template-specialization of the msSingleton attribute for the ScriptManager class
template<> ssuge::ScriptManager * ssuge::Singleton<ssuge::ScriptManager>::msSingleton = nullptr;

ssuge::ScriptManager::ScriptManager()
{
	if (mLuaState == NULL)
	{
		mLuaState = luaL_newstate();
		luaL_openlibs(mLuaState);


	
		lua_pushglobaltable(mLuaState);
		luaL_setfuncs(mLuaState, global_functions, 0);
		lua_pop(mLuaState, -1);

		lua_newtable(mLuaState); //{}
		lua_setglobal(mLuaState, "GameObjectGroups");//_G.GameObjectGroups = {}
		lua_pop(mLuaState, -1);

		luaL_newmetatable(mLuaState, "__GameObject__");
		luaL_setfuncs(mLuaState, gameobject_methods, 0);
		lua_setglobal(mLuaState, "GameObject");
		lua_pop(mLuaState, -1);
		luaL_newmetatable(mLuaState, "__GuiComponent__");
		luaL_setfuncs(mLuaState, guicomponent_methods, 0);
		lua_setglobal(mLuaState, "GuiComponent");
		

		LOG_MANAGER->log("Starting up lua interperator", LL_NORMAL);


		LOG_MANAGER->log("Running lua init file", LL_NORMAL);
		SCRIPT_MANAGER->runFile("init.lua");

	
	}
	else
	{
		//this will never happen... Right?
		LOG_MANAGER->log("Multiple lua interperators trying to be started :(", LL_ERROR);
	}



}

ssuge::ScriptManager::~ScriptManager()
{
	lua_close(mLuaState);
}

void ssuge::ScriptManager::runFile(std::string filename, bool hasReturn)
{

	luaL_loadfile(mLuaState,filename.c_str());
	int test = lua_pcall(mLuaState,0,hasReturn?1:0,0);
	
	if (test != LUA_OK)
	{
		
		
		LOG_MANAGER->log("LUA Script ERROR", LL_ERROR);
		const char * errstr = luaL_checkstring(mLuaState,-1);
		std::cout << errstr << std::endl;

	}
	if (hasReturn)
	{
		if (!lua_istable(mLuaState, -1))
		{
			stackDump(mLuaState);
			const char * errstring = "ERROR: THIS CURRENTLY ONLY WORKS WITH TABLES";
			lua_pushstring(mLuaState,errstring);
			lua_error(mLuaState);
		}

		//strip lua part off
		std::string temp = filename;
		int found = temp.find_last_of(".");
		temp.erase(temp.begin() + found, temp.end());
		/*strips  ..\\..\\ */
		found = temp.find_last_of("\\");
		temp = temp.substr(found + 1, temp.length());
	

		lua_setglobal(mLuaState,temp.c_str());
		
	}
	
}

void ssuge::ScriptManager::executeScriptMethod(std::string script ,std::string methodname, std::string gameobject,std::string group)
{
	if (script != "")
	{
		
			lua_getglobal(mLuaState, script.c_str());
			lua_getfield(mLuaState, -1, methodname.c_str());

			lua_getglobal(mLuaState, "GameObjectGroups");
			lua_getfield(mLuaState, -1, group.c_str());
			lua_getfield(mLuaState, -1, gameobject.c_str());

			lua_remove(mLuaState, -2); //cleanup
			lua_remove(mLuaState, -2); //cleanup
			
			lua_pcall(mLuaState, 1, 0, 0);
			lua_remove(mLuaState,-1); //cleanup
	}
	
}

void ssuge::ScriptManager::checkLuaArgs(lua_State * L, int expectedMin, int expectedMax)
{

	if (lua_gettop(L) < expectedMin || lua_gettop(L) > expectedMax)
	{
		stackDump(L);
		std::ostringstream stringStream;
		if(expectedMax != expectedMin)
			stringStream << "Invalid arguement number. Expected:(min)"<<expectedMin<<" (max)"<<expectedMax <<" got: " << lua_gettop(L);
		else
			stringStream << "Invalid arguement number. Expected:"<< expectedMax << " got: " << lua_gettop(L);

		LOG_MANAGER->log(stringStream.str(), LL_ERROR);
		lua_pushstring(L, stringStream.str().c_str());
		lua_error(L);
	}
}
