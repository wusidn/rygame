#include "GameScene.h"

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

    return true;
}