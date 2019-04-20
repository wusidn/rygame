#include "MainScene.h"
#include "Config.h"
#include "DeviceControl.h"
#include "GameScene.h"
#include <iostream>

#define GAME_PRICE 3
#define RANK_COUNT 5

int MainScene::sm_listenId = 0;

std::map< int, int > MainScene::sm_rank;

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

    auto t_tipsSprite = Sprite::create( t_coinNumber >= GAME_PRICE ? "MainStartGameText.png" : "MainCoinTips.png" );
    t_tipsSprite->setPosition( Vec2( 900, 270 ) );
    t_tipsSprite->setScale( 0.7f );
    t_mainImage->addChild( t_tipsSprite );

    auto t_rankListBgSprite = Sprite::create( "MainRankListBg.png" );
    auto t_rankListBgSpriteSizeHalf = t_rankListBgSprite->getContentSize() * 0.5f;
    t_rankListBgSprite->setPosition( t_center );
    addChild( t_rankListBgSprite );

    auto t_rankListTipsSprite = Sprite::create( t_coinNumber >= GAME_PRICE ? "MainStartGameText.png" : "MainCoinTips.png" );
    t_rankListTipsSprite->setPosition( Vec2( Vec2( t_rankListBgSpriteSizeHalf.width, 120.0f ) ) );
    t_rankListTipsSprite->setScale( 0.7f );
    // t_rankListBgSprite->setRotationSkewY( 30.0f );
    t_rankListBgSprite->addChild( t_rankListTipsSprite );

    t_rankListBgSprite->setVisible( false );


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

    auto t_animation = Sequence::create( 

        DelayTime::create( 10.0f ),
        
        // ActionEase::create

        ActionFloat::create( 1.0f, 0.0f, 90.0f, [=]( const float p_data ){
            t_mainImage->setRotationSkewY( p_data );
        } ),
        CallFunc::create( [=](){
            t_rankListBgSprite->setRotationSkewY( -90.0f );
            t_mainImage->setVisible( false );
            t_rankListBgSprite->setVisible( true );
            t_mainImage->setRotationSkewY( 0.0f );
        } ),
        ActionFloat::create( 0.5f, -90.0f, 0.0f, [=]( const float p_data ){
            t_rankListBgSprite->setRotationSkewY( p_data );
        } ),

        DelayTime::create( 10.0f ),
        
        ActionFloat::create( 1.0f, 0.0f, 90.0f, [=]( const float p_data ){
            t_rankListBgSprite->setRotationSkewY( p_data );
        } ),
        CallFunc::create( [=](){
            t_mainImage->setRotationSkewY( -90.0f );
            t_rankListBgSprite->setVisible( false );
            t_mainImage->setVisible( true );
            t_rankListBgSprite->setRotationSkewY( 0.0f );
        } ),
        ActionFloat::create( 0.5f, -90.0f, 0.0f, [=]( const float p_data ){
            t_mainImage->setRotationSkewY( p_data );
        } ),

     nullptr );

    runAction( Repeat::create( t_animation, 10 ) );


    sm_listenId = DeviceControl::listenButtonState( [t_coinNumLabel]( int p_btnId, bool state ){
        if( p_btnId == 0x10 && state ){
            std::stringstream t_sstr;
            t_sstr << Config::getCoinNumber() << "/" << GAME_PRICE;
            t_coinNumLabel->setString( t_sstr.str() );
        }else if( p_btnId == 0x07 && state ){
            if( Config::getCoinNumber() < GAME_PRICE ){
                return;
            }

            DeviceControl::unbindListenButtonState( sm_listenId );

            Director::getInstance()->replaceScene( GameScene::create() );

            Config::setCoinNumber( Config::getCoinNumber() - GAME_PRICE );

        }
    } );

    return true;
}