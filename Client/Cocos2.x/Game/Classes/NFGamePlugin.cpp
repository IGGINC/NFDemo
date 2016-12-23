// -------------------------------------------------------------------------
//    @FileName			:    NFGamePlugin.cpp
//    @Author           :    Johance
//    @Date             :    2016-12-22
//    @Module           :    NFGamePlugin
//
// -------------------------------------------------------------------------
#include "stdafx.h"

#include "NFGamePlugin.h"

#include "NFComm\NFConfigPlugin\NFCClassModule.h"
#include "NFComm\NFConfigPlugin\NFCElementModule.h"
#include "NFComm\NFLogPlugin\NFCLogModule.h"
#include "NFComm\NFLogPlugin\NFCLogModule.h"

#include "NFComm\NFKernelPlugin\NFCKernelModule.h"
#include "NFComm\NFKernelPlugin\NFCSceneAOIModule.h"
#include "NFComm\NFKernelPlugin\NFCEventModule.h"
#include "NFComm\NFKernelPlugin\NFCScheduleModule.h"

#include "Logic\LoginLogic.h"
#include "Logic\NetLogic.h"
#include "UI\UIManager.h"
const int NFGamePlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFGamePlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFGamePlugin);
}

void NFGamePlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFIClassModule, NFCClassModule)
    REGISTER_MODULE(pPluginManager, NFIElementModule, NFCElementModule)
    REGISTER_MODULE(pPluginManager, NFILogModule, NFCLogModule)
    REGISTER_MODULE(pPluginManager, NFISceneAOIModule, NFCSceneAOIModule)
	REGISTER_MODULE(pPluginManager, NFIKernelModule, NFCKernelModule)
	REGISTER_MODULE(pPluginManager, NFIEventModule, NFCEventModule)
	REGISTER_MODULE(pPluginManager, NFIScheduleModule, NFCScheduleModule)
	
    REGISTER_MODULE(pPluginManager, NFINetClientModule, NFINetClientModule)
	
	REGISTER_MODULE(pPluginManager, CNetLogic, CNetLogic)
	REGISTER_MODULE(pPluginManager, CLoginLogic, CLoginLogic)
	REGISTER_MODULE(pPluginManager, UIManager, UIManager)
}

void NFGamePlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIElementModule, NFCElementModule)
    UNREGISTER_MODULE(pPluginManager, NFIClassModule, NFCClassModule)
    UNREGISTER_MODULE(pPluginManager, NFILogModule, NFCLogModule)
	UNREGISTER_MODULE(pPluginManager, NFIEventModule, NFCEventModule)
	UNREGISTER_MODULE(pPluginManager, NFIKernelModule, NFCKernelModule)
	UNREGISTER_MODULE(pPluginManager, NFISceneAOIModule, NFCSceneAOIModule)
	UNREGISTER_MODULE(pPluginManager, NFIScheduleModule, NFCScheduleModule)

    UNREGISTER_MODULE(pPluginManager, NFINetClientModule, NFINetClientModule)
	
	UNREGISTER_MODULE(pPluginManager, CNetLogic, CNetLogic)
	UNREGISTER_MODULE(pPluginManager, CLoginLogic, CLoginLogic)
	UNREGISTER_MODULE(pPluginManager, UIManager, UIManager)
}
