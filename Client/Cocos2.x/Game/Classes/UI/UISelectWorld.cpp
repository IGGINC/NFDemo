#include "stdafx.h"

#include "UISelectWorld.h"
#include "Logic\LoginLogic.h"
#include "Logic\NetLogic.h"

UISelectWorld::UISelectWorld()
{
}

UISelectWorld::~UISelectWorld()
{
	g_pLoginLogic->RemoveEventCallBack(E_LoginEvent_WorldList, this);
}

bool UISelectWorld::initLayout()
{	
	if(!UIDialog::initLayout())
		return false;

	m_pListView = ui::ListView::create();
	m_pListView->setSizeType(ui::SizeType::SIZE_PERCENT);
	m_pListView->setSizePercent(ccp(0.1f, 0.5f));
	m_pListView->setPositionType(ui::PositionType::POSITION_PERCENT);
	m_pListView->setPositionPercent(ccp(0.5f, 0.5f));
	//m_pListView->setBackGroundColorType(ui::LayoutBackGroundColorType::LAYOUT_COLOR_SOLID);
	//m_pListView->setBackGroundColor(ccc3(255, 0, 0));
	m_pListView->setAnchorPoint(ccp(0.5, 0.5));

	auto button = ui::Button::create();
	button->loadTextureNormal("Button_Normal.png",ui::TextureResType::UI_TEX_TYPE_LOCAL);
	button->setScale9Enabled(true);
	button->setSizePercent(ccp(1.0f, 0.2f));
	button->setSizeType(ui::SizeType::SIZE_PERCENT);
	m_pListView->setItemModel(button);
	m_pContent->addChild(m_pListView);
	
	g_pLoginLogic->AddEventCallBack(E_LoginEvent_WorldList, this, &UISelectWorld::OnWorldListEvent);
	return true;
}

void UISelectWorld::updateData()
{
	NFMsg::ReqServerList xMsg;
	xMsg.set_type(NFMsg::RSLT_WORLD_SERVER);
	g_pNetLogic->SendToServerByPB(NFMsg::EGameMsgID::EGMI_REQ_WORLD_LIST, xMsg);
	m_pListView->removeAllItems();
}

int UISelectWorld::OnWorldListEvent(const int nEventID, const NFIDataList& varDataList)
{
	auto worldList = g_pLoginLogic->GetWorldList();
	for(int i = 0; i < worldList.size(); i++)
	{
		const NFMsg::ServerInfo &kServerInfo =  worldList[i];
		m_pListView->pushBackDefaultItem();
		ui::Button *pButton = (ui::Button *)m_pListView->getItem(m_pListView->getItems()->count()-1);
		pButton->setTitleText(kServerInfo.name());
		pButton->setTitleColor(ccc3(0, 0, 0));
	}

	return 0;
}

void UISelectWorld::OnSelectWorldProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
}