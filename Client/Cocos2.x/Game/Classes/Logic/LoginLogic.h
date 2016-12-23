// -------------------------------------------------------------------------
//    @FileName			:    CLoginLogic.h
//    @Author           :    Johance
//    @Date             :    2012-12-15
//    @Module           :    NFCLoginLogic
//
// -------------------------------------------------------------------------

#ifndef NFC_LOGINLOGIC_MODULE_H
#define NFC_LOGINLOGIC_MODULE_H

#include "LogicBase.h"

enum LoginLogicEvent
{
	E_LoginEvent_LoginSuccess,
	E_LoginEvent_WorldList,
};

class CLoginLogic
    : public CLogicBase, public NFSingleton<CLoginLogic>
{
public:
	CLoginLogic() {};
	virtual ~CLoginLogic() {};
    CLoginLogic(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool ReadyExecute();
    virtual bool Execute();

    virtual bool AfterInit();

	// 发送消息
public:
	void LoginPB(const std::string &strAccount, const std::string &strPwd, const std::string &strKey);
	void RequireWorldList();
	void RequireConnectWorld(int nWorldID);
	void RequireVerifyWorldKey(const std::string &strAccount, const std::string &strKey);
	void RequireServerList();
	void RequireSelectServer(int nServerID);

	// 接收消息
private:
	void OnLoginProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnWorldList(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnConnectWorld(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnConnectKey(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnSelectServer(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

public:
	std::vector<NFMsg::ServerInfo> GetWorldList() { return m_WorldServerList; }
	std::vector<NFMsg::ServerInfo> GetGateList() { return m_GameServerList; }

private:
	std::string m_strAccount;
	std::string m_strKey;
	std::vector<NFMsg::ServerInfo> m_WorldServerList;
	std::vector<NFMsg::ServerInfo> m_GameServerList;
};

#define g_pLoginLogic (CLoginLogic::Instance())

#endif