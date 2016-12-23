// -------------------------------------------------------------------------
//    @FileName			:    CNetLogic.h
//    @Author           :    Johance
//    @Date             :    2012-12-15
//    @Module           :    NFCNetLogic
//
// -------------------------------------------------------------------------

#ifndef NFC_NetLogic_MODULE_H
#define NFC_NetLogic_MODULE_H

#include "LogicBase.h"

class CNetLogic
    : public CLogicBase, public NFSingleton<CNetLogic>
{
public:
	CNetLogic() {};
	virtual ~CNetLogic() {};
    CNetLogic(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool ReadyExecute();
    virtual bool Execute();

    virtual bool AfterInit();

public:	
	template<typename BaseType>
	bool AddReceiveCallBack(const int nMsgID, BaseType* pBase, void (BaseType::*handleRecieve)(const int, const int, const char*, const uint32_t))
	{
		return GetNetModule()->AddReceiveCallBack(nMsgID, pBase, handleRecieve);
	}

	void ConnectServer(const std::string strIp, int nPort);	
	void SendToServerByPB(const uint16_t nMsgID, google::protobuf::Message& xData);

protected:
	NFINetClientModule *GetNetModule();
	void OnSocketEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);
	void OnEventResult(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnMsgRecive(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

protected:	
	std::list<std::pair<int, std::string>> m_listDelayMsg;
	NFINetClientModule* m_pNetClientModule;
	NFILogModule* m_pLogModule;
	bool m_bSocketReady;
};

#define g_pNetLogic (CNetLogic::Instance())
#define g_pNetClientModule (CNetLogic::Instance()->GetNetModule())

#endif