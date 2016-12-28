// -------------------------------------------------------------------------
//    @FileName			:    CRecordLogic.h
//    @Author           :    Johance
//    @Date             :    2012-12-27
//    @Module           :    CRecordLogic
//
// -------------------------------------------------------------------------

#ifndef RECORDLOGIC_MODULE_H
#define RECORDLOGIC_MODULE_H

#include "LogicBase.h"

enum RecordLogicEvent
{
};

class CRecordLogic
    : public CLogicBase, public NFSingleton<CRecordLogic>
{
public:
	CRecordLogic() {};
	virtual ~CRecordLogic() {};
    CRecordLogic(NFIPluginManager* p)
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

	// 接收消息
private:
	void OnRecordInt(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnRecordFloat(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnRecordString(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnRecordObject(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnSwapRow(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnAddRow(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnRemoveRow(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);	
	void OnObjectRecordEntry(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

private:
	void AddRecord(const NF_SHARE_PTR<NFIObject>& object, const std::string &strRecordName, const NFMsg::RecordAddRowStruct &data);
};

#define g_pRecordLogic (CRecordLogic::Instance())

#endif