// -------------------------------------------------------------------------
//    @FileName			:    CLogicBase.h
//    @Author           :    Johance
//    @Date             :    2012-12-15
//    @Module           :    NFCLogicBase
//
// -------------------------------------------------------------------------

#ifndef NFC_LogicBase_MODULE_H
#define NFC_LogicBase_MODULE_H

#include "../../../../Common/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFINetClientModule.hpp"
#include "NFComm/NFCore/NFSingleton.hpp"

class CLogicBase
    : public NFIModule
{
protected:	
	typedef std::function<int(const int, const NFIDataList&)> MODULE_EVENT_FUNCTOR;
	typedef NF_SHARE_PTR<MODULE_EVENT_FUNCTOR> MODULE_EVENT_FUNCTOR_PTR;//EVENT

public:
	CLogicBase() {};
	virtual ~CLogicBase() {};
    CLogicBase(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool ReadyExecute();
    virtual bool Execute();

    virtual bool AfterInit();
	
	template<typename BaseType>
	bool AddEventCallBack(const int nEventID, BaseType* pBase, int (BaseType::*handler)(const int, const NFIDataList&))
	{
		MODULE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2);
		MODULE_EVENT_FUNCTOR_PTR functorPtr(new MODULE_EVENT_FUNCTOR(functor));
		mModuleEventPrtMap[pBase] = functorPtr.get();
		return AddEventCallBack(nEventID, functorPtr);
	}
	virtual bool DoEvent(const int nEventID, const NFIDataList& valueList);
	virtual bool RemoveEventCallBack(const int nEventID, void *pTarget);

protected:
	virtual bool AddEventCallBack(const int nEventID, const MODULE_EVENT_FUNCTOR_PTR cb);

	// ������Ϣ
public:

	// ������Ϣ
private:

public:

private:
	NFList<int> mModuleRemoveListEx;
	NFMapEx<int, NFList<MODULE_EVENT_FUNCTOR_PTR>> mModuleEventInfoMapEx;
	std::map<void*, MODULE_EVENT_FUNCTOR*> mModuleEventPrtMap;
};

#endif