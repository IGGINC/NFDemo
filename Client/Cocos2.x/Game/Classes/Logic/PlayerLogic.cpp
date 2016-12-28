// -------------------------------------------------------------------------
//    @FileName			:    CPlayerLogic.cpp
//    @Author           :    Johance
//    @Date             :    2012-12-15
//    @Module           :    NFCPlayerLogic
//
// -------------------------------------------------------------------------

#include "PlayerLogic.h"
#include "../../../../Common/NFMessageDefine/NFMsgDefine.h"
#include "../../../../Common/NFMessageDefine/NFProtocolDefine.hpp"
#include "NetLogic.h"
#include "LoginLogic.h"
#include "PropertyLogic.h"

bool CPlayerLogic::Init()
{
	return true;
}

bool CPlayerLogic::Shut()
{
	return true;
}

bool CPlayerLogic::ReadyExecute()
{
	return true;
}

bool CPlayerLogic::Execute()
{


	return true;
}

bool CPlayerLogic::AfterInit()
{
	CLogicBase::AfterInit();
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_ROLE_LIST, this, &CPlayerLogic::OnRoleList);

	
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_OBJECT_ENTRY, this, &CPlayerLogic::OnObjectEntry);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_OBJECT_LEAVE, this, &CPlayerLogic::OnObjectLeave);

	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_MOVE, this, &CPlayerLogic::OnObjectMove);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_MOVE_IMMUNE, this, &CPlayerLogic::OnObjectJump);

	return true;
}

//--------------------------------------------����Ϣ-------------------------------------------------------------
void CPlayerLogic::RequireRoleList()
{
	m_RoleList.clear();

	NFMsg::ReqRoleList xMsg;
	xMsg.set_account(g_pLoginLogic->GetAccount());
	xMsg.set_game_id(g_pLoginLogic->GetServerID());
	g_pNetLogic->SendToServerByPB(NFMsg::EGameMsgID::EGMI_REQ_ROLE_LIST, xMsg);
}

void CPlayerLogic::RequireCreateRole(string strRoleName, int byCareer, int bySex)
{
	NFMsg::ReqCreateRole xMsg;
	xMsg.set_account(g_pLoginLogic->GetAccount());	
	xMsg.set_race(0);

	xMsg.set_noob_name(strRoleName);
	xMsg.set_career(byCareer);
	xMsg.set_sex(bySex);
	xMsg.set_game_id(g_pLoginLogic->GetServerID());
	g_pNetLogic->SendToServerByPB(NFMsg::EGameMsgID::EGMI_REQ_CREATE_ROLE, xMsg);
}

void CPlayerLogic::RequireEnterGameServer(int nRoleIndex)
{
	if(nRoleIndex >= m_RoleList.size())
	{
		NFASSERT(0, "out of range", __FILE__, __FUNCTION__);
		return ;
	}

	NFMsg::ReqEnterGameServer xMsg;
	xMsg.set_account(g_pLoginLogic->GetAccount());	
	xMsg.set_game_id(g_pLoginLogic->GetServerID());

	xMsg.set_name(m_RoleList[nRoleIndex].noob_name());
	*xMsg.mutable_id() = m_RoleList[nRoleIndex].id();

	g_pNetLogic->SendToServerByPB(NFMsg::EGameMsgID::EGMI_REQ_ENTER_GAME, xMsg);
}

void CPlayerLogic::RequireMove(NFVector3 pos)
{
	

	//if(nRoleIndex >= m_RoleList.size())
	//{
	//	NFASSERT(0, "out of range", __FILE__, __FUNCTION__);
	//	return ;
	//}

	//NFMsg::ReqMoveEnterGameServer xMsg;
	//xMsg.set_account(g_pLoginLogic->GetAccount());	
	//xMsg.set_game_id(g_pLoginLogic->GetServerID());

	//xMsg.set_name(m_RoleList[nRoleIndex].noob_name());
	//*xMsg.mutable_id() = m_RoleList[nRoleIndex].id();

	//g_pNetLogic->SendToServerByPB(NFMsg::EGameMsgID::EGMI_REQ_ENTER_GAME, xMsg);
}
//--------------------------------------------����Ϣ-------------------------------------------------------------
void CPlayerLogic::OnRoleList(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::AckRoleLiteInfoList xMsg;
	if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	// Ŀǰ������ֻ��һ����ɫ
	m_RoleList.clear();
	for(int i = 0; i < xMsg.char_data_size(); i++)
	{
		m_RoleList.push_back(xMsg.char_data(i));
	}

	DoEvent(E_PlayerEvent_RoleList, NFCDataList());
}

void CPlayerLogic::OnObjectEntry(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::AckPlayerEntryList xMsg;
	if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}
	
	for(int i = 0; i < xMsg.object_list_size(); i++)
	{
		const NFMsg::PlayerEntryInfo info =  xMsg.object_list(i);

		NFCDataList var;
		var.Add("X");
		var.AddFloat(info.x());
		var.Add("Y");
		var.AddFloat(info.y());
		var.Add("Z");
		var.AddFloat(info.z());
		g_pKernelModule->CreateObject(NFINetModule::PBToNF(info.object_guid()), info.scene_id(), 0, info.class_id(), info.config_id(), var);
	}
}

void CPlayerLogic::OnObjectLeave(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::AckPlayerLeaveList xMsg;
	if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}
	
	for(int i = 0; i < xMsg.object_list_size(); i++)
	{
		g_pKernelModule->DestroyObject(NFINetModule::PBToNF(xMsg.object_list(i)));
	}
}

// �ƶ�
void CPlayerLogic::OnObjectMove(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ReqAckPlayerMove xMsg;
	if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}
		
	float fMove = g_pKernelModule->GetPropertyInt(NFINetModule::PBToNF(xMsg.mover()), "MOVE_SPEED")/10000.0f;
	NFCDataList var;
	var.AddObject(NFINetModule::PBToNF(xMsg.mover()));
	var.AddFloat(fMove);
	const NFMsg::Position &pos = xMsg.target_pos().Get(0);
	var.AddVector3(NFVector3(pos.x(), pos.y(), pos.z()));
	DoEvent(E_PlayerEvent_PlayerMove, var);
}

void CPlayerLogic::OnObjectJump(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ReqAckPlayerMove xMsg;
	if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}
		
	float fMove = g_pKernelModule->GetPropertyInt(NFINetModule::PBToNF(xMsg.mover()), "MOVE_SPEED")/10000.0f;
	NFCDataList var;
	var.AddObject(NFINetModule::PBToNF(xMsg.mover()));
	var.AddFloat(fMove);
	const NFMsg::Position &pos = xMsg.target_pos().Get(0);
	var.AddVector3(NFVector3(pos.x(), pos.y(), pos.z()));

	DoEvent(E_PlayerEvent_PlayerJump, var);
}
