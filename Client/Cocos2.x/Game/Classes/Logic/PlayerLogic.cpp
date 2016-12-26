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

	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_PROPERTY_INT, this, &CPlayerLogic::OnPropertyInt);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_PROPERTY_FLOAT, this, &CPlayerLogic::OnPropertyFloat);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_PROPERTY_STRING, this, &CPlayerLogic::OnPropertyString);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_PROPERTY_OBJECT, this, &CPlayerLogic::OnPropertyObject);
	
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_RECORD_INT, this, &CPlayerLogic::OnRecordInt);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_RECORD_FLOAT, this, &CPlayerLogic::OnRecordFloat);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_RECORD_STRING, this, &CPlayerLogic::OnRecordString);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_RECORD_OBJECT, this, &CPlayerLogic::OnRecordObject);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_SWAP_ROW, this, &CPlayerLogic::OnSwapRow);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_ADD_ROW, this, &CPlayerLogic::OnAddRow);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_REMOVE_ROW, this, &CPlayerLogic::OnRemoveRow);

	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_OBJECT_RECORD_ENTRY, this, &CPlayerLogic::OnObjectRecordEntry);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_OBJECT_PROPERTY_ENTRY, this, &CPlayerLogic::OnObjectPropertyEntry);

	return true;
}

//--------------------------------------------发消息-------------------------------------------------------------
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

