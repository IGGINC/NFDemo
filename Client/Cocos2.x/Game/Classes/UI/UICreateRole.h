#ifndef _UICreateRole_H__
#define _UICreateRole_H__

#include "UIManager.h"

class UICreateRole : public IUniqueDialog<UICreateRole>
{
public:
	UICreateRole();
	~UICreateRole();
	virtual const char* getUIName() { return ""; }
	virtual const char* getUIPath() { return ""; }
	virtual int getUIPriority() { return 0; }

    virtual bool initLayout();
	virtual void updateData();

private:
	int OnRoleListEvent(const int nEventID, const NFIDataList& varDataList);

private:
	void OnCareerSelected(CCObject *pObject, ui::TouchEventType type);
	void OnSexSelected(CCObject *pObject, ui::TouchEventType type);
	
private:
	ui::CheckBox *m_pCareer1;

};

#endif // _UICreateRole_H__
