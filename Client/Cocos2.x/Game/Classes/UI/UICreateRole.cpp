#include "stdafx.h"

#include "UICreateRole.h"
#include "Logic\PlayerLogic.h"

UICreateRole::UICreateRole()
{
}

UICreateRole::~UICreateRole()
{
	g_pPlayerLogic->RemoveEventCallBack(E_PlayerEvent_RoleList, this);
}

bool UICreateRole::initLayout()
{	
	if(!UIDialog::initLayout())
		return false;

	return true;
}

void UICreateRole::updateData()
{
	g_pPlayerLogic->RequireRoleList();
	m_pRoleList->removeAllItems();
}

int UICreateRole::OnRoleListEvent(const int nEventID, const NFIDataList& varDataList)
{
	auto roleList = g_pPlayerLogic->GetRoleList();
	for(int i = 0; i < roleList.size(); i++)
	{
		const NFMsg::RoleLiteInfo &kRoleInfo =  roleList[i];
		m_pRoleList->pushBackDefaultItem();
		ui::Button *pButton = (ui::Button *)m_pRoleList->getItem(m_pRoleList->getItems()->count()-1);
		pButton->addTouchEventListener(this, ui::SEL_TouchEvent(&UICreateRole::OnRoleSelected));
		pButton->setTitleText(kRoleInfo.noob_name());
		pButton->setTitleColor(ccc3(0, 0, 0));
	}

	return 0;
}

void UICreateRole::OnRoleSelected(CCObject *pObject , ui::TouchEventType type)
{
	if(type == ui::TouchEventType::TOUCH_EVENT_ENDED)
	{
		auto serverList = g_pPlayerLogic->GetRoleList();
		int nIndex = m_pRoleList->getIndex((Widget*)pObject);
		//g_pPlayerLogic->RequireRoleList(serverList[nIndex].server_id());
	}
}

void UICreateRole::OnRoleCreate(CCObject *pObject , ui::TouchEventType type)
{
	if(type == ui::TouchEventType::TOUCH_EVENT_ENDED)
	{

	}
}