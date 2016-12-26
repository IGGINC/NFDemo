// -------------------------------------------------------------------------
//    @FileName			:    ScenePlugin.h
//    @Author           :    Johance
//    @Date             :    2016-12-26
//    @Module           :    ScenePlugin
//
// -------------------------------------------------------------------------

#ifndef ScenePlugin_H
#define ScenePlugin_H

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"


class ScenePlugin : public NFIPlugin
{
public:
    ScenePlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};

#endif