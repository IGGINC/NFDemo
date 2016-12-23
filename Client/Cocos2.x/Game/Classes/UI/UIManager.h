/********************************************************************
	purpose:	介面管理模块接口的实现类
	介面只要分為 
	系統類介面
	對話框介面
	信息介面
	功能介面
	主介面UI
	場景介面
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
	
    // UI的根節點
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

// 与管理器逻辑无关的纯UI逻辑类
template<typename T>
class IUniqueDialog: public UIDialog
{	
public:
	// 显示窗口(窗口没创建时创建窗口并显示，已经创建则只显示)
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
	IUniqueDialog() { // 不允许外部手动new
		*_getDialog() = this;
	}
	virtual ~IUniqueDialog() { // 不允许外部手动delete
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
