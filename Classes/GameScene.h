#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class GameScene: public Scene
{
public:
    CREATE_FUNC( GameScene );
protected:
    virtual bool init( void ) override;

private:
    int sm_listenButtonId;
};

#endif //__GAME_SCENE_H__