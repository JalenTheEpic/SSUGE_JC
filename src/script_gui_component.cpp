#include <stdafx.h>
#include <script_gui_component.h>
#include <lua.hpp>
#include <script_manager.h>

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


int GuiComponent_new(lua_State * L)
{
	
	SCRIPT_MANAGER->checkLuaArgs(L, 2, 6);
	
	// Checks the parameters of the stack.
	std::string elemType = luaL_checkstring(L, 1);

	std::string elemName = luaL_checkstring(L, 2);

	float posX = lua_gettop(L)>=3?luaL_checknumber(L,3):0.f;
	float posY = lua_gettop(L) >= 4 ? luaL_checknumber(L, 4) : 0.f;
	float dimX = lua_gettop(L) >= 5 ? luaL_checknumber(L, 5) : 0.1f;
	float dimY = lua_gettop(L) >= 6 ? luaL_checknumber(L, 6) : 0.1f;
	
	
	
	struct ScriptGuiComponent * temp = (ScriptGuiComponent *)lua_newuserdata(L, sizeof(struct ScriptGuiComponent));

	temp->gc = new ssuge::GuiComponent(elemType, elemName,posX,posY,dimX,dimY);
	
	

	// Gets the metatable for GuiComponent.
	luaL_getmetatable(L, "__GuiComponent__");

	lua_setmetatable(L, -2);



	return 1;

}


int GuiComponent_index(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 2, 2);

	const char * key = luaL_checkstring(L, 2);
	ScriptGuiComponent * G = (ScriptGuiComponent*)luaL_checkudata(L, 1, "__GuiComponent__");
	
	if (G->gc->getField(key).empty())
	{
		luaL_getmetatable(L, "__GuiComponent__");				// [..., table, key, metatable]
		lua_pushvalue(L, -2);							// [..., table, key, metatable, key]
		lua_rawget(L, -2);								// [..., table, key, metatable, result]
		return 1;
	}
	lua_pushstring(L, G->gc->getField(key).c_str());
	return 1;
	
}

int GuiComponent_newindex(lua_State * L)
{
	
	SCRIPT_MANAGER->checkLuaArgs(L, 3, 3);
	//gets the parameters and checks the types for us
	ScriptGuiComponent * G = (ScriptGuiComponent*)luaL_checkudata(L, 1, "__GuiComponent__");
	std::string  key = luaL_checkstring(L, 2);
	std::string  value = luaL_checkstring(L, 3);

	G->gc->setField(key,value);
	


	return 0;
}

int GuiComponent_setPosition(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 3, 3);
	ScriptGuiComponent * G = (ScriptGuiComponent*)luaL_checkudata(L, 1, "__GuiComponent__");
	float x = luaL_checknumber(L,2);
	float y = luaL_checknumber(L, 3);
	G->gc->setPosition(x,y);
	return 0;

}

int GuiComponent_setDimensions(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 3, 3);
	ScriptGuiComponent * G = (ScriptGuiComponent*)luaL_checkudata(L, 1, "__GuiComponent__");
	float x = luaL_checknumber(L, 2);
	float y = luaL_checknumber(L, 3);
	G->gc->setDimensions(x, y);
	return 0;

}

int GuiComponent_setMaterial(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 2, 2);
	ScriptGuiComponent * G = (ScriptGuiComponent*)luaL_checkudata(L, 1, "__GuiComponent__");
	const char * str = luaL_checkstring(L, 2);
	G->gc->setMaterial(str);
	return 0;
}

