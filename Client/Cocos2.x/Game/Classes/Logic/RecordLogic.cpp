// -------------------------------------------------------------------------
//    @FileName			:    CRecordLogic.cpp
//    @Author           :    Johance
//    @Date             :    2012-12-27
//    @Module           :    CRecordLogic
//
// -------------------------------------------------------------------------

#include "RecordLogic.h"
#include "../../../../Common/NFMessageDefine/NFMsgDefine.h"
#include "../../../../Common/NFMessageDefine/NFProtocolDefine.hpp"
#include "NetLogic.h"
#include "LoginLogic.h"

bool CRecordLogic::Init()
{
	return true;
}

bool CRecordLogic::Shut()
{
	return true;
}

bool CRecordLogic::ReadyExecute()
{
	return true;
}

bool CRecordLogic::Execute()
{


	return true;
}

bool CRecordLogic::AfterInit()
{
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_RECORD_INT, this, &CRecordLogic::OnRecordInt);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_RECORD_FLOAT, this, &CRecordLogic::OnRecordFloat);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_RECORD_STRING, this, &CRecordLogic::OnRecordString);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_RECORD_OBJECT, this, &CRecordLogic::OnRecordObject);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_SWAP_ROW, this, &CRecordLogic::OnSwapRow);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_ADD_ROW, this, &CRecordLogic::OnAddRow);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_REMOVE_ROW, this, &CRecordLogic::OnRemoveRow);

	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_OBJECT_RECORD_ENTRY, this, &CRecordLogic::OnObjectRecordEntry);

	return true;
}

//--------------------------------------------发消息-------------------------------------------------------------
//--------------------------------------------收消息-------------------------------------------------------------
void CRecordLogic::OnRecordInt(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

void CRecordLogic::OnRecordFloat(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

void CRecordLogic::OnRecordString(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

void CRecordLogic::OnRecordObject(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

void CRecordLogic::OnSwapRow(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

void CRecordLogic::OnAddRow(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

void CRecordLogic::OnRemoveRow(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

void CRecordLogic::OnObjectRecordEntry(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

void CRecordLogic::AddRecord(const NF_SHARE_PTR<NFIObject>& object, const std::string &strRecordName, const NFMsg::RecordAddRowStruct &data)
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
