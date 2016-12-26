#ifndef _UISelectServer_H__
#define _UISelectServer_H__

#include "UIManager.h"

class UISelectServer : public IUniqueDialog<UISelectServer>
{
public:
	UISelectServer();
	~UISelectServer();
	virtual const char* getUIName() { return ""; }
	virtual const char* getUIPath() { return ""; }
	virtual int getUIPriority() { return 0; }

    virtual bool initLayout();
	virtual void initData(void *customData);

private:
	int OnWorldListEvent(const int nEventID, const NFIDataList& varDataList);
	int OnServerListEvent(const int nEventID, const NFIDataList& varDataList);

private:
	void OnWorldSelected(CCObject *pObject, ui::TouchEventType type);
	void OnServerSelected(CCObject *pObject, ui::TouchEventType type);
	
private:
	ui::ListView *m_pServerList;

};

#endif // _UISelectServer_H__