int GuiComponent_setText(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 2, 2);
	ScriptGuiComponent * G = (ScriptGuiComponent*)luaL_checkudata(L, 1, "__GuiComponent__");
	const char * str = luaL_checkstring(L, 2);
	G->gc->setText(str);
	return 0;
}
int GuiComponent_setFont(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 2, 2);
	ScriptGuiComponent * G = (ScriptGuiComponent*)luaL_checkudata(L, 1, "__GuiComponent__");
	const char * str = luaL_checkstring(L, 2);
	G->gc->setFont(str);
	return 0;
}
int GuiComponent_setTextHeight(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 2, 2);
	ScriptGuiComponent * G = (ScriptGuiComponent*)luaL_checkudata(L, 1, "__GuiComponent__");
	float str = luaL_checknumber(L, 2);
	G->gc->setTextHeight(str);
	return 0;
}
int GuiComponent_setTextPos(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 3,3);
	ScriptGuiComponent * G = (ScriptGuiComponent*)luaL_checkudata(L, 1, "__GuiComponent__");
	float str = luaL_checknumber(L, 2);
	float str2 = luaL_checknumber(L, 3);
	G->gc->setTextPos(str, str2);
	return 0;
}
int GuiComponent_Dimensions(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 3, 3);
	ScriptGuiComponent * G = (ScriptGuiComponent*)luaL_checkudata(L, 1, "__GuiComponent__");
	float str = luaL_checknumber(L, 2);
	float str2 = luaL_checknumber(L, 3);
	G->gc->setTextPos(str, str2);
	return 0;
}

int GuiComponent_show(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 1, 1);
	ScriptGuiComponent * G = (ScriptGuiComponent*)luaL_checkudata(L, 1, "__GuiComponent__");
	G->gc->show();
	return 0;
}

int GuiComponent_hide(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 1, 1);
	ScriptGuiComponent * G = (ScriptGuiComponent*)luaL_checkudata(L, 1, "__GuiComponent__");
	G->gc->hide();
	return 0;
}

int GuiComponent_setImage(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 2, 2);
	ScriptGuiComponent * G = (ScriptGuiComponent*)luaL_checkudata(L, 1, "__GuiComponent__");
	const char * str = luaL_checkstring(L, 2);
	G->gc->setImage(str);
	return 0;
}

int GuiComponent_setColorSolid(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 4, 4);
	ScriptGuiComponent * G = (ScriptGuiComponent*)luaL_checkudata(L, 1, "__GuiComponent__");
	float r = luaL_checknumber(L, 2);
	float g = luaL_checknumber(L, 3);
	float b = luaL_checknumber(L, 4);
	
	G->gc->setColorSolid(r,g,b);
	return 0;
}

int GuiComponent_setImageColorFilter(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 4, 4);
	ScriptGuiComponent * G = (ScriptGuiComponent*)luaL_checkudata(L, 1, "__GuiComponent__");
	float r = luaL_checknumber(L, 2);
	float g = luaL_checknumber(L, 3);
	float b = luaL_checknumber(L, 4);

	G->gc->setImageColorFilter(r, g, b);
	return 0;

}

int GuiComponent_addText(lua_State * L)
{
	SCRIPT_MANAGER->checkLuaArgs(L, 3, 9);
	ScriptGuiComponent * G = (ScriptGuiComponent*)luaL_checkudata(L, 1, "__GuiComponent__");
	const char * str = luaL_checkstring(L, 2);
	const char * str2= luaL_checkstring(L, 3);
	const char * str3= luaL_checkstring(L, 4);
	float str4 = luaL_checknumber(L, 5);
	float str5 = luaL_checknumber(L, 6);
	float str6 = luaL_checknumber(L, 7);
	float str7 = luaL_checknumber(L, 8);
	float str8 = luaL_checknumber(L, 9);
	G->gc->addText(str, str2, str3, str4, str5, str6, str7, str8);
	return 0;
}

const struct luaL_Reg guicomponent_methods[] =
{
	{ "new", GuiComponent_new },
	{ "__index", GuiComponent_index },
	{ "__newindex", GuiComponent_newindex },
	{ "setPosition", GuiComponent_setPosition },
	{ "setDimensions", GuiComponent_setDimensions },
	{ "setMaterial", GuiComponent_setMaterial },

	{ "setText", GuiComponent_setText },
	{ "setFont", GuiComponent_setFont },
	{ "setTextHeight", GuiComponent_setTextHeight },
	{ "setTextPos", GuiComponent_setTextPos },
	{ "setTextDimensions", GuiComponent_Dimensions },

	{ "show", GuiComponent_show },
	{ "hide", GuiComponent_hide },
	{ "setImage", GuiComponent_setImage },
	{ "setColorSolid", GuiComponent_setColorSolid },
	{ "setImageColorFilter", GuiComponent_setImageColorFilter },
	{ "addText", GuiComponent_addText },
	{ NULL, NULL }
};
