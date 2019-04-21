#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include <vector>

USING_NS_CC;

class GameScene: public Scene
{

    enum GameState{
        Before = 0,
        Ready,
        TPT,
        End
    };

public:
    CREATE_FUNC( GameScene );
protected:
    virtual bool init( void ) override;
    virtual void update( float p_dt );
private:
    int sm_listenButtonId;
    GameState m_gameState; 
    int m_gameLevel, m_gameTime;
    float m_gameScore;
    std::vector< int > m_targetScoreList;
};

#endif //__GAME_SCENE_H__