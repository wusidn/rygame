#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class MainScene: public Scene
{
public:
    CREATE_FUNC( MainScene );
protected:
    virtual bool init( void ) override;
};

#endif //__MAIN_SCENE_H__