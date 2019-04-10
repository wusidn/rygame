#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class LoadingScene : public Scene
{
public:
    CREATE_FUNC( LoadingScene );
    
protected:
    virtual bool init( void ) override;
};

#endif //__LOADING_SCENE_H__