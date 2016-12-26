#include "stdafx.h"

#include "UISelectRole.h"
#include "Logic\PlayerLogic.h"
#include "Scene\GameScene.h"

UISelectRole::UISelectRole()
{
}

UISelectRole::~UISelectRole()
{
	g_pPlayerLogic->RemoveEventCallBack(E_PlayerEvent_RoleList, this);
}

bool UISelectRole::initLayout()
{	
	if(!UIDialog::initLayout())
		return false;

	m_pRoleList = ui::ListView::create();
	m_pRoleList->setSizeType(ui::SizeType::SIZE_PERCENT);
	m_pRoleList->setSizePercent(ccp(0.1f, 0.25f));
	m_pRoleList->setPositionType(ui::PositionType::POSITION_PERCENT);
	m_pRoleList->setPositionPercent(ccp(0.5f, 0.5f));
	//m_pRoleList->setBackGroundColorType(ui::LayoutBackGroundColorType::LAYOUT_COLOR_SOLID);
	m_pRoleList->setBackGroundColor(ccc3(255, 0, 0));
	m_pRoleList->setAnchorPoint(ccp(0.5, 0.5));
	m_pContent->addChild(m_pRoleList);

	auto button = ui::Button::create();
	button->loadTextureNormal("Button_Normal.png",ui::TextureResType::UI_TEX_TYPE_LOCAL);
	button->setScale9Enabled(true);
	button->setSizePercent(ccp(1.0f, 0.2f));
	button->setSizeType(ui::SizeType::SIZE_PERCENT);
	m_pRoleList->setItemModel(button);

	button = ui::Button::create();
	button->loadTextureNormal("Button_Normal.png",ui::TextureResType::UI_TEX_TYPE_LOCAL);
	button->setScale9Enabled(true);
	button->setSizePercent(ccp(0.1f, 0.1f));
	button->setSizeType(ui::SizeType::SIZE_PERCENT);
	button->setTitleText("CreateRole");
	button->setPositionType(ui::PositionType::POSITION_PERCENT);
	button->setPositionPercent(ccp(0.5f, 0.2f));
	button->setTitleColor(ccc3(0, 0, 0));
	button->addTouchEventListener(this, ui::SEL_TouchEvent(&UISelectRole::OnRoleCreate));
	m_pContent->addChild(button);

	
	g_pPlayerLogic->AddEventCallBack(E_PlayerEvent_RoleList, this, &UISelectRole::OnRoleListEvent);
	return true;
}

void UISelectRole::initData(void *customData)
{
	g_pPlayerLogic->RequireRoleList();
	m_pRoleList->removeAllItems();
}

int UISelectRole::OnRoleListEvent(const int nEventID, const NFIDataList& varDataList)
{
	auto roleList = g_pPlayerLogic->GetRoleList();
	m_pRoleList->removeAllItems();
	for(int i = 0; i < roleList.size(); i++)
	{
		const NFMsg::RoleLiteInfo &kRoleInfo =  roleList[i];
		m_pRoleList->pushBackDefaultItem();
		ui::Button *pButton = (ui::Button *)m_pRoleList->getItem(m_pRoleList->getItems()->count()-1);
		pButton->addTouchEventListener(this, ui::SEL_TouchEvent(&UISelectRole::OnRoleSelected));
		pButton->setTitleText(kRoleInfo.noob_name());
		pButton->setTitleColor(ccc3(0, 0, 0));
	}

	return 0;
}

void UISelectRole::OnRoleSelected(CCObject *pObject , ui::TouchEventType type)
{
	if(type == ui::TouchEventType::TOUCH_EVENT_ENDED)
	{
		auto serverList = g_pPlayerLogic->GetRoleList();
		int nIndex = m_pRoleList->getIndex((Widget*)pObject);
		g_pPlayerLogic->RequireEnterGameServer(0);
		g_pUIManager->CloseDialog();
		GameScene::showScene();
	}
}

void UISelectRole::OnRoleCreate(CCObject *pObject , ui::TouchEventType type)
{
	if(type == ui::TouchEventType::TOUCH_EVENT_ENDED)
	{
		NFCDataList varData;
		g_pKernelModule->Random(0, 1000, 1, varData);
		g_pPlayerLogic->RequireCreateRole(CCString::createWithFormat("TestRole%lld", varData.Int(0))->getCString(), 1, 1);
	}
}