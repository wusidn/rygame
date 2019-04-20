#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include <vector>

USING_NS_CC;

class MainScene: public Scene
{
public:
    CREATE_FUNC( MainScene );
protected:
    virtual bool init( void ) override;

    void playBgm( void );
private:
    static int sm_listenId;
    int m_canPlay;
    static std::vector< int > sm_rank;

    int sm_bgmId;
};

#endif //__MAIN_SCENE_H__