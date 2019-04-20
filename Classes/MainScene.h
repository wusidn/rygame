#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include <map>

USING_NS_CC;

class MainScene: public Scene
{
public:
    CREATE_FUNC( MainScene );
protected:
    virtual bool init( void ) override;
private:
    static int sm_listenId;
    static std::map< int, int > sm_rank;
};

#endif //__MAIN_SCENE_H__