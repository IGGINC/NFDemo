// -------------------------------------------------------------------------
//    @FileName			:    LogicPlugin.h
//    @Author           :    Johance
//    @Date             :    2016-12-26
//    @Module           :    LogicPlugin
//
// -------------------------------------------------------------------------

#ifndef LOGIC_PLUGIN_H
#define LOGIC_PLUGIN_H

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"


class LogicPlugin : public NFIPlugin
{
public:
    LogicPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};

#endif