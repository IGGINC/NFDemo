// -------------------------------------------------------------------------
//    @FileName			:    CPropertyLogic.cpp
//    @Author           :    Johance
//    @Date             :    2012-12-27
//    @Module           :    CPropertyLogic
//
// -------------------------------------------------------------------------

#include "PropertyLogic.h"
#include "../../../../Common/NFMessageDefine/NFMsgDefine.h"
#include "../../../../Common/NFMessageDefine/NFProtocolDefine.hpp"
#include "NetLogic.h"
#include "LoginLogic.h"

bool CPropertyLogic::Init()
{
	return true;
}

bool CPropertyLogic::Shut()
{
	return true;
}

bool CPropertyLogic::ReadyExecute()
{
	return true;
}

bool CPropertyLogic::Execute()
{


	return true;
}

bool CPropertyLogic::AfterInit()
{
	CLogicBase::AfterInit();

	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_PROPERTY_INT, this, &CPropertyLogic::OnPropertyInt);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_PROPERTY_FLOAT, this, &CPropertyLogic::OnPropertyFloat);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_PROPERTY_STRING, this, &CPropertyLogic::OnPropertyString);
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_PROPERTY_OBJECT, this, &CPropertyLogic::OnPropertyObject);

	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_OBJECT_PROPERTY_ENTRY, this, &CPropertyLogic::OnObjectPropertyEntry);

	return true;
}

//--------------------------------------------����Ϣ-------------------------------------------------------------
//--------------------------------------------����Ϣ-------------------------------------------------------------
// ����
void CPropertyLogic::OnPropertyInt(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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
		NF_SHARE_PTR<NFIProperty> prop = propertyMgr->GetElement(xMsg.property_list(i).property_name());
		if(prop == nullptr)
		{
			prop = propertyMgr->AddProperty(NFGUID(),  xMsg.property_list(i).property_name(), TDATA_TYPE::TDATA_INT);
		}
		prop->SetInt(xMsg.property_list(i).data());
	}
}

void CPropertyLogic::OnPropertyFloat(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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
		NF_SHARE_PTR<NFIProperty> prop = propertyMgr->GetElement(xMsg.property_list(i).property_name());
		if(prop == nullptr)
		{
			prop = propertyMgr->AddProperty(NFGUID(),  xMsg.property_list(i).property_name(), TDATA_TYPE::TDATA_FLOAT);
		}
		prop->SetFloat(xMsg.property_list(i).data());
	}
}

void CPropertyLogic::OnPropertyString(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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
		NF_SHARE_PTR<NFIProperty> prop = propertyMgr->GetElement(xMsg.property_list(i).property_name());
		if(prop == nullptr)
		{
			prop = propertyMgr->AddProperty(NFGUID(),  xMsg.property_list(i).property_name(), TDATA_TYPE::TDATA_STRING);
		}
		prop->SetString(xMsg.property_list(i).data());
	}
}

void CPropertyLogic::OnPropertyObject(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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
		NF_SHARE_PTR<NFIProperty> prop = propertyMgr->GetElement(xMsg.property_list(i).property_name());
		if(prop == nullptr)
		{
			prop = propertyMgr->AddProperty(NFGUID(),  xMsg.property_list(i).property_name(), TDATA_TYPE::TDATA_OBJECT);
		}
		prop->SetObject(NFINetModule::PBToNF(xMsg.property_list(i).data()));
	}
}

void CPropertyLogic::OnObjectPropertyEntry(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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
			NF_SHARE_PTR<NFIProperty> prop = propertyMgr->GetElement(data.property_float_list(j).property_name());
			if(prop == nullptr)
			{
				prop = propertyMgr->AddProperty(NFGUID(),  data.property_float_list(j).property_name(), TDATA_TYPE::TDATA_FLOAT);
			}
			prop->SetFloat(data.property_float_list(j).data());
		}
		for(int j = 0; j < data.property_int_list_size(); j++)
		{
			NF_SHARE_PTR<NFIProperty> prop = propertyMgr->GetElement(data.property_int_list(j).property_name());
			if(prop == nullptr)
			{
				prop = propertyMgr->AddProperty(NFGUID(),  data.property_int_list(j).property_name(), TDATA_TYPE::TDATA_INT);
			}
			prop->SetInt(data.property_int_list(j).data());
		}
		for(int j = 0; j < data.property_string_list_size(); j++)
		{
			NF_SHARE_PTR<NFIProperty> prop = propertyMgr->GetElement(data.property_string_list(j).property_name());
			if(prop == nullptr)
			{
				prop = propertyMgr->AddProperty(NFGUID(),  data.property_string_list(j).property_name(), TDATA_TYPE::TDATA_STRING);
			}
			prop->SetString(data.property_string_list(j).data());
		}
		for(int j = 0; j < data.property_object_list_size(); j++)
		{
			NF_SHARE_PTR<NFIProperty> prop = propertyMgr->GetElement(data.property_object_list(j).property_name());
			if(prop == nullptr)
			{
				prop = propertyMgr->AddProperty(NFGUID(),  data.property_object_list(j).property_name(), TDATA_TYPE::TDATA_OBJECT);
			}
			prop->SetObject(NFINetModule::PBToNF(data.property_object_list(j).data()));
		}
		for(int j = 0; j < data.property_vector2_list_size(); j++)
		{
			NF_SHARE_PTR<NFIProperty> prop = propertyMgr->GetElement(data.property_vector2_list(j).property_name());
			if(prop == nullptr)
			{
				prop = propertyMgr->AddProperty(NFGUID(),  data.property_vector2_list(j).property_name(), TDATA_TYPE::TDATA_VECTOR2);
			}
			prop->SetVector2(NFINetModule::PBToNF(data.property_vector2_list(j).data()));
		}
		for(int j = 0; j < data.property_vector3_list_size(); j++)
		{
			NF_SHARE_PTR<NFIProperty> prop = propertyMgr->GetElement(data.property_vector3_list(j).property_name());
			if(prop == nullptr)
			{
				prop = propertyMgr->AddProperty(NFGUID(),  data.property_vector3_list(j).property_name(), TDATA_TYPE::TDATA_VECTOR3);
			}
			prop->SetVector3(NFINetModule::PBToNF(data.property_vector3_list(j).data()));
		}
	}
}
