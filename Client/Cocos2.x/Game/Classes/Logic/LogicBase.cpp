// -------------------------------------------------------------------------
//    @FileName			:    CLogicBase.cpp
//    @Author           :    Johance
//    @Date             :    2012-12-15
//    @Module           :    NFCLogicBase
//
// -------------------------------------------------------------------------

#include "LogicBase.h"
#include "../../../../Common/NFMessageDefine/NFMsgDefine.h"
#include "../../../../Common/NFMessageDefine/NFProtocolDefine.hpp"
#include "NetLogic.h"

#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFISceneAOIModule.h"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFINetClientModule.hpp"

NFIClassModule *g_pClassModule = NULL;
NFIElementModule *g_pElementModule = NULL;
NFILogModule *g_pLogModule = NULL;
NFISceneAOIModule *g_pSceneModule = NULL;
NFIKernelModule *g_pKernelModule = NULL;
NFIEventModule *g_pEventModule = NULL;
NFIScheduleModule *g_pScheduleModule = NULL;
NFINetClientModule *g_pNetClientModule = NULL;

bool CLogicBase::Init()
{
    return true;
}

bool CLogicBase::Shut()
{
    return true;
}

bool CLogicBase::ReadyExecute()
{
    return true;
}

bool CLogicBase::Execute()
{


    return true;
}

bool CLogicBase::AfterInit()
{
	static bool bInitGlobal = false;
	if(!bInitGlobal)
	{
		g_pSceneModule = pPluginManager->FindModule<NFISceneAOIModule>();
		g_pClassModule = pPluginManager->FindModule<NFIClassModule>();
		g_pElementModule = pPluginManager->FindModule<NFIElementModule>();
		g_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
		g_pLogModule = pPluginManager->FindModule<NFILogModule>();
		g_pScheduleModule = pPluginManager->FindModule<NFIScheduleModule>();
		g_pEventModule = pPluginManager->FindModule<NFIEventModule>();
		g_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();

		bInitGlobal = true;
	}

    return true;
}


bool CLogicBase::DoEvent(const int nEventID, const NFIDataList & valueList)
{
	bool bRet = false;
	auto moduleEventInfoMapEx = mModuleEventInfoMapEx;

	NF_SHARE_PTR<NFList<MODULE_EVENT_FUNCTOR_PTR>> xEventListPtr = moduleEventInfoMapEx.GetElement(nEventID);
	if (xEventListPtr)
	{
		MODULE_EVENT_FUNCTOR_PTR pFunPtr;
		bool bRet = xEventListPtr->First(pFunPtr);
		while (bRet)
		{
			MODULE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
			pFunc->operator()(nEventID, valueList);

			bRet = xEventListPtr->Next(pFunPtr);
		}

		bRet = true;
	}

	return bRet;
}

bool CLogicBase::RemoveEventCallBack(const int nEventID, void *pTarget)
{
	bool bRet = false;
	auto itTarFun = mModuleEventPrtMap.find(pTarget);
	if(itTarFun == mModuleEventPrtMap.end())
		return false;

	NF_SHARE_PTR<NFList<MODULE_EVENT_FUNCTOR_PTR>> xEventListPtr = mModuleEventInfoMapEx.GetElement(nEventID);
	if (xEventListPtr)
	{
		MODULE_EVENT_FUNCTOR_PTR pFunPtr;
		bool bRet = xEventListPtr->First(pFunPtr);
		while (bRet)
		{
			MODULE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
			
			if(itTarFun->second == pFunc)
			{
				xEventListPtr->Remove(pFunPtr);
				mModuleEventPrtMap.erase(itTarFun);
				break;
			}

			bRet = xEventListPtr->Next(pFunPtr);
		}

		bRet = true;
	}

	return bRet;
}

bool CLogicBase::AddEventCallBack(const int nEventID, const MODULE_EVENT_FUNCTOR_PTR cb)
{
	NF_SHARE_PTR<NFList<MODULE_EVENT_FUNCTOR_PTR>> xEventListPtr = mModuleEventInfoMapEx.GetElement(nEventID);
	if (!xEventListPtr)
	{
		xEventListPtr = NF_SHARE_PTR<NFList<MODULE_EVENT_FUNCTOR_PTR>>(NF_NEW NFList<MODULE_EVENT_FUNCTOR_PTR>());
		mModuleEventInfoMapEx.AddElement(nEventID, xEventListPtr);
	}

	xEventListPtr->Add(cb);

	return false;
}