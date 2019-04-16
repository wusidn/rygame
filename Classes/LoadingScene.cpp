#include "LoadingScene.h"
#include "DeviceControl.h"
#include "Config.h"

#include <iostream>

USING_NS_CC;
using namespace cocos2d::ui;


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
        if( p_btnId == 0x10 && p_state )
        {
            Config::setCoinNumber( Config::getCoinNumber() + 1 );
        }
    } );

    return true;

}
