#include <stdafx.h>
#include <script_functions.h>
#include <log_manager.h>
#include <input_manager.h>
#include <script_manager.h>
#include <application.h>
#include <script_game_object.h>
#include <physics_manager.h>
#include <gui_manager.h>
#include <script_gui_component.h>

int log(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs( L, 1, 1);

	const char * str = luaL_checkstring(L, 1);
	LOG_MANAGER->log(str);
	return 0;
}

int quit(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 0, 0);
	APPLICATION->quit();
	return 0;
}

int registerListener(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 1, 1);
	ScriptGameObject * G = (ScriptGameObject*)luaL_checkudata(L, 1, "__GameObject__");
	INPUT_MANAGER->addListener(G->gObj);
	return 0;
}

int getAxis(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 1, 1);
	std::string str = luaL_checkstring(L,1);
	lua_pushnumber(L,INPUT_MANAGER->getAxisValue(str));
	return 1;
}

int isActionActive(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 1, 1);
	std::string str = luaL_checkstring(L, 1);
	lua_pushboolean(L,INPUT_MANAGER->isActionActive(str));
	return 1;
}


int getDT(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 0, 0);
	lua_pushnumber(L, APPLICATION->getDeltaTime());
	return 1;
}


int createGroup(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 1, 1);
	const char * str = luaL_checkstring(L, 1);
	GAME_OBJECT_MANAGER->createGroupName(str);

	lua_getglobal(L, "GameObjectGroups"); 
	lua_newtable(L);
	lua_setfield(L, -2, str);


	return 0;
}

int setGravity(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 3, 3);
	float x = luaL_checknumber(L,1);
	float y = luaL_checknumber(L, 2);
	float z = luaL_checknumber(L, 3);
	PHYSICS_MANAGER->setGravity(x,y,z);
	return 0;

}

int addgui(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 1, 1);

	ScriptGuiComponent * G = (ScriptGuiComponent*)luaL_checkudata(L, 1, "__GuiComponent__");
	GUI_MANAGER->addComponent(G->gc);
	return 0;

}

int remgui(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 1, 1);

	ScriptGuiComponent * G = (ScriptGuiComponent*)luaL_checkudata(L, 1, "__GuiComponent__");
	GUI_MANAGER->removeComponent(G->gc);
	return 0;

}




const struct luaL_Reg global_functions[]
{
	{"log", log},
	{"createGameGroup", createGroup },
	{"quit",quit},
	{"getDT",getDT},
	{"isActionActive",isActionActive },
	{"registerListener",registerListener },
	{"getAxis",getAxis },
	{"setGravity",setGravity },
	{ "addGuiComponent",addgui },
	{ "removeGuiComponent",remgui },
	{NULL,NULL}
};
