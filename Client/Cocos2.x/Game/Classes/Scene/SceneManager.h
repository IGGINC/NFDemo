/********************************************************************
	purpose:	介面管理模块接口的实现类
	介面只要分為 
	系統類介面
	對話框介面
	信息介面
	功能介面
	主介面UI
	場景介面
*********************************************************************/
#ifndef _SceneManager_H_
#define _SceneManager_H_

#include "NFComm/NFCore/NFSingleton.hpp"
#include "NFComm/NFPluginModule/NFIModule.h"

class SceneManager : public NFIModule, public NFSingleton<SceneManager>
{
public:
	SceneManager() {}
	~SceneManager() {}

    SceneManager(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool ReadyExecute();
    virtual bool Execute();

    virtual bool AfterInit();

public:
    virtual CCNode* RootNode();

	void ShowScene(CCNode *pScene);
	void CloseScene(CCNode *pScene);

private:
    // 场景的根节点
	CC_SYNTHESIZE_READONLY(CCNode*, m_pRootNode, RootNode);
	CC_SYNTHESIZE_READONLY(CCNode*, m_pCurrentScene, CurrentScene);
};

#define g_pSceneManager (SceneManager::Instance())

// 与管理器逻辑无关的纯UI逻辑类
template<typename T>
class IUniqueScene: public CCLayer
{	
public:
	// 显示窗口(窗口没创建时创建窗口并显示，已经创建则只显示)
	static void showScene(const void *customData = nullptr) 
	{ 
		IUniqueScene **p = _getScene();
		if (!*p) {
			*p = new T;
			(*p)->initLayout();
			(*p)->autorelease();
		}
		
		(*p)->initData((void*)customData);
		g_pSceneManager->ShowScene(*p);
	}
	static void closeUI(const void *customData = nullptr) 
	{ 
		IUniqueScene **p = _getScene();
		if (*p) {
			g_pSceneManager->CloseScene(*p);
		}
	}
	IUniqueScene() { // 不允许外部手动new
		*_getScene() = this;
	}
	virtual ~IUniqueScene() { // 不允许外部手动delete
		if(*_getScene() == this)
			*_getScene() = nullptr;
	}

	virtual bool initLayout() { return true; };
	virtual void initData(void *) {};

private:
	static IUniqueScene** _getScene() {
		static T* _instance;
		return (IUniqueScene**)(&_instance);
	}
};

#endif
