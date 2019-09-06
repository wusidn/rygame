#include "LoadingScene.h"
#include "DeviceControl.h"
#include "Config.h"
#include "AudioEngine.h"

#include <iostream>

USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocos2d::experimental;


bool LoadingScene::init( void )
{
    if( !Scene::init() )
    {
        return false;
    }

    if( !DeviceControl::init() )
    {
        return false;
    }

    if( !Config::init() )
    {
        return false;
    }

    DeviceControl::listenButtonState( []( int p_btnId, bool p_state ){
        if( ( p_btnId == BTN_COIN_1 || p_btnId == BTN_COIN_2 ) && p_state )
        {
            Config::setCoinNumber( Config::getCoinNumber() + 1 );
            AudioEngine::play2d( "audios/Coin.mp3" );
        }
    } );

    return true;

}
