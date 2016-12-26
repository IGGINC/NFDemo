#include "stdafx.h"

#include "UISelectServer.h"
#include "Logic\LoginLogic.h"
#include "Logic\NetLogic.h"

#include "UISelectRole.h"

UISelectServer::UISelectServer()
{
}

UISelectServer::~UISelectServer()
{
	g_pLoginLogic->RemoveEventCallBack(E_LoginEvent_WorldList, this);
	g_pLoginLogic->RemoveEventCallBack(E_LoginEvent_ServerList, this);
}

bool UISelectServer::initLayout()
{	
	if(!UIDialog::initLayout())
		return false;

	m_pServerList = ui::ListView::create();
	m_pServerList->setSizeType(ui::SizeType::SIZE_PERCENT);
	m_pServerList->setSizePercent(ccp(0.1f, 0.25f));
	m_pServerList->setPositionType(ui::PositionType::POSITION_PERCENT);
	m_pServerList->setPositionPercent(ccp(0.5f, 0.5f));
	//m_pServerList->setBackGroundColorType(ui::LayoutBackGroundColorType::LAYOUT_COLOR_SOLID);
	m_pServerList->setBackGroundColor(ccc3(255, 0, 0));
	m_pServerList->setAnchorPoint(ccp(0.5, 0.5));
	m_pContent->addChild(m_pServerList);

	auto button = ui::Button::create();
	button->loadTextureNormal("Button_Normal.png",ui::TextureResType::UI_TEX_TYPE_LOCAL);
	button->setScale9Enabled(true);
	button->setSizePercent(ccp(1.0f, 0.2f));
	button->setSizeType(ui::SizeType::SIZE_PERCENT);

	m_pServerList->setItemModel(button);
	
	g_pLoginLogic->AddEventCallBack(E_LoginEvent_WorldList, this, &UISelectServer::OnWorldListEvent);
	g_pLoginLogic->AddEventCallBack(E_LoginEvent_ServerList, this, &UISelectServer::OnServerListEvent);
	return true;
}

void UISelectServer::initData(void *customData)
{
	g_pLoginLogic->RequireWorldList();
	m_pServerList->removeAllItems();
}

int UISelectServer::OnWorldListEvent(const int nEventID, const NFIDataList& varDataList)
{
	auto worldList = g_pLoginLogic->GetWorldList();
	for(int i = 0; i < worldList.size(); i++)
	{
		const NFMsg::ServerInfo &kServerInfo =  worldList[i];
		m_pServerList->pushBackDefaultItem();
		ui::Button *pButton = (ui::Button *)m_pServerList->getItem(m_pServerList->getItems()->count()-1);
		pButton->addTouchEventListener(this, ui::SEL_TouchEvent(&UISelectServer::OnWorldSelected));
		pButton->setTitleText(kServerInfo.name());
		pButton->setTitleColor(ccc3(0, 0, 0));
	}

	return 0;
}

int UISelectServer::OnServerListEvent(const int nEventID, const NFIDataList& varDataList)
{
	auto worldList = g_pLoginLogic->GetServerList();
	for(int i = 0; i < worldList.size(); i++)
	{
		const NFMsg::ServerInfo &kServerInfo =  worldList[i];
		m_pServerList->pushBackDefaultItem();
		ui::Button *pButton = (ui::Button *)m_pServerList->getItem(m_pServerList->getItems()->count()-1);
		pButton->addTouchEventListener(this, ui::SEL_TouchEvent(&UISelectServer::OnServerSelected));
		pButton->setTitleText(kServerInfo.name());
		pButton->setTitleColor(ccc3(0, 0, 0));
	}

	return 0;
}

void UISelectServer::OnWorldSelected(CCObject *pObject , ui::TouchEventType type)
{
	if(type == ui::TouchEventType::TOUCH_EVENT_ENDED)
	{
		auto worldList = g_pLoginLogic->GetWorldList();
		int nIndex = m_pServerList->getIndex((Widget*)pObject);
		g_pLoginLogic->RequireConnectWorld(worldList[nIndex].server_id());		
		m_pServerList->removeAllItems();
	}
}

void UISelectServer::OnServerSelected(CCObject *pObject , ui::TouchEventType type)
{
	if(type == ui::TouchEventType::TOUCH_EVENT_ENDED)
	{
		auto serverList = g_pLoginLogic->GetServerList();
		int nIndex = m_pServerList->getIndex((Widget*)pObject);
		g_pLoginLogic->RequireSelectServer(serverList[nIndex].server_id());
		UISelectRole::showUI(true);
	}
}