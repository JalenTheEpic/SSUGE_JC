#ifndef _SCRIPT_GUI_COMPONENT_H_
#define _SCRIPT_GUI_COMPONENT_H_
#include <stdafx.h>
#include <gui_component.h>
#include <gui_manager.h>



struct ScriptGuiComponent
{
	ssuge::GuiComponent * gc;
};

extern const struct luaL_Reg guicomponent_methods[];




#endif // !_SCRIPT_GUI_COMPONENT_H_