//--------------------------------------------收消息-------------------------------------------------------------
void CPlayerLogic::OnRoleList(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::AckRoleLiteInfoList xMsg;
	if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	// 目前服务器只有一个角色
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

// 移动
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

// 属性
void CPlayerLogic::OnPropertyInt(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ObjectPropertyInt xMsg;
	if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	NF_SHARE_PTR<NFIObject> object = g_pKernelModule->GetObject(NFINetModule::PBToNF(xMsg.player_id()));
	NF_SHARE_PTR<NFIPropertyManager> propertyMgr = object->GetPropertyManager();
	 
	for(int i = 0; i < xMsg.property_list_size(); i++)
	{
		NF_SHARE_PTR<NFIProperty> property = propertyMgr->GetElement(xMsg.property_list(i).property_name());
		if(property == nullptr)
		{
			property = propertyMgr->AddProperty(NFGUID(),  xMsg.property_list(i).property_name(), TDATA_TYPE::TDATA_INT);
		}
		property->SetInt(xMsg.property_list(i).data());
	}
}

void CPlayerLogic::OnPropertyFloat(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ObjectPropertyFloat xMsg;
	if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	NF_SHARE_PTR<NFIObject> object = g_pKernelModule->GetObject(NFINetModule::PBToNF(xMsg.player_id()));
	NF_SHARE_PTR<NFIPropertyManager> propertyMgr = object->GetPropertyManager();
	 
	for(int i = 0; i < xMsg.property_list_size(); i++)
	{
		NF_SHARE_PTR<NFIProperty> property = propertyMgr->GetElement(xMsg.property_list(i).property_name());
		if(property == nullptr)
		{
			property = propertyMgr->AddProperty(NFGUID(),  xMsg.property_list(i).property_name(), TDATA_TYPE::TDATA_FLOAT);
		}
		property->SetFloat(xMsg.property_list(i).data());
	}
}

void CPlayerLogic::OnPropertyString(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ObjectPropertyString xMsg;
	if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	NF_SHARE_PTR<NFIObject> object = g_pKernelModule->GetObject(NFINetModule::PBToNF(xMsg.player_id()));
	NF_SHARE_PTR<NFIPropertyManager> propertyMgr = object->GetPropertyManager();
	 
	for(int i = 0; i < xMsg.property_list_size(); i++)
	{
		NF_SHARE_PTR<NFIProperty> property = propertyMgr->GetElement(xMsg.property_list(i).property_name());
		if(property == nullptr)
		{
			property = propertyMgr->AddProperty(NFGUID(),  xMsg.property_list(i).property_name(), TDATA_TYPE::TDATA_STRING);
		}
		property->SetString(xMsg.property_list(i).data());
	}
}

void CPlayerLogic::OnPropertyObject(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ObjectPropertyObject xMsg;
	if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	NF_SHARE_PTR<NFIObject> object = g_pKernelModule->GetObject(NFINetModule::PBToNF(xMsg.player_id()));
	NF_SHARE_PTR<NFIPropertyManager> propertyMgr = object->GetPropertyManager();
	 
	for(int i = 0; i < xMsg.property_list_size(); i++)
	{
		NF_SHARE_PTR<NFIProperty> property = propertyMgr->GetElement(xMsg.property_list(i).property_name());
		if(property == nullptr)
		{
			property = propertyMgr->AddProperty(NFGUID(),  xMsg.property_list(i).property_name(), TDATA_TYPE::TDATA_OBJECT);
		}
		property->SetObject(NFINetModule::PBToNF(xMsg.property_list(i).data()));
	}
}

// 数据
void CPlayerLogic::OnRecordInt(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ObjectRecordInt xMsg;
	if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	NF_SHARE_PTR<NFIObject> object = g_pKernelModule->GetObject(NFINetModule::PBToNF(xMsg.player_id()));
	NF_SHARE_PTR<NFIRecordManager> recordMgr = object->GetRecordManager();
	 
	for(int i = 0; i < xMsg.property_list_size(); i++)
	{
		NF_SHARE_PTR<NFIRecord> record = recordMgr->GetElement(xMsg.record_name());

		const NFMsg::RecordInt &recordInt = xMsg.property_list(i);
		record->SetInt(recordInt.row(), recordInt.col(), recordInt.data());
	}
}

void CPlayerLogic::OnRecordFloat(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ObjectRecordFloat xMsg;
	if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	NF_SHARE_PTR<NFIObject> object = g_pKernelModule->GetObject(NFINetModule::PBToNF(xMsg.player_id()));
	NF_SHARE_PTR<NFIRecordManager> recordMgr = object->GetRecordManager();
	 
	for(int i = 0; i < xMsg.property_list_size(); i++)
	{
		NF_SHARE_PTR<NFIRecord> record = recordMgr->GetElement(xMsg.record_name());

		const NFMsg::RecordFloat &recordFloat = xMsg.property_list(i);
		record->SetFloat(recordFloat.row(), recordFloat.col(), recordFloat.data());
	}
}

void CPlayerLogic::OnRecordString(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ObjectRecordString xMsg;
	if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	NF_SHARE_PTR<NFIObject> object = g_pKernelModule->GetObject(NFINetModule::PBToNF(xMsg.player_id()));
	NF_SHARE_PTR<NFIRecordManager> recordMgr = object->GetRecordManager();
	 
	for(int i = 0; i < xMsg.property_list_size(); i++)
	{
		NF_SHARE_PTR<NFIRecord> record = recordMgr->GetElement(xMsg.record_name());

		const NFMsg::RecordString &recordString = xMsg.property_list(i);
		record->SetString(recordString.row(), recordString.col(), recordString.data());
	}
}

void CPlayerLogic::OnRecordObject(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ObjectRecordObject xMsg;
	if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	NF_SHARE_PTR<NFIObject> object = g_pKernelModule->GetObject(NFINetModule::PBToNF(xMsg.player_id()));
	NF_SHARE_PTR<NFIRecordManager> recordMgr = object->GetRecordManager();
	 
	for(int i = 0; i < xMsg.property_list_size(); i++)
	{
		NF_SHARE_PTR<NFIRecord> record = recordMgr->GetElement(xMsg.record_name());

		const NFMsg::RecordObject &recordObject = xMsg.property_list(i);
		record->SetObject(recordObject.row(), recordObject.col(), NFINetModule::PBToNF(recordObject.data()));
	}
}

void CPlayerLogic::OnSwapRow(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ObjectRecordSwap xMsg;
	if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	NF_SHARE_PTR<NFIObject> object = g_pKernelModule->GetObject(NFINetModule::PBToNF(xMsg.player_id()));
	NF_SHARE_PTR<NFIRecordManager> recordMgr = object->GetRecordManager();
	 
	NF_SHARE_PTR<NFIRecord> record = recordMgr->GetElement(xMsg.origin_record_name());
	record->SwapRowInfo(xMsg.row_origin(), xMsg.row_target());
}

void CPlayerLogic::OnAddRow(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ObjectRecordAddRow xMsg;
	if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}
	
	NF_SHARE_PTR<NFIObject> object = g_pKernelModule->GetObject(NFINetModule::PBToNF(xMsg.player_id()));
	NF_SHARE_PTR<NFIRecordManager> recordMgr = object->GetRecordManager();

	for(int i = 0; i < xMsg.row_data_size(); i++)
	{
		AddRecord(object, xMsg.record_name(), xMsg.row_data(i));
	}
}

