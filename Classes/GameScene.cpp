#include "GameScene.h"
#include "DeviceControl.h"
#include "MainScene.h"
#include "AudioEngine.h"

using namespace cocos2d::experimental;

bool GameScene::init( void )
{
    if( !Scene::init() )
    {
        return false;
    }

    auto t_visibleSizeHalf = Director::getInstance()->getVisibleSize() * 0.5f;
    Vec2 t_origin = Director::getInstance()->getVisibleOrigin();

    Vec2 t_center =  Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_visibleSizeHalf.height );

    auto t_background = Sprite::create( "GameBg.png" );
    auto t_backgroundSizeHalf = t_background->getContentSize() * 0.5f;
    t_background->setPosition( t_center );
    t_background->setScale( t_visibleSizeHalf.width / t_backgroundSizeHalf.width, t_visibleSizeHalf.height / t_backgroundSizeHalf.height );
    addChild( t_background );



    sm_listenButtonId = DeviceControl::listenButtonState( [this]( int p_btnId, bool p_state ){
        if( p_btnId == BTN_START && p_state )
        {
            Director::getInstance()->replaceScene( TransitionFade::create( 3.0f, MainScene::create() ) );
        }
    } );


    runAction( Sequence::create( DelayTime::create( 2.0f ), 
                CallFunc::create( [=](){
                    AudioEngine::play2d( "audios/InGame.mp3" );
                } ),
    nullptr ) );

    return true;
}