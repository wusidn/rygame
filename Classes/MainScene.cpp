#include "MainScene.h"
#include "Config.h"
#include "DeviceControl.h"
#include "GameScene.h"
#include "AudioEngine.h"
#include <iostream>

#define GAME_PRICE 3
#define RANK_COUNT 5

using namespace cocos2d::experimental;

int MainScene::sm_listenId = 0;

std::vector< int > MainScene::sm_rank;

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
    t_rankListBgSprite->addChild( t_rankListTipsSprite );

    t_rankListBgSprite->setVisible( false );


    for( int i = 0; i < RANK_COUNT; i++)
    {
        std::stringstream t_sstr;
        if( sm_rank.size() > i )
        {
            t_sstr << sm_rank[i];
        }else{
            t_sstr << "-";
        }
        auto t_label = Label::createWithTTF( t_sstr.str(), "fonts/AMSIPRO-ULTRA.ttf", 70 );
        t_label->setPosition( Vec2( t_rankListBgSpriteSizeHalf.width + ( ceil(i / 2.0f) * (i % 2 ? 1 : -1) * 250.0f )  , t_rankListBgSpriteSizeHalf.height - 80.0f ) );
        t_rankListBgSprite->addChild( t_label );
    }


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

    auto t_coinNumLabel = Label::createWithTTF( t_sstr.str(), "fonts/AMSIPRO-ULTRA.ttf", 70 );
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

    runAction( RepeatForever::create( t_animation ) );


    sm_listenId = DeviceControl::listenButtonState( [t_coinNumLabel, t_tipsSprite, t_rankListTipsSprite, this]( int p_btnId, bool state ){
        if( p_btnId == BTN_COIN && state ){
            std::stringstream t_sstr;
            t_sstr << Config::getCoinNumber() << "/" << GAME_PRICE;
            t_coinNumLabel->setString( t_sstr.str() );


            if( Config::getCoinNumber() >= GAME_PRICE && !m_canPlay )
            {
                m_canPlay = true;
                t_tipsSprite->setTexture( "MainStartGameText.png" );
                t_rankListTipsSprite->setTexture( "MainStartGameText.png" );
            }
        }else if( p_btnId == BTN_START && state ){
            if( !m_canPlay ){
                return;
            }

            sm_bgmId = -1;
            AudioEngine::stopAll();

            AudioEngine::play2d( "audios/Confirm.mp3" );

            DeviceControl::unbindListenButtonState( sm_listenId );

            Director::getInstance()->replaceScene( TransitionFade::create( 3.0f, GameScene::create() ) );

            Config::setCoinNumber( Config::getCoinNumber() - GAME_PRICE );

        }
    } );

    m_canPlay = Config::getCoinNumber() >= GAME_PRICE;

    playBgm();

    return true;
}


void MainScene::playBgm( void )
{
    srand ((unsigned)time(nullptr));
    int t_playIndex = rand() % 3;

    std::stringstream t_audioPath;
    t_audioPath << "audios/Standby_" << t_playIndex + 1 << ".mp3";
    
    sm_bgmId = AudioEngine::play2d( t_audioPath.str() );

    AudioEngine::setFinishCallback( sm_bgmId, [this]( int p_audioId, const std::string & p_path ){
        
        if( sm_bgmId < 0 )
        {
            return;
        }

        playBgm();
    } );
}