void CPlayerLogic::OnRemoveRow(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ObjectRecordRemove xMsg;
	if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}
	
	NF_SHARE_PTR<NFIObject> object = g_pKernelModule->GetObject(NFINetModule::PBToNF(xMsg.player_id()));
	NF_SHARE_PTR<NFIRecordManager> recordMgr = object->GetRecordManager();

	for(int i = 0; i < xMsg.remove_row_size(); i++)
	{
		NF_SHARE_PTR<NFIRecord> record = recordMgr->GetElement(xMsg.record_name());
		record->Remove(xMsg.remove_row(i));
	}
}

void CPlayerLogic::OnObjectRecordEntry(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::MultiObjectRecordList xMsg;
	if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	for(int i = 0; i < xMsg.multi_player_record_size(); i++)
	{
		const NFMsg::ObjectRecordList &data = xMsg.multi_player_record(i);
		NFGUID id = NFINetModule::PBToNF(data.player_id());

		NF_SHARE_PTR<NFIObject> object = g_pKernelModule->GetObject(id);
		for(int j = 0; j < data.record_list_size(); j++)
		{
			const NFMsg::ObjectRecordBase &objectRecord = data.record_list(j);
			const std::string &strRecordName = data.record_list(j).record_name();
			for(int k = 0; k < objectRecord.row_struct_size(); k++)
			{
				AddRecord(object, strRecordName, objectRecord.row_struct(k));
			}
		}
	}
}
void CPlayerLogic::OnObjectPropertyEntry(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::MultiObjectPropertyList xMsg;
	if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	for(int i = 0; i < xMsg.multi_player_property_size(); i++)
	{
		const NFMsg::ObjectPropertyList &data = xMsg.multi_player_property(i);
		NFGUID id = NFINetModule::PBToNF(data.player_id());
		
		NF_SHARE_PTR<NFIObject> object = g_pKernelModule->GetObject(id);
		NF_SHARE_PTR<NFIPropertyManager> propertyMgr = object->GetPropertyManager();
		
		for(int j = 0; j < data.property_float_list_size(); j++)
		{
			NF_SHARE_PTR<NFIProperty> property = propertyMgr->GetElement(data.property_float_list(j).property_name());
			if(property == nullptr)
			{
				property = propertyMgr->AddProperty(NFGUID(),  data.property_float_list(j).property_name(), TDATA_TYPE::TDATA_FLOAT);
			}
			property->SetFloat(data.property_float_list(j).data());
		}
		for(int j = 0; j < data.property_int_list_size(); j++)
		{
			NF_SHARE_PTR<NFIProperty> property = propertyMgr->GetElement(data.property_int_list(j).property_name());
			if(property == nullptr)
			{
				property = propertyMgr->AddProperty(NFGUID(),  data.property_int_list(j).property_name(), TDATA_TYPE::TDATA_INT);
			}
			property->SetInt(data.property_int_list(j).data());
		}
		for(int j = 0; j < data.property_string_list_size(); j++)
		{
			NF_SHARE_PTR<NFIProperty> property = propertyMgr->GetElement(data.property_string_list(j).property_name());
			if(property == nullptr)
			{
				property = propertyMgr->AddProperty(NFGUID(),  data.property_string_list(j).property_name(), TDATA_TYPE::TDATA_STRING);
			}
			property->SetString(data.property_string_list(j).data());
		}
		for(int j = 0; j < data.property_object_list_size(); j++)
		{
			NF_SHARE_PTR<NFIProperty> property = propertyMgr->GetElement(data.property_object_list(j).property_name());
			if(property == nullptr)
			{
				property = propertyMgr->AddProperty(NFGUID(),  data.property_object_list(j).property_name(), TDATA_TYPE::TDATA_OBJECT);
			}
			property->SetObject(NFINetModule::PBToNF(data.property_object_list(j).data()));
		}
		for(int j = 0; j < data.property_vector2_list_size(); j++)
		{
			NF_SHARE_PTR<NFIProperty> property = propertyMgr->GetElement(data.property_vector2_list(j).property_name());
			if(property == nullptr)
			{
				property = propertyMgr->AddProperty(NFGUID(),  data.property_vector2_list(j).property_name(), TDATA_TYPE::TDATA_VECTOR2);
			}
			property->SetVector2(NFINetModule::PBToNF(data.property_vector2_list(j).data()));
		}
		for(int j = 0; j < data.property_vector3_list_size(); j++)
		{
			NF_SHARE_PTR<NFIProperty> property = propertyMgr->GetElement(data.property_vector3_list(j).property_name());
			if(property == nullptr)
			{
				property = propertyMgr->AddProperty(NFGUID(),  data.property_vector3_list(j).property_name(), TDATA_TYPE::TDATA_VECTOR3);
			}
			property->SetVector3(NFINetModule::PBToNF(data.property_vector3_list(j).data()));
		}
	}
}

void CPlayerLogic::AddRecord(const NF_SHARE_PTR<NFIObject>& object, const std::string &strRecordName, const NFMsg::RecordAddRowStruct &data)
{
	NF_SHARE_PTR<NFIRecordManager> recordMgr = object->GetRecordManager();

	NF_SHARE_PTR<NFIDataList> varData = NF_SHARE_PTR<NFIDataList>(NF_NEW NFCDataList());
	NF_SHARE_PTR<NFIDataList> varInit = NF_SHARE_PTR<NFIDataList>(NF_NEW NFCDataList());
	NF_SHARE_PTR<NFIDataList> varTag = NF_SHARE_PTR<NFIDataList>(NF_NEW NFCDataList());
	for(int j = 0; j < data.record_float_list_size(); j++)
	{
		varInit->AddFloat(0);
		varData->AddFloat(data.record_float_list(j).data());
	}
	for(int j = 0; j < data.record_int_list_size(); j++)
	{
		varInit->AddInt(0);
		varData->AddInt(data.record_int_list(j).data());
	}
	for(int j = 0; j < data.record_string_list_size(); j++)
	{
		varInit->AddString("");
		varData->AddString(data.record_string_list(j).data());
	}
	for(int j = 0; j < data.record_object_list_size(); j++)
	{
		varInit->AddObject(NFGUID());
		varData->AddObject(NFINetModule::PBToNF(data.record_object_list(j).data()));
	}
	for(int j = 0; j < data.record_vector2_list_size(); j++)
	{
		varInit->AddVector2(NFVector2());
		varData->AddVector2(NFINetModule::PBToNF(data.record_vector2_list(j).data()));
	}
	for(int j = 0; j < data.record_vector3_list_size(); j++)
	{
		varInit->AddVector3(NFVector3());
		varData->AddVector3(NFINetModule::PBToNF(data.record_vector3_list(j).data()));
	}

	NF_SHARE_PTR<NFIRecord> record = recordMgr->GetElement(strRecordName);
	if(record == nullptr)
	{
		record = recordMgr->AddRecord(NFGUID(), strRecordName, varInit, varTag, 255);
	}
	record->AddRow(data.row(), *varData);
}
