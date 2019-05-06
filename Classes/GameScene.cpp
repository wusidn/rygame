#include "cocos2d.h"
#include "GameScene.h"
#include "DeviceControl.h"
#include "MainScene.h"
#include "AudioEngine.h"
#include "ScoreNumber.h"

#include <iostream>

USING_NS_CC;
using namespace cocos2d::experimental;

bool GameScene::init( void )
{
    if( !Scene::init() )
    {
        return false;
    }

    m_targetScoreList.push_back( 100 );
    m_targetScoreList.push_back( 200 );
    m_targetScoreList.push_back( 300 );

    m_gameState = GameState::Before;
    m_gameLevel = 0;
    m_gameScore = 0;
    m_gameTime = 60.99f;

    auto t_visibleSizeHalf = Director::getInstance()->getVisibleSize() * 0.5f;
    Vec2 t_origin = Director::getInstance()->getVisibleOrigin();

    Vec2 t_center =  Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_visibleSizeHalf.height );

    auto t_background = Sprite::create( "GameBg.png" );
    auto t_backgroundSizeHalf = t_background->getContentSize() * 0.5f;
    t_background->setPosition( t_center );
    t_background->setScale( t_visibleSizeHalf.width / t_backgroundSizeHalf.width, t_visibleSizeHalf.height / t_backgroundSizeHalf.height );
    addChild( t_background );

    auto t_gameLevelSprite = Sprite::create( "GameLevel1.png" );
    t_gameLevelSprite->setPosition( Vec2( 180.0f, t_origin.y + t_visibleSizeHalf.height * 2.0f - 55.0f ) );
    t_gameLevelSprite->setScale( 0.4f );
    addChild( t_gameLevelSprite );


    auto t_gameStopWatchSprite = Sprite::create( "GameStopWatch45.png" );
    auto t_gameStopWatchSizeHalf = t_gameStopWatchSprite->getContentSize() * 0.5f;
    t_gameStopWatchSprite->setPosition( Vec2( t_origin.y + t_visibleSizeHalf.width * 2.0f - t_gameStopWatchSizeHalf.width, t_origin.y + t_visibleSizeHalf.height * 2.0f - t_gameStopWatchSizeHalf.height ) );
    addChild( t_gameStopWatchSprite );

    m_gameTimeBit1Sprite = Sprite::create( "TimeFont/8.png" );
    m_gameTimeBit1Sprite->setPosition( Vec2( t_gameStopWatchSizeHalf.width, t_gameStopWatchSizeHalf.height ) );
    t_gameStopWatchSprite->addChild( m_gameTimeBit1Sprite );

    m_gameTimeBit2Sprite = Sprite::create( "TimeFont/8.png" );
    m_gameTimeBit2Sprite->setPosition( Vec2( t_gameStopWatchSizeHalf.width - 80.0f, t_gameStopWatchSizeHalf.height ) );
    t_gameStopWatchSprite->addChild( m_gameTimeBit2Sprite );


    auto t_gameWatchNeedleSprite = Sprite::create( "GameWatchNeedle.png" );
    auto t_gameWatchNeedleSizeHalf = t_gameWatchNeedleSprite->getContentSize() * 0.5f;
    t_gameWatchNeedleSprite->setPosition( Vec2( t_gameStopWatchSizeHalf.width + 1.0f, t_gameStopWatchSizeHalf.height - 65.0f ) );
    t_gameWatchNeedleSprite->setAnchorPoint( Vec2( 0.5f, 0.9f ) );
    t_gameStopWatchSprite->addChild( t_gameWatchNeedleSprite );

    // t_gameWatchNeedleSprite->runAction( RepeatForever::create( 
    //     ActionFloat::create( 10.0f, 0.0f, 360.0f, [=]( const float p_data ){
    //         t_gameWatchNeedleSprite->setRotation( p_data );
    //     } )
    //  ) );

    auto t_gameTargetScoreBgSprite = Sprite::create( "GameTargetScoreBox.png" );
    auto t_gameTargetScoreBgSizeHalf = t_gameTargetScoreBgSprite->getContentSize() * 0.5f;
    t_gameTargetScoreBgSprite->setPosition( Vec2( 190.0f, t_origin.y + t_visibleSizeHalf.height * 2.0f - 270.0f ) );
    addChild( t_gameTargetScoreBgSprite );

    auto t_gameTargetScoreLabel = Label::createWithTTF( "100", "fonts/MF-MINGHEI-NONCOMMERCIAL-REGULAR.ttf", 120 );
    t_gameTargetScoreLabel->setPosition( Vec2( t_gameTargetScoreBgSizeHalf.width, 120.0f ) );
    t_gameTargetScoreBgSprite->addChild( t_gameTargetScoreLabel );


    auto t_scoreNumber_1 = ScoreNumber::create();
    t_scoreNumber_1->setPosition( Vec2( 735.0f, 921.0f ) );
    addChild( t_scoreNumber_1 );

    auto t_scoreNumber_2 = ScoreNumber::create();
    t_scoreNumber_2->setPosition( Vec2( 965.0f, 921.0f ) );
    addChild( t_scoreNumber_2 );

    auto t_scoreNumber_3 = ScoreNumber::create();
    t_scoreNumber_3->setPosition( Vec2( 1195.0f, 921.0f ) );
    addChild( t_scoreNumber_3 );


    runAction( Sequence::create( DelayTime::create( 2.0f ), 
                CallFunc::create( [=](){
                    AudioEngine::play2d( "audios/InGame.mp3" );
                } ),
    nullptr ) );

    auto t_curtain = DrawNode::create();
    t_curtain->drawSolidRect( t_origin, t_visibleSizeHalf * 2.0f, Color4F( 0.0f, 0.0f, 0.0f, 0.8f ) );
    addChild( t_curtain );

    auto t_promptBoxSprite = Sprite::create( "GamePromptBox.png" );
    auto t_promptBoxSizeHalf = t_promptBoxSprite->getContentSize() * 0.5f;
    t_promptBoxSprite->setPosition( Vec2( t_center.x, t_center.y + t_visibleSizeHalf.height + t_promptBoxSizeHalf.height ) );
    addChild( t_promptBoxSprite );

    auto t_LevelSprite = Sprite::create( "GameLevel1.png" );
    auto t_LevelSizeHalf = t_LevelSprite->getContentSize() * 0.5f;
    t_LevelSprite->setPosition( t_promptBoxSizeHalf );
    t_promptBoxSprite->addChild( t_LevelSprite );

    // t_promptBoxSprite->setVisible( false );

    auto t_time1 = Sprite::create( "Game1.png" );
    t_time1->setPosition( t_center );
    addChild( t_time1 );

    auto t_time2 = Sprite::create( "Game2.png" );
    t_time2->setPosition( t_center );
    addChild( t_time2 );

    auto t_time3 = Sprite::create( "Game3.png" );
    t_time3->setPosition( t_center );
    addChild( t_time3 );

    auto t_timeGo = Sprite::create( "GameGo.png" );
    t_timeGo->setPosition( t_center );
    addChild( t_timeGo );

    t_time1->setVisible( false );
    t_time2->setVisible( false );
    t_time3->setVisible( false );
    t_timeGo->setVisible( false );


    auto t_CountDown = [=](){

        auto t_readyTime = Sequence::create( 

            CallFunc::create( [=](){
                t_time3->setScale( 5.0f );
                t_time3->setOpacity( 0 );
                t_time3->setVisible( true );
            } ),

            ActionFloat::create( 1.0f, 1.0f, 0.0f, [=]( float p_data ){
                t_promptBoxSprite->setPosition( Vec2( t_center.x, t_center.y + ( t_promptBoxSizeHalf.height + t_visibleSizeHalf.height ) * p_data ) );
            } ),

            DelayTime::create( 1.6f ),

            ActionFloat::create( 0.1f, 1.0f, 0.0f, [=]( float p_data ){
                t_promptBoxSprite->setOpacity( (int)( 255 * p_data ) );
                t_LevelSprite->setOpacity( (int)( 255 * p_data ) );
            } ),

            DelayTime::create( 0.6f ),

            ActionFloat::create( 0.3f, 0.0f, 1.0f, [=]( float p_data ){

                t_time3->setScale( 5.0f - 4.0f * p_data );
                t_time3->setOpacity( (int)( 255 * p_data ) );
            } ),
            DelayTime::create( 0.6f ),

            ActionFloat::create( 0.1f, 1.0f, 0.0f, [=]( float p_data ){
                t_time3->setOpacity( (int)( 255 * p_data ) );
            } ),

            CallFunc::create( [=](){
                t_time2->setScale( 5.0f );
                t_time2->setOpacity( 0 );
                t_time2->setVisible( true );
            } ),

            ActionFloat::create( 0.3f, 0.0f, 1.0f, [=]( float p_data ){

                t_time2->setScale( 5.0f - 4.0f * p_data );
                t_time2->setOpacity( (int)( 255 * p_data ) );
            } ),
            DelayTime::create( 0.6f ),

            ActionFloat::create( 0.1f, 1.0f, 0.0f, [=]( float p_data ){
                t_time2->setOpacity( (int)( 255 * p_data ) );
            } ),

            CallFunc::create( [=](){
                t_time1->setScale( 5.0f );
                t_time1->setOpacity( 0 );
                t_time1->setVisible( true );
            } ),

            ActionFloat::create( 0.3f, 0.0f, 1.0f, [=]( float p_data ){

                t_time1->setScale( 5.0f - 4.0f * p_data );
                t_time1->setOpacity( (int)( 255 * p_data ) );
            } ),
            DelayTime::create( 0.6f ),

            ActionFloat::create( 0.1f, 1.0f, 0.0f, [=]( float p_data ){
                t_time1->setOpacity( (int)( 255 * p_data ) );
            } ),

            CallFunc::create( [=](){
                t_timeGo->setScale( 5.0f );
                t_timeGo->setOpacity( 0 );
                t_timeGo->setVisible( true );
            } ),

            ActionFloat::create( 0.3f, 0.0f, 1.0f, [=]( float p_data ){

                t_timeGo->setScale( 5.0f - 4.0f * p_data );
                t_timeGo->setOpacity( (int)( 255 * p_data ) );
            } ),
            DelayTime::create( 0.6f ),

            ActionFloat::create( 0.1f, 1.0f, 0.0f, [=]( float p_data ){
                t_timeGo->setOpacity( (int)( 255 * p_data ) );
                t_curtain->setOpacity( (int)( 255 * p_data ) );
            } ),

            CallFunc::create( [=](){
                m_gameState = GameState::TPT;
            } ),
        
        nullptr );

        runAction( t_readyTime );
    };

    DeviceControl::openBaffle( [=](){
        runAction( Sequence::create( 

            DelayTime::create( 2.0f ),
            
            CallFunc::create( [=](){
                t_CountDown();
            } ),
            nullptr
         ) );
    } );


    sm_listenButtonId = DeviceControl::listenButtonState( [=]( int p_btnId, bool p_state ){
        if( p_btnId == BTN_START && p_state )
        {
            DeviceControl::closeBaffle([]{
            
            });
            Director::getInstance()->replaceScene( TransitionFade::create( 3.0f, MainScene::create() ) );

            DeviceControl::unbindListenButtonState( sm_listenButtonId );
        }
        else if( p_btnId == BTN_GOAL && p_state ){

            if( m_gameState != GameState::TPT )
            {
                return;
            }

            m_gameScore += 1;

            unsigned short t_hundred = m_gameScore / 100 % 10;
            unsigned short t_shiwei = m_gameScore / 10 % 10;
            unsigned short t_unit = m_gameScore % 10;

            t_scoreNumber_1->setNumber( t_hundred );
            t_scoreNumber_2->setNumber( t_shiwei );
            t_scoreNumber_3->setNumber( t_unit );
        }
    } );


    schedule( schedule_selector( GameScene::update ) );

    return true;
}

void GameScene::update( float p_dt )
{
    static int st_gameTime = 0;
    if( m_gameState == GameState::TPT )
    {

        if( m_gameTime - p_dt < 0 )
        {
            m_gameState = GameState::End;
            return;
        }

        m_gameTime -= p_dt;
    }

    if( floor( m_gameTime ) != st_gameTime )
    {
        st_gameTime = floor( m_gameTime );
        std::stringstream sstr;
        sstr << "TimeFont/" << st_gameTime / 10 % 10 << ".png";
        m_gameTimeBit2Sprite->setTexture( sstr.str() );
        sstr.str( "" );
        sstr << "TimeFont/" << st_gameTime % 10 << ".png";
        m_gameTimeBit1Sprite->setTexture( sstr.str() );
    }
}