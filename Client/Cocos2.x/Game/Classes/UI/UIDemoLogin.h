#ifndef _UIDemoLogin_H__
#define _UIDemoLogin_H__

#include "UIManager.h"

class UIDemoLogin : public IUniqueDialog<UIDemoLogin>
{
public:
	UIDemoLogin();
	~UIDemoLogin();
	virtual const char* getUIName() { return "DemoLogin"; }
	virtual const char* getUIPath() { return "DemoLogin/DemoLogin.ExportJson"; }
	virtual int getUIPriority() { return 0; }

    virtual bool initLayout();
	virtual void initData(void *customData);

private:
	void OnSocketEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);
	int OnLoginEvent(const int, const NFIDataList&);

private:
	void onLoginTouch(CCObject *pObject , ui::TouchEventType touchType);
	
private:
	ui::UIButton *m_pLoginButton;
	ui::UITextField *m_pUserName;
	ui::UITextField *m_pUserPWD;

};

#endif // __HELLOWORLD_SCENE_H__
