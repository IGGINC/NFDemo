#ifndef _UIDialog_H__
#define _UIDialog_H__

class UIDialog : public cocos2d::CCLayer
{
public:
	UIDialog();
	~UIDialog();
		
    CCObject *GetObjectByName(const char *pName);
    template <typename T>
    T GetObjectByName(T *ppControl, const char *pName)
    {
        *ppControl = dynamic_cast<T>(GetObjectByName(pName));
        return *ppControl;
    }

	virtual const char* getUIName() { return ""; }
	virtual const char* getUIPath() { return ""; }
	virtual int getUIPriority() { return 0; }

	virtual bool init();
    virtual bool initLayout();
	virtual void initData(void *customData) {};
	
    virtual void onEnter();
    virtual void onExit();

protected:
	std::string m_strFileName;
	ui::Widget *m_pContent;
};

#endif // _UIDialog_H__
