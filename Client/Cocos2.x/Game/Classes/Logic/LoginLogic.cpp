// -------------------------------------------------------------------------
//    @FileName			:    CLoginLogic.cpp
//    @Author           :    Johance
//    @Date             :    2012-12-15
//    @Module           :    NFCLoginLogic
//
// -------------------------------------------------------------------------

#include "LoginLogic.h"
#include "../../../../Common/NFMessageDefine/NFMsgDefine.h"
#include "../../../../Common/NFMessageDefine/NFProtocolDefine.hpp"
#include "NetLogic.h"

bool CLoginLogic::Init()
{
    return true;
}

bool CLoginLogic::Shut()
{
    return true;
}

bool CLoginLogic::ReadyExecute()
{
    return true;
}

bool CLoginLogic::Execute()
{


    return true;
}

bool CLoginLogic::AfterInit()
{
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_LOGIN, this, &CLoginLogic::OnLoginProcess);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_WORLD_LIST, this, &CLoginLogic::OnWorldList);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_CONNECT_WORLD, this, &CLoginLogic::OnConnectWorld);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_CONNECT_KEY, this, &CLoginLogic::OnConnectKey);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_SELECT_SERVER, this, &CLoginLogic::OnSelectServer);

    return true;
}
//--------------------------------------------发消息-------------------------------------------------------------
void CLoginLogic::LoginPB(const std::string &strAccount, const std::string &strPwd, const std::string &strKey)
{
	NFMsg::ReqAccountLogin xMsg;
	xMsg.set_security_code("");
	xMsg.set_signbuff("");
	xMsg.set_clientversion(1);
	xMsg.set_loginmode(0);
	xMsg.set_clientip(0);
	xMsg.set_clientip(0);
	xMsg.set_clientmac(0);
	xMsg.set_device_info("");
	xMsg.set_extra_info("");

	m_strAccount = strAccount;
	m_strKey = strKey;

	xMsg.set_account(strAccount);
	xMsg.set_password(strPwd);

	g_pNetLogic->SendToServerByPB(NFMsg::EGameMsgID::EGMI_REQ_LOGIN, xMsg);
}

void CLoginLogic::RequireWorldList()
{
	NFMsg::ReqServerList xMsg;
	xMsg.set_type(NFMsg::RSLT_WORLD_SERVER);
	g_pNetLogic->SendToServerByPB(NFMsg::EGameMsgID::EGMI_REQ_WORLD_LIST, xMsg);
}

void CLoginLogic::RequireConnectWorld(int nWorldID)
{
	NFMsg::ReqConnectWorld xMsg;
	xMsg.set_world_id(nWorldID);
	g_pNetLogic->SendToServerByPB(NFMsg::EGameMsgID::EGMI_REQ_CONNECT_WORLD, xMsg);
}

void CLoginLogic::RequireVerifyWorldKey(const std::string &strAccount, const std::string &strKey)
{
	NFMsg::ReqAccountLogin xMsg;
	xMsg.set_signbuff("");
	xMsg.set_clientversion(1);
	xMsg.set_loginmode(0);
	xMsg.set_clientip(0);
	xMsg.set_clientip(0);
	xMsg.set_clientmac(0);
	xMsg.set_device_info("");
	xMsg.set_extra_info("");
	xMsg.set_password("");

	xMsg.set_account(strAccount);
	xMsg.set_security_code(strKey);

	g_pNetLogic->SendToServerByPB(NFMsg::EGameMsgID::EGMI_REQ_CONNECT_KEY, xMsg);
}

void CLoginLogic::RequireServerList()
{
	NFMsg::ReqServerList xMsg;
	xMsg.set_type(NFMsg::RSLT_GAMES_ERVER);
	g_pNetLogic->SendToServerByPB(NFMsg::EGameMsgID::EGMI_REQ_WORLD_LIST, xMsg);
}

void CLoginLogic::RequireSelectServer(int nServerID)
{
	NFMsg::ReqSelectServer xMsg;
	xMsg.set_world_id(nServerID);
	g_pNetLogic->SendToServerByPB(NFMsg::EGameMsgID::EGMI_REQ_SELECT_SERVER, xMsg);
}

//--------------------------------------------收消息-------------------------------------------------------------
void CLoginLogic::OnLoginProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::AckEventResult xMsg;
	if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	if(NFMsg::EGEC_ACCOUNT_SUCCESS == xMsg.event_code())
	{
		CCLOG("Login Success");
		DoEvent(E_LoginEvent_LoginSuccess, NFCDataList());
	}
	else
	{
		CCLOG("Login Failure(%d)", xMsg.event_code());
	}
}

void CLoginLogic::OnWorldList(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::AckServerList xMsg;
	if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	if(xMsg.type() == NFMsg::RSLT_WORLD_SERVER)
	{
		m_WorldServerList.clear();
		m_WorldServerList.resize(xMsg.info_size());
		for(int i = 0; i < xMsg.info_size(); i++)
		{
			m_WorldServerList[i] = xMsg.info(i);
		}
	}
	else if(xMsg.type() == NFMsg::RSLT_GAMES_ERVER)
	{
		m_GameServerList.clear();
		m_GameServerList.resize(xMsg.info_size());
		for(int i = 0; i < xMsg.info_size(); i++)
		{
			m_GameServerList[i] = xMsg.info(i);
		}
	}
	DoEvent(E_LoginEvent_WorldList, NFCDataList());
}

void CLoginLogic::OnConnectWorld(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::AckConnectWorldResult xMsg;
	if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	m_strKey = xMsg.world_key();
	g_pNetLogic->ConnectServer(xMsg.world_ip(), xMsg.world_port());	

	RequireVerifyWorldKey(m_strAccount, m_strKey);
}

void CLoginLogic::OnConnectKey(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::AckEventResult xMsg;
	if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	if(xMsg.event_code() == NFMsg::EGEC_VERIFY_KEY_SUCCESS)
	{
		RequireServerList();
	}
}

void CLoginLogic::OnSelectServer(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::AckEventResult xMsg;
	if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	if(xMsg.event_code() == NFMsg::EGEC_SELECTSERVER_SUCCESS)
	{
		CCLOG("SelectGame SUCCESS");
	}
}
