#include "stdafx.h"

#include "UIDemoLogin.h"
#include "Logic\LoginLogic.h"
#include "Logic\NetLogic.h"

#include "UISelectServer.h"

UIDemoLogin::UIDemoLogin()
{
}

UIDemoLogin::~UIDemoLogin()
{
	g_pLoginLogic->RemoveEventCallBack(E_LoginEvent_LoginSuccess, this);
}

bool UIDemoLogin::initLayout()
{	
	if(!UIDialog::initLayout())
		return false;
		
	GetObjectByName(&m_pLoginButton, "login_Button");
	GetObjectByName(&m_pUserName, "name_TextField");
	GetObjectByName(&m_pUserPWD, "password_TextField");

	m_pUserName->setText("test1");
	m_pUserPWD->setText("123456");

	m_pLoginButton->addTouchEventListener(this, ui::SEL_TouchEvent(&UIDemoLogin::onLoginTouch));
		
	g_pNetLogic->ConnectServer("127.0.0.1", 14001);
	g_pLoginLogic->AddEventCallBack(E_LoginEvent_LoginSuccess, this, &UIDemoLogin::OnLoginEvent);
	return true;
}

void UIDemoLogin::initData(void *customData)
{
}


void UIDemoLogin::onLoginTouch(CCObject *pObject ,ui::TouchEventType touchType)
{
	if(touchType == ui::TouchEventType::TOUCH_EVENT_ENDED)
	{		

		g_pLoginLogic->LoginPB(m_pUserName->getStringValue(), m_pUserPWD->getStringValue(), "");
	}
}

int UIDemoLogin::OnLoginEvent(const int nEventID, const NFIDataList& varDataList)
{
	UISelectServer::showUI();
	return 0;
}