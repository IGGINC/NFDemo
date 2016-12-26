// -------------------------------------------------------------------------
//    @FileName			:    UIPlugin.cpp
//    @Author           :    Johance
//    @Date             :    2016-12-26
//    @Module           :    UIPlugin
//
// -------------------------------------------------------------------------
#include "stdafx.h"

#include "UIPlugin.h"

#include "UIManager.h"

const int UIPlugin::GetPluginVersion()
{
    return 0;
}

const std::string UIPlugin::GetPluginName()
{
	return GET_CLASS_NAME(UIPlugin);
}

void UIPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, UIManager, UIManager)
}

void UIPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, UIManager, UIManager)
}
