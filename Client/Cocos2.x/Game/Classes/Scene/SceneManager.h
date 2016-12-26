/********************************************************************
	purpose:	�������ģ��ӿڵ�ʵ����
	����ֻҪ�֞� 
	ϵ�y���
	��Ԓ�����
	��Ϣ����
	���ܽ���
	������UI
	��������
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
    // �����ĸ��ڵ�
	CC_SYNTHESIZE_READONLY(CCNode*, m_pRootNode, RootNode);
	CC_SYNTHESIZE_READONLY(CCNode*, m_pCurrentScene, CurrentScene);
};

#define g_pSceneManager (SceneManager::Instance())

// ��������߼��޹صĴ�UI�߼���
template<typename T>
class IUniqueScene: public CCLayer
{	
public:
	// ��ʾ����(����û����ʱ�������ڲ���ʾ���Ѿ�������ֻ��ʾ)
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
	IUniqueScene() { // �������ⲿ�ֶ�new
		*_getScene() = this;
	}
	virtual ~IUniqueScene() { // �������ⲿ�ֶ�delete
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
