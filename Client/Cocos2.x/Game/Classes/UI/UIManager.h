/********************************************************************
	purpose:	�������ģ��ӿڵ�ʵ����
	����ֻҪ�֞� 
	ϵ�y���
	��Ԓ�����
	��Ϣ����
	���ܽ���
	������UI
	��������
*********************************************************************/
#ifndef _UIManager_H_
#define _UIManager_H_

#include "UIDialog.h"
#include "NFComm/NFCore/NFSingleton.hpp"
#include "NFComm/NFPluginModule/NFIModule.h"

class UIManager : public NFIModule, public NFSingleton<UIManager>
{
public:
	UIManager() {}
	~UIManager() {}

    UIManager(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool ReadyExecute();
    virtual bool Execute();

    virtual bool AfterInit();

	// ---------  ------------
    virtual CCNode* RootNode();
    
    virtual UIDialog* ShowDialog(UIDialog *pDialog, bool bPushToHistory = true);
    virtual void CloseDialog(UIDialog *pDialog);
	virtual void ChangeBackDialog();
	virtual bool HasBackDialog();
	virtual void CloseDialog();

public:
	void onBackKeyClicked();
	void onMenuKeyClicked();

private:
    std::vector<UIDialog*> m_vecDialog;
	
    // UI�ĸ����c
	CC_SYNTHESIZE_READONLY(CCNode*, m_pRootNode, RootNode);
	CC_SYNTHESIZE_READONLY(CCNode*, m_pPanelNode, PanelNode);
	CC_SYNTHESIZE_READONLY(CCNode*, m_pDialogNode, DialogNode);
	CC_SYNTHESIZE_READONLY(CCNode*, m_pBoxNode, BoxNode);
	CC_SYNTHESIZE_READONLY(CCNode*, m_pEffectNode, EffectNode);

private:
    CCArray *m_pDialogQueue;
    UIDialog *m_pCurrentDialog;
};

#define g_pUIManager (UIManager::Instance())

// ��������߼��޹صĴ�UI�߼���
template<typename T>
class IUniqueDialog: public UIDialog
{	
public:
	// ��ʾ����(����û����ʱ�������ڲ���ʾ���Ѿ�������ֻ��ʾ)
	static void showUI(bool bPushToHistory = true, const void *customData = nullptr) 
	{ 
		IUniqueDialog **p = _getDialog();
		if (!*p) {
			*p = new T;
			(*p)->initLayout();
			(*p)->autorelease();
		}
		
		(*p)->setUserData((void*)customData);
		g_pUIManager->ShowDialog(*p, bPushToHistory);
	}
	static void closeUI(const void *customData = nullptr) 
	{ 
		IUniqueDialog **p = _getDialog();
		if (*p) {
			g_pUIManager->CloseDialog(*p);
		}
	}
	IUniqueDialog() { // �������ⲿ�ֶ�new
		*_getDialog() = this;
	}
	virtual ~IUniqueDialog() { // �������ⲿ�ֶ�delete
		if(*_getDialog() == this)
			*_getDialog() = nullptr;
	}

private:
	static IUniqueDialog** _getDialog() {
		static T* _instance;
		return (IUniqueDialog**)(&_instance);
	}
};

#endif
