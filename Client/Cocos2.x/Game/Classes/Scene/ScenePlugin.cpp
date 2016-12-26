// -------------------------------------------------------------------------
//    @FileName			:    ScenePlugin.cpp
//    @Author           :    Johance
//    @Date             :    2016-12-26
//    @Module           :    ScenePlugin
//
// -------------------------------------------------------------------------
#include "stdafx.h"

#include "ScenePlugin.h"

#include "SceneManager.h"

const int ScenePlugin::GetPluginVersion()
{
    return 0;
}

const std::string ScenePlugin::GetPluginName()
{
	return GET_CLASS_NAME(ScenePlugin);
}

void ScenePlugin::Install()
{
	REGISTER_MODULE(pPluginManager, SceneManager, SceneManager)
}

void ScenePlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, SceneManager, SceneManager)
}
