// -------------------------------------------------------------------------
//    @FileName			:    CNetLogic.cpp
//    @Author           :    Johance
//    @Date             :    2012-12-15
//    @Module           :    NFCNetLogic
//
// -------------------------------------------------------------------------

#include "NetLogic.h"
#include "../../../../Common/NFMessageDefine/NFMsgDefine.h"
#include "../../../../Common/NFMessageDefine/NFProtocolDefine.hpp"

bool CNetLogic::Init()
{
    return true;
}

bool CNetLogic::Shut()
{
    return true;
}

bool CNetLogic::ReadyExecute()
{
    return true;
}

bool CNetLogic::Execute()
{


    return true;
}

bool CNetLogic::AfterInit()
{
	CLogicBase::AfterInit();
	g_pNetClientModule->AddEventCallBack(this, &CNetLogic::OnSocketEvent);
	g_pNetClientModule->AddReceiveCallBack(this, &CNetLogic::OnMsgRecive);

	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_EVENT_RESULT, this, &CNetLogic::OnEventResult);

    return true;
}

void CNetLogic::ConnectServer(const std::string strIp, int nPort)
{
	m_listDelayMsg.clear();
	m_bSocketReady = false;
	ConnectData xServerData;

	xServerData.nGameID = 0;
	xServerData.strIP = strIp;
	xServerData.nPort = nPort;

	g_pNetClientModule->AddServer(xServerData);
}

void CNetLogic::SendToServerByPB(const uint16_t nMsgID, google::protobuf::Message& xData)
{
	if(m_bSocketReady)
	{
		g_pNetClientModule->SendToAllServerByPB(nMsgID, xData);
	}
	else
	{
        NFMsg::MsgBase xMsg;
        xData.SerializeToString(xMsg.mutable_msg_data());
        NFMsg::Ident* pPlayerID = xMsg.mutable_player_id();
        *pPlayerID = NFINetModule::NFToPB(NFGUID());

        std::string strMsg;
        xMsg.SerializeToString(&strMsg);
		m_listDelayMsg.push_back(std::make_pair<int, std::string>(nMsgID, strMsg));
	}
}

NFINetClientModule *CNetLogic::GetNetModule()
{	
	if(!g_pNetClientModule)
		g_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	return g_pNetClientModule;
}

void CNetLogic::OnSocketEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
{
    if (eEvent & NF_NET_EVENT_EOF)
    {
        g_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_EOF", "Connection closed", __FUNCTION__, __LINE__);
    }
    else if (eEvent & NF_NET_EVENT_ERROR)
    {
        g_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_ERROR", "Got an error on the connection", __FUNCTION__, __LINE__);
    }
    else if (eEvent & NF_NET_EVENT_TIMEOUT)
    {
        g_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_TIMEOUT", "read timeout", __FUNCTION__, __LINE__);
    }
    else  if (eEvent == NF_NET_EVENT_CONNECTED)
    {
        g_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_CONNECTED", "connectioned success", __FUNCTION__, __LINE__);
		m_bSocketReady = true;

		while(m_listDelayMsg.size() > 0)
		{
			auto msg = m_listDelayMsg.front();

			g_pNetClientModule->SendToAllServer(msg.first, msg.second);

			m_listDelayMsg.pop_front();
		}
    }
}

void CNetLogic::OnEventResult(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::AckEventResult xMsg;
	if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}
}

void CNetLogic::OnMsgRecive(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CCLOG("MsgRecv:%d not Register", nMsgID);
}
