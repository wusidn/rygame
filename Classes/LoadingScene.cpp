#include "LoadingScene.h"
#include "Serial.h"
#include <thread>

USING_NS_CC;
using namespace cocos2d::ui;

bool LoadingScene::init( void )
{
    if( !Scene::init() )
    {
        return false;
    }

    //

    std::thread([](){
        Serial::open();
    }).detach();
    
    return true;
}

