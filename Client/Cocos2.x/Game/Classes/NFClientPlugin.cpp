// -------------------------------------------------------------------------
//    @FileName			:    NFClientPlugin.cpp
//    @Author           :    Johance
//    @Date             :    2016-12-22
//    @Module           :    NFClientPlugin
//
// -------------------------------------------------------------------------
#include "stdafx.h"

#include "NFClientPlugin.h"

#include "NFComm\NFConfigPlugin\NFCClassModule.h"
#include "NFComm\NFConfigPlugin\NFCElementModule.h"
#include "NFComm\NFLogPlugin\NFCLogModule.h"
#include "NFComm\NFLogPlugin\NFCLogModule.h"

#include "NFComm\NFKernelPlugin\NFCKernelModule.h"
#include "NFComm\NFKernelPlugin\NFCSceneAOIModule.h"
#include "NFComm\NFKernelPlugin\NFCEventModule.h"
#include "NFComm\NFKernelPlugin\NFCScheduleModule.h"

const int NFClientPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFClientPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFClientPlugin);
}

void NFClientPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFIClassModule, NFCClassModule)
    REGISTER_MODULE(pPluginManager, NFIElementModule, NFCElementModule)
    REGISTER_MODULE(pPluginManager, NFILogModule, NFCLogModule)
    REGISTER_MODULE(pPluginManager, NFISceneAOIModule, NFCSceneAOIModule)
	REGISTER_MODULE(pPluginManager, NFIKernelModule, NFCKernelModule)
	REGISTER_MODULE(pPluginManager, NFIEventModule, NFCEventModule)
	REGISTER_MODULE(pPluginManager, NFIScheduleModule, NFCScheduleModule)
	
    REGISTER_MODULE(pPluginManager, NFINetClientModule, NFINetClientModule)
}

void NFClientPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIElementModule, NFCElementModule)
    UNREGISTER_MODULE(pPluginManager, NFIClassModule, NFCClassModule)
    UNREGISTER_MODULE(pPluginManager, NFILogModule, NFCLogModule)
	UNREGISTER_MODULE(pPluginManager, NFIEventModule, NFCEventModule)
	UNREGISTER_MODULE(pPluginManager, NFIKernelModule, NFCKernelModule)
	UNREGISTER_MODULE(pPluginManager, NFISceneAOIModule, NFCSceneAOIModule)
	UNREGISTER_MODULE(pPluginManager, NFIScheduleModule, NFCScheduleModule)

    UNREGISTER_MODULE(pPluginManager, NFINetClientModule, NFINetClientModule)
}
