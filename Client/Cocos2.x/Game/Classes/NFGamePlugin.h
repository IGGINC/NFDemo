// -------------------------------------------------------------------------
//    @FileName			:    NFGamePlugin.h
//    @Author           :    Johance
//    @Date             :    2016-12-22
//    @Module           :    NFGamePlugin
//
// -------------------------------------------------------------------------

#ifndef NFC_GAMEPLUGIN_H
#define NFC_GAMEPLUGIN_H

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"


class NFGamePlugin : public NFIPlugin
{
public:
    NFGamePlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};

#endif