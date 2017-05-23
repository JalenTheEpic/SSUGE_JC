#ifndef _SCRIPT_GAME_OBJECT_H_
#define _SCRIPT_GAME_OBJECT_H_
#include <stdafx.h>
#include <game_object.h>
#include <game_object_manager.h>
#include <lua.hpp>



struct ScriptGameObject
{
	ssuge::GameObject * gObj;
};

extern const struct luaL_Reg gameobject_methods[];




#endif // !_SCRIPT_GAME_OBJECT_H_

