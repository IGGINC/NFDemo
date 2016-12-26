#ifndef _GameScene_H__
#define _GameScene_H__

#include "SceneManager.h"
#include "Entity\BattleEntityPlayer.h"

class GameScene : public IUniqueScene<GameScene>
{
public:
	GameScene();
	~GameScene();
	
	virtual bool initLayout();
	virtual void initData(void *pData);

private:
	int OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var);

private:
	NFMap<NFGUID, BattleEntityPlayer> m_Players;
};

#endif // __HELLOWORLD_SCENE_H__
