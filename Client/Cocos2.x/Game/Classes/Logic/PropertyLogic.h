// -------------------------------------------------------------------------
//    @FileName			:    CPropertyLogic.h
//    @Author           :    Johance
//    @Date             :    2012-12-27
//    @Module           :    CPropertyLogic
//
// -------------------------------------------------------------------------

#ifndef NFC_PropertyLogic_MODULE_H
#define NFC_PropertyLogic_MODULE_H

#include "LogicBase.h"

enum PropertyLogicEvent
{
};

class CPropertyLogic
    : public CLogicBase, public NFSingleton<CPropertyLogic>
{
public:
	CPropertyLogic() {};
	virtual ~CPropertyLogic() {};
    CPropertyLogic(NFIPluginManager* p)
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
	// ������Ϣ
private:
	// ����
	void OnPropertyInt(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnPropertyFloat(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnPropertyString(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnPropertyObject(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnObjectPropertyEntry(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
};

#define g_pPropertyLogic (CPropertyLogic::Instance())

#endif