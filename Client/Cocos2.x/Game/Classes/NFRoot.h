#ifndef __NFRoot_H__
#define __NFRoot_H__

#include "cocos2d.h"

class NFRoot : public cocos2d::CCNode
{
public:
	NFRoot();
	~NFRoot();
    virtual bool init();  
    static cocos2d::CCScene* scene();
	virtual void update(float dt);
    CREATE_FUNC(NFRoot);
};

#endif // __HELLOWORLD_SCENE_H__
