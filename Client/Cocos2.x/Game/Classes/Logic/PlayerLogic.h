// -------------------------------------------------------------------------
//    @FileName			:    CPlayerLogic.h
//    @Author           :    Johance
//    @Date             :    2012-12-15
//    @Module           :    NFCPlayerLogic
//
// -------------------------------------------------------------------------

#ifndef NFC_PlayerLogic_MODULE_H
#define NFC_PlayerLogic_MODULE_H

#include "LogicBase.h"

enum PlayerLogicEvent
{
	E_PlayerEvent_RoleList,
	E_PlayerEvent_PlayerMove,
	E_PlayerEvent_PlayerJump,
};

class CPlayerLogic
    : public CLogicBase, public NFSingleton<CPlayerLogic>
{
public:
	CPlayerLogic() {};
	virtual ~CPlayerLogic() {};
    CPlayerLogic(NFIPluginManager* p)
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
	void RequireRoleList();
	void RequireCreateRole(string strRoleName, int byCareer, int bySex);
	void RequireEnterGameServer(int nRoleIndex);

	// 接收消息
private:
	void OnRoleList(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	// 进入和离开
	void OnObjectEntry(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnObjectLeave(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	// 移动
	void OnObjectMove(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnObjectJump(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	// 属性
	void OnPropertyInt(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnPropertyFloat(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnPropertyString(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnPropertyObject(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	// 数据
	void OnRecordInt(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnRecordFloat(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnRecordString(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnRecordObject(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnSwapRow(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnAddRow(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnRemoveRow(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	
	void OnObjectRecordEntry(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnObjectPropertyEntry(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

private:
	void AddRecord(const NF_SHARE_PTR<NFIObject>& object, const std::string &strRecordName, const NFMsg::RecordAddRowStruct &data);

public:
	std::vector<NFMsg::RoleLiteInfo> GetRoleList() { return m_RoleList; }

private:
	std::vector<NFMsg::RoleLiteInfo> m_RoleList;
};

#define g_pPlayerLogic (CPlayerLogic::Instance())

#endif