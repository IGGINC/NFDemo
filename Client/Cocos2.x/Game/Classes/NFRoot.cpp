#include "stdafx.h"

#include "NFRoot.h"

#include "NFComm\NFPluginLoader\NFCPluginManager.h"
#include "NFGamePlugin.h"

#include "UI\UIDemoLogin.h"

USING_NS_CC;

CCScene* NFRoot::scene()
{
    CCScene *scene = CCScene::create();    
    NFRoot *layer = NFRoot::create();
    scene->addChild(layer);
    return scene;
}

NFRoot::NFRoot()
{		
}

NFRoot::~NFRoot()
{
	NFCPluginManager::GetSingletonPtr()->BeforeShut();
	NFCPluginManager::GetSingletonPtr()->Shut();
	NFCPluginManager::GetSingletonPtr()->ReleaseInstance();
}

bool NFRoot::init()
{
    if ( !CCNode::init() )
    {
        return false;
    }

	NFIPluginManager *pPluginManager = NFCPluginManager::GetSingletonPtr();

	pPluginManager->SetGetFileDataFunctor([](const char *pName, unsigned long &nSize) -> unsigned char *
	{
		return CCFileUtils::sharedFileUtils()->getFileData(pName, "rb", &nSize);
	});

	pPluginManager->SetAppName("GameClient");

	CREATE_PLUGIN(pPluginManager, NFGamePlugin);

	NFCPluginManager::GetSingletonPtr()->Init();
	NFCPluginManager::GetSingletonPtr()->AfterInit();
	NFCPluginManager::GetSingletonPtr()->CheckConfig();
	NFCPluginManager::GetSingletonPtr()->ReadyExecute();

	CCFileUtils::sharedFileUtils()->addSearchPath("EditorResources");
	CCFileUtils::sharedFileUtils()->addSearchPath("EditorResources/ccs");

	IUniqueDialog<UIDemoLogin>::showUI();

	scheduleUpdate();

    return true;
}

void NFRoot::update(float dt)
{
	NFCPluginManager::GetSingletonPtr()->Execute();
}