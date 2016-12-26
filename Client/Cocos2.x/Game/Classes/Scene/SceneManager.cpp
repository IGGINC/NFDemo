/********************************************************************
	purpose:	界面管理模块实现
*********************************************************************/
#include "stdafx.h"

#include "SceneManager.h"

#include "../NFRoot.h"
//-----------------------------------------------------
// 初始化
//-----------------------------------------------------
bool SceneManager::Init()
{
    m_pRootNode = CCNode::create();
    m_pRootNode->retain();
	m_pCurrentScene = NULL;
	NFRoot::Instance()->addChild(m_pRootNode, NFRoot::eIZO_Scene);
    
	return true;
}
//-----------------------------------------------------
// 关闭
//-----------------------------------------------------
bool SceneManager::Shut()
{
	if (NULL == m_pRootNode)
		return true;

	m_pRootNode->removeFromParentAndCleanup(true);
	CC_SAFE_RELEASE_NULL(m_pRootNode);

	return true;
}

bool SceneManager::ReadyExecute()
{
	return true;
}

bool SceneManager::Execute()
{
	return true;
}

bool SceneManager::AfterInit()
{
	return true;
}

CCNode* SceneManager::RootNode()
{
    return m_pRootNode;
}

void SceneManager::ShowScene(CCNode *pScene)
{
	if(!pScene)
		return ;

	if(m_pCurrentScene == pScene)
		return ;

	if(m_pCurrentScene)
	{
		CloseScene(m_pCurrentScene);
	}
	m_pCurrentScene = pScene;	
	m_pRootNode->addChild(pScene);
	m_pCurrentScene->retain();
}

void SceneManager::CloseScene(CCNode *pScene)
{
	if(!pScene)
		return ;

	if(m_pCurrentScene == pScene)
	{
		// 使其在下一阵
		m_pCurrentScene->autorelease();
		m_pCurrentScene->release();
		m_pCurrentScene = NULL;
	}

	m_pRootNode->removeChild(pScene);
}