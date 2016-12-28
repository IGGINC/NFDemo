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

	// ������Ϣ
public:
	void RequireRoleList();
	void RequireCreateRole(string strRoleName, int byCareer, int bySex);
	void RequireEnterGameServer(int nRoleIndex);
	void RequireMove(NFVector3 pos);

	// ������Ϣ
private:
	void OnRoleList(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	// ������뿪
	void OnObjectEntry(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnObjectLeave(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	// �ƶ�
	void OnObjectMove(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnObjectJump(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

private:
	void AddRecord(const NF_SHARE_PTR<NFIObject>& object, const std::string &strRecordName, const NFMsg::RecordAddRowStruct &data);

public:
	std::vector<NFMsg::RoleLiteInfo> GetRoleList() { return m_RoleList; }

private:
	std::vector<NFMsg::RoleLiteInfo> m_RoleList;
};

#define g_pPlayerLogic (CPlayerLogic::Instance())

#endif