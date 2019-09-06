#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include <vector>
#include <functional>

USING_NS_CC;

class GameScene: public Scene
{

    enum GameState{
        Before = 0,
        Ready,
        TPT,
        PassThrough,
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
    float m_gameTime;
    int m_gameLevel;
    int m_gameScore;
    int m_sumScore;
    std::vector< int > m_targetScoreList;

    std::function< void(void) > m_gameEndFunc;

    Sprite * m_gameTimeBit1Sprite, * m_gameTimeBit2Sprite;
    Sprite * m_promptBoxSprite, * m_promptSprite, * m_gameLevelSprite;
    Label * m_promptLabel;
};

#endif //__GAME_SCENE_H__