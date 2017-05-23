#include <stdafx.h>
#include <script_game_object.h>
#include <mesh_component_factory.h>
#include <mesh_component.h>
#include <script_manager.h>
#include <physics_manager.h>
#include <physics_component.h>
#include <physics_component_factory.h>

int GameObject_new(lua_State * L) 
{
	
	SCRIPT_MANAGER->checkLuaArgs(L, 3, 4);
	
	// Checks the parameters of the stack.
	std::string group = luaL_checkstring(L, 1);

	std::string name = luaL_checkstring(L, 2);

	int tag = luaL_checkinteger(L, 3);
	std::string script = "";
	if (lua_gettop(L) == 4) 
	{
		script = luaL_checkstring(L, 4);
	}
	
	// Gets the gameobject group.
	lua_getglobal(L, "GameObjectGroups");
	lua_pushstring(L, group.c_str());
	lua_gettable(L, -2);

	
	struct ScriptGameObject * temp = (ScriptGameObject *)lua_newuserdata(L, sizeof(struct ScriptGameObject));

	temp->gObj = GAME_OBJECT_MANAGER->createGameObject(group, name, tag, nullptr);

	// Splits up the fluff on the script string name.
	std::string tmp = script;
	if (tmp != "") 
	{
		int found = tmp.find_last_of(".");
		tmp.erase(tmp.begin() + found, tmp.end());
		found = tmp.find_last_of("\\");
		tmp = tmp.substr(found + 1, tmp.length());
	}


	temp->gObj->setScriptName(tmp);

	// Sets the group of the game object.
	temp->gObj->setField("group",group);

	// Gets the metatable for gameobject.
	luaL_getmetatable(L, "__GameObject__");

	lua_setmetatable(L, -2);

	lua_pushvalue(L, -1);

	lua_setfield(L, -3, name.c_str());

	// Calls the onCreate method of the script.
	
	if(script!="")
		SCRIPT_MANAGER->runFile(script, true);
	temp->gObj->callScriptMethod("onCreate");

	return 1;

}


int GameObject_index(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 2, 2);

	const char * key = luaL_checkstring(L, 2);
	ScriptGameObject * G = (ScriptGameObject*)luaL_checkudata(L, 1, "__GameObject__");
	if (!std::strcmp(key,"tag"))
	{
		lua_pushnumber(L, G->gObj->getTag());
		return 1;
	}
	else if (!std::strcmp(key, "name"))
	{

		lua_pushstring(L, G->gObj->getName().c_str());
		
		return 1;

	}
	else if (!std::strcmp(key, "position"))
	{
		// bro you doin too much lmao
		return 0;

	}
	else
	{
		if (G->gObj->getField(key).empty())
		{
			luaL_getmetatable(L, "__GameObject__");				// [..., table, key, metatable]
			lua_pushvalue(L, -2);							// [..., table, key, metatable, key]
			lua_rawget(L, -2);								// [..., table, key, metatable, result]
			return 1;
		}
		lua_pushstring(L, G->gObj->getField(key).c_str());
		return 1;
	}
}

int GameObject_newindex(lua_State * L)
{
	
	SCRIPT_MANAGER->checkLuaArgs(L, 3, 3);
	//gets the parameters and checks the types for us
	ScriptGameObject * G = (ScriptGameObject*)luaL_checkudata(L, 1, "__GameObject__");
	std::string  key = luaL_checkstring(L, 2);
	std::string  value = luaL_checkstring(L, 3);

	G->gObj->setField(key,value);
	


	return 0;
}

int GameObject_translate(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 4, 4);
	ScriptGameObject * G = (ScriptGameObject*)luaL_checkudata(L, 1, "__GameObject__");
	float x = luaL_checknumber(L,2);
	float y = luaL_checknumber(L, 3);
	float z = luaL_checknumber(L, 4);
	G->gObj->translateLocal(x,y,z);
	return 0;
}

int GameObject_createMesh(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 2, 2);
	
	ScriptGameObject * G = (ScriptGameObject*)luaL_checkudata(L, 1, "__GameObject__");
	std::string str = luaL_checkstring(L,2);
	MESH_COMPONENT_FACTORY->allocate(G->gObj,str);

	return 0;
}

static int GameObject_setScale(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 4, 4);
	ScriptGameObject * G = (ScriptGameObject*)luaL_checkudata(L, 1, "__GameObject__");
	float x = luaL_checknumber(L,2);
	float y = luaL_checknumber(L, 3);
	float z = luaL_checknumber(L, 4);
	G->gObj->setScale(x,y,z);

	return 0;
}

static int GameObject_setPosition(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 4, 4);
	ScriptGameObject * G = (ScriptGameObject*)luaL_checkudata(L, 1, "__GameObject__");
	float x = luaL_checknumber(L, 2);
	float y = luaL_checknumber(L, 3);
	float z = luaL_checknumber(L, 4);
	G->gObj->setPosition(x, y, z);

	return 0;
}

static int GameObject_translateLocal(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 4, 4);
	ScriptGameObject * G = (ScriptGameObject*)luaL_checkudata(L, 1, "__GameObject__");
	float x = luaL_checknumber(L, 2);
	float y = luaL_checknumber(L, 3);
	float z = luaL_checknumber(L, 4);
	G->gObj->translateLocal(x, y, z);
	return 0;
}

static int GameObject_createPhysics(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 4, 4);
	ScriptGameObject * G = (ScriptGameObject*)luaL_checkudata(L, 1, "__GameObject__");
	const char * s = luaL_checkstring(L, 2);
	ssuge::ColliderType t;
	
	if (!std::strcmp(s,"box"))
	{
		t = ssuge::ColliderType::BOX;
	}
	else 
	{
		t = ssuge::ColliderType::SPEHRE;
	}
	float mass = luaL_checknumber(L, 3);

	if (!lua_isboolean(L,4))
	{
		lua_pushstring(L,"expected bool");
	}

	bool c = lua_toboolean(L, 4);
	ssuge::PhysicsComponent * phyC2 = PHYSICS_COMPONENT_FACTORY->allocate(G->gObj,t,mass,c);
	
	return 0;
}



const struct luaL_Reg gameobject_methods[] =
{
	{ "new", GameObject_new },
	{ "__index", GameObject_index },
	{ "__newindex", GameObject_newindex },
	{ "translate", GameObject_translate },
	{ "createMesh", GameObject_createMesh },
	{ "setScale", GameObject_setScale },
	{ "setPosition", GameObject_setPosition },
	{ "translateLocal", GameObject_translateLocal },
	{ "createPhysics", GameObject_createPhysics },
	{ NULL, NULL }
};
