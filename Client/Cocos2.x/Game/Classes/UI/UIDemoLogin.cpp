#include "stdafx.h"

#include "UIDemoLogin.h"
#include "Logic\LoginLogic.h"
#include "Logic\NetLogic.h"

#include "UISelectWorld.h"

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

void UIDemoLogin::updateData()
{
}

void UIDemoLogin::onLoginTouch(CCObject *pObject ,ui::TouchEventType touchType)
{
	if(touchType == ui::TouchEventType::TOUCH_EVENT_ENDED)
	{		
		NFMsg::ReqAccountLogin xMsg;
		xMsg.set_security_code("");
		xMsg.set_signbuff("");
		xMsg.set_clientversion(1);
		xMsg.set_loginmode(0);
		xMsg.set_clientip(0);
		xMsg.set_clientip(0);
		xMsg.set_clientmac(0);
		xMsg.set_device_info("");
		xMsg.set_extra_info("");

		xMsg.set_account(m_pUserName->getStringValue());
		xMsg.set_password(m_pUserPWD->getStringValue());

		g_pNetLogic->SendToServerByPB(NFMsg::EGameMsgID::EGMI_REQ_LOGIN, xMsg);
	}
}

int UIDemoLogin::OnLoginEvent(const int nEventID, const NFIDataList& varDataList)
{
	UISelectWorld::showUI();
	return 0;
}