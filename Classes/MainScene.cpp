#include "MainScene.h"
#include "Config.h"
#include "DeviceControl.h"

#define GAME_PRICE 3

bool MainScene::init( void )
{
    if( !Scene::init() )
    {
        return false;
    }

    int t_coinNumber = Config::getCoinNumber();

    auto t_visibleSizeHalf = Director::getInstance()->getVisibleSize() * 0.5f;
    Vec2 t_origin = Director::getInstance()->getVisibleOrigin();

    Vec2 t_center =  Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_visibleSizeHalf.height );

    auto t_background = Sprite::create( "MainSceneBg.png" );
    auto t_backgroundSizeHalf = t_background->getContentSize() * 0.5f;
    t_background->setPosition( t_center );
    t_background->setScale( t_visibleSizeHalf.width / t_backgroundSizeHalf.width, t_visibleSizeHalf.height / t_backgroundSizeHalf.height );
    addChild( t_background );

    auto t_mainImage = Sprite::create( "Main.png" );
    auto t_mainImageSizehalf = t_mainImage->getContentSize() * 0.5f;
    t_mainImage->setPosition( t_center );
    addChild( t_mainImage );

    auto t_coinBackground = Sprite::create( "MainCoinBg.png" );
    auto t_coinBackgroundSizehalf = t_coinBackground->getContentSize() * 0.5f;

    t_coinBackground->setPosition( t_center + t_visibleSizeHalf - t_coinBackgroundSizehalf - Vec2( 30.0f, 30.0f ) );
    addChild( t_coinBackground );

    auto t_coinSprite = Sprite::create( "MainCoin.png" );
    auto t_coinSpriteSizeHalf = t_coinSprite->getContentSize() * 0.5f;
    t_coinSprite->setPosition( t_coinSpriteSizeHalf );
    t_coinBackground->addChild( t_coinSprite );


    std::stringstream t_sstr;
    t_sstr << t_coinNumber << "/" << GAME_PRICE;

    auto t_coinNumLabel = Label::createWithTTF( t_sstr.str(), "fonts/MF-MINGHEI-NONCOMMERCIAL-REGULAR.ttf", 70 );
    t_coinNumLabel->setPosition( Vec2( 200.0f, 90.0f ) );
    t_coinBackground->addChild( t_coinNumLabel );

    auto t_tipsSprite = Sprite::create( t_coinNumber >= GAME_PRICE ? "MainStartGameText.png" : "MainCoinTips.png" );
    t_tipsSprite->setPosition( Vec2( 900, 270 ) );
    t_tipsSprite->setScale( 0.7f );
    t_mainImage->addChild( t_tipsSprite );

    DeviceControl::listenButtonState( [=]( int p_btnId, bool state ){
        if( p_btnId == 0x10 && state ){
            std::stringstream t_sstr;
            t_sstr << Config::getCoinNumber() << "/" << GAME_PRICE;
            t_coinNumLabel->setString( t_sstr.str() );
        }
    } );

    return true;
}