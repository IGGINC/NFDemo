#include "stdafx.h"
#include "Logic/NetLogic.h"
#include "Logic/PlayerLogic.h"
#include "../../../../Common/NFMessageDefine/NFProtocolDefine.hpp"
#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

bool GameScene::initLayout()
{	
	CCSprite *pBackGround = CCSprite::create("Scene/bbg_fine_ship.jpg");
	pBackGround->setAnchorPoint(ccp(0,0));
	pBackGround->setScale(1.2);
	addChild(pBackGround);
	
    g_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &GameScene::OnObjectClassEvent);

	return true;
}

void GameScene::initData(void *pData)
{
}

int GameScene::OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
{
    if (strClassName == NFrame::Player::ThisName())
    {
        if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
        {
			BattleEntityPlayer *pPlayer = BattleEntityPlayer::create();
			m_Players.AddElement(self, pPlayer);
			addChild(pPlayer);
        }
        else if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
        {
			m_Players.GetElement(self)->removeFromParent();
			m_Players.RemoveElement(self);
        }
    }

    return 0;
}

bool GameScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint pos = this->convertTouchToNodeSpace(pTouch);

	g_pPlayerLogic->RequireMove(NFVector3(pos.x, pos.y, 0));

	return true;
}

void GameScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
}

void GameScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
}

void GameScene::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
}