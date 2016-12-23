#include "stdafx.h"

#include "UIDialog.h"

UIDialog::UIDialog()
	: m_pContent(NULL)
	, m_bDirty(true)
{
}

UIDialog::~UIDialog()
{
}

CCObject *UIDialog::GetObjectByName(const char *pName)
{
	return ui::UIHelper::seekWidgetByName(m_pContent, pName);
}

bool UIDialog::init()
{
	CCLayer::init();
	return initLayout();
}

bool UIDialog::initLayout()
{	
	ui::TouchGroup* tg = cocos2d::ui::TouchGroup::create();
		
	setTouchPriority(getUIPriority());
	setTouchEnabled(true);

	tg->setTouchPriority(getTouchPriority()-1);
	addChild(tg);
	
	if(strcmp(getUIPath(), "") != 0)
	{
		m_strFileName = CCFileUtils::sharedFileUtils()->fullPathForFilename(getUIPath());
		m_pContent = GUIReader::shareReader()->widgetFromJsonFile(m_strFileName.c_str());
	}
	else
	{
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		ui::Layout *pLayout = ui::Layout::create();
		pLayout->setSize(winSize);
		m_pContent = pLayout;
	}

	if(!m_pContent)
	{
		return false;
	}

	tg->addWidget(m_pContent);

	return true;
}

void UIDialog::onEnter()
{
	CCLayer::onEnter();

	if(m_bDirty && m_pContent)
	{
		m_bDirty = false;
		updateData();
	}
}

void UIDialog::onExit()
{
	CCLayer::onExit();
}

void UIDialog::updateData()
{
}