#ifndef _UISelectWorld_H__
#define _UISelectWorld_H__

#include "UIManager.h"

class UISelectWorld : public IUniqueDialog<UISelectWorld>
{
public:
	UISelectWorld();
	~UISelectWorld();
	virtual const char* getUIName() { return ""; }
	virtual const char* getUIPath() { return ""; }
	virtual int getUIPriority() { return 0; }

    virtual bool initLayout();
	virtual void updateData();

private:
	int OnWorldListEvent(const int nEventID, const NFIDataList& varDataList);
	void OnSelectWorldProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

private:
	void onLoginTouch(CCObject *pObject , ui::TouchEventType touchType);
	
private:
	ui::ListView *m_pListView;

};

#endif // __HELLOWORLD_SCENE_H__
