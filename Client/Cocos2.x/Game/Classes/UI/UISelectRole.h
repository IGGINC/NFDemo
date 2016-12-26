#ifndef _UISelectRole_H__
#define _UISelectRole_H__

#include "UIManager.h"

class UISelectRole : public IUniqueDialog<UISelectRole>
{
public:
	UISelectRole();
	~UISelectRole();
	virtual const char* getUIName() { return ""; }
	virtual const char* getUIPath() { return ""; }
	virtual int getUIPriority() { return 0; }

    virtual bool initLayout();
	virtual void initData(void *customData);

private:
	int OnRoleListEvent(const int nEventID, const NFIDataList& varDataList);

private:
	void OnRoleSelected(CCObject *pObject, ui::TouchEventType type);
	void OnRoleCreate(CCObject *pObject, ui::TouchEventType type);
	
private:
	ui::ListView *m_pRoleList;

};

#endif // _UISelectRole_H__
