/********************************************************************
	purpose:	界面管理模块实现
*********************************************************************/
#include "stdafx.h"

#include "UIManager.h"

#include "../NFRoot.h"
//-----------------------------------------------------
// 初始化
//-----------------------------------------------------
bool UIManager::Init()
{
	m_pCurrentDialog = NULL;
    m_pRootNode = CCNode::create();
    m_pRootNode->retain();

	NFRoot::Instance()->addChild(m_pRootNode, NFRoot::eIZO_UI);
    
    m_pPanelNode = CCNode::create();
    m_pRootNode->addChild(m_pPanelNode);

    m_pDialogNode = CCNode::create();
    m_pRootNode->addChild(m_pDialogNode);

    m_pBoxNode = CCNode::create();
    m_pRootNode->addChild(m_pBoxNode);

	m_pEffectNode = CCNode::create(); 
	m_pRootNode->addChild(m_pEffectNode);

    m_pDialogQueue = CCArray::create();
    m_pDialogQueue->retain();

	return true;
}
//-----------------------------------------------------
// 关闭
//-----------------------------------------------------
bool UIManager::Shut()
{
	if (NULL == m_pRootNode)
		return true;

	m_pRootNode->removeFromParentAndCleanup(true);
	CC_SAFE_RELEASE_NULL(m_pRootNode);

	for (unsigned int i = 0; i < m_vecDialog.size(); i++)
	{
		if (m_vecDialog[i])
		{
			m_vecDialog[i] = NULL;
		}
	}

	m_pRootNode = NULL;
	m_pDialogNode = NULL;
	m_pPanelNode = NULL;
	m_pEffectNode = NULL;
	m_pBoxNode = NULL;

	CC_SAFE_RELEASE_NULL(m_pDialogQueue);

	return true;
}

bool UIManager::ReadyExecute()
{
	return true;
}

bool UIManager::Execute()
{
	return true;
}

bool UIManager::AfterInit()
{
	return true;
}

CCNode* UIManager::RootNode()
{
    return m_pRootNode;
}

UIDialog* UIManager::ShowDialog(UIDialog *pDialog, bool bPushToHistory/* = true*/)
{
    UIDialog *pOldDialog = m_pCurrentDialog;

    m_pCurrentDialog = pDialog;
    
    if(pOldDialog)
    {
        if (bPushToHistory)
            m_pDialogQueue->addObject(pOldDialog);

        m_pDialogNode->removeChild(pOldDialog);
    }

    if(m_pCurrentDialog)
    {
        m_pDialogNode->addChild(m_pCurrentDialog);
    }

    return m_pCurrentDialog;
}

void UIManager::CloseDialog(UIDialog *pDialog)
{
	if(m_pCurrentDialog == pDialog)
	{
		ChangeBackDialog();
		return ;
	}

	int nIndex = m_pDialogQueue->indexOfObject(pDialog);
	if(nIndex != CC_INVALID_INDEX)
	{
		m_pDialogQueue->removeObjectAtIndex(nIndex);
	}
	m_pDialogNode->removeChild(pDialog);
}

void UIManager::ChangeBackDialog()
{
    int nDialogs = m_pDialogQueue->count();

    if (m_pCurrentDialog)
    {
        if (nDialogs == 0)
        {
			m_pCurrentDialog->runAction(CCSequence::create(CCDelayTime::create(0.3f), CCRemoveSelf::create(), NULL));
        }
        else
        {
            m_pDialogNode->removeChild(m_pCurrentDialog);
        }
        m_pCurrentDialog = NULL;
    }

    if(nDialogs > 0)
    {
        m_pCurrentDialog = (UIDialog*)m_pDialogQueue->objectAtIndex(nDialogs-1);
        if(m_pCurrentDialog)
        {
            m_pDialogNode->addChild(m_pCurrentDialog);
        }
        m_pDialogQueue->removeLastObject();
    }
}

bool UIManager::HasBackDialog()
{
    return m_pDialogQueue->count() > 0;
}

void UIManager::CloseDialog()
{
    if(m_pCurrentDialog)
    {
        m_pCurrentDialog->removeFromParentAndCleanup(true);
        m_pCurrentDialog = NULL;
    }
    m_pDialogQueue->removeAllObjects();
}

void UIManager::onBackKeyClicked()
{
	CCLOG("%s", __FUNCTION__);
}

void UIManager::onMenuKeyClicked()
{

}