// -------------------------------------------------------------------------
//    @FileName			:    LogicPlugin.cpp
//    @Author           :    Johance
//    @Date             :    2016-12-26
//    @Module           :    LogicPlugin
//
// -------------------------------------------------------------------------
#include "stdafx.h"

#include "LogicPlugin.h"

#include "LoginLogic.h"
#include "PlayerLogic.h"
#include "PropertyLogic.h"
#include "RecordLogic.h"
#include "NetLogic.h"

const int LogicPlugin::GetPluginVersion()
{
    return 0;
}

const std::string LogicPlugin::GetPluginName()
{
	return GET_CLASS_NAME(LogicPlugin);
}

void LogicPlugin::Install()
{
	
	REGISTER_MODULE(pPluginManager, CNetLogic, CNetLogic)
	REGISTER_MODULE(pPluginManager, CLoginLogic, CLoginLogic)
	REGISTER_MODULE(pPluginManager, CPlayerLogic, CPlayerLogic)
	REGISTER_MODULE(pPluginManager, CPropertyLogic, CPropertyLogic)
	REGISTER_MODULE(pPluginManager, CRecordLogic, CRecordLogic)
}

void LogicPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, CNetLogic, CNetLogic)
	UNREGISTER_MODULE(pPluginManager, CLoginLogic, CLoginLogic)
	UNREGISTER_MODULE(pPluginManager, CPlayerLogic, CPlayerLogic)
	UNREGISTER_MODULE(pPluginManager, CPropertyLogic, CPropertyLogic)
	UNREGISTER_MODULE(pPluginManager, CRecordLogic, CRecordLogic)
}
