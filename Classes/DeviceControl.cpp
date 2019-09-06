#include "DeviceControl.h"

#include "Serial.h"
#include <thread>
#include <iostream>
#include "cocos2d.h"
#include "MainScene.h"
#include "GameScene.h"

USING_NS_CC;

bool DeviceControl::sm_needInitDevice = true;

unsigned char DeviceControl::sm_cmdStartDeviceListeanRes[] = { (unsigned char)0x00, 0x01, 0x01, 0xFF };
unsigned char DeviceControl::sm_cmdButtonStateChanged[] = { (unsigned char)0x00, 0x0A, 0x02 };

unsigned char DeviceControl::sm_cmdOpenRelay1[] = { (unsigned char)0x10, 0x12, 0x03, 0xFF, 0xFF, 0x0D };
unsigned char DeviceControl::sm_cmdCloseRelay1[] = { (unsigned char)0x10, 0x12, 0x03, 0x00, 0x00, 0x0D };


// 0-9 . -
unsigned char DeviceControl::sm_ledNumberMask[] = { (unsigned char)0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x80, 0x40 };


std::map< int, std::function< void( int, bool ) > >  DeviceControl::sm_buttonStateListenPool;

bool DeviceControl::init()
{
 //

    std::thread([](){

        if( !Serial::open() )
        {
            std::cout << "open serial final" << std::endl;
            return;
        }

        Serial::bindRecvDataFunc( []( const unsigned char * p_data, size_t p_dataSize ){
            
            if( sm_needInitDevice && p_dataSize == 6 && cmdCompare( p_data + 1, sm_cmdStartDeviceListeanRes, sizeof( sm_cmdStartDeviceListeanRes ) ) )
            {
                sm_needInitDevice = false;

                Director::getInstance()->getScheduler()->performFunctionInCocosThread([]
                {
                    Director::getInstance()->replaceScene( MainScene::create() );
                });
            }

            if( cmdCompare( p_data + 1, sm_cmdButtonStateChanged, sizeof( sm_cmdButtonStateChanged ) ) )
            {
                unsigned int t_buttonId = (unsigned int)p_data[4];
                bool t_state = p_data[5] == 0x00;
                onButtonStateChanged( t_buttonId, t_state );
            }

        } );

        unsigned char write_buf[] = { (unsigned char)0x01, 0x01, 0x01, 0xFF };

        do{

            Serial::write( write_buf, sizeof( write_buf ) );
            
            sleep( 2 );
        }while( sm_needInitDevice );

    }).detach();
    
    return true;
}


int DeviceControl::listenButtonState( std::function< void( int, bool ) > p_callBack )
{
    static int s_listenId = 0;

    sm_buttonStateListenPool[s_listenId] = p_callBack;

    return s_listenId++;
} 


bool DeviceControl::unbindListenButtonState( int p_listenId )
{
    auto t_it = sm_buttonStateListenPool.find( p_listenId );
    if( t_it == sm_buttonStateListenPool.end() )
    {
        return false;
    }

    sm_buttonStateListenPool.erase( t_it );

    return true;
}

void DeviceControl::openBaffle( std::function< void(void) > p_opendCallback )
{
    static int t_listenId;
    static std::function< void(void) > t_opendFunc;
    t_opendFunc = p_opendCallback;
    Serial::write( sm_cmdOpenRelay1, sizeof( sm_cmdOpenRelay1 ) );


    t_listenId = listenButtonState( [=]( int p_btnId, bool p_state ){
        if( p_btnId == BTN_BAFFLE_DOWN && p_state )
        {
            Serial::write( sm_cmdCloseRelay1, sizeof( sm_cmdCloseRelay1 ) );
            
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
            {
                t_opendFunc();
            });
            unbindListenButtonState( t_listenId );
        }
    } );
}

void DeviceControl::closeBaffle( std::function< void(void) > p_closedCallback )
{
    static int t_listenId;
    static std::function< void(void) > t_closedFunc;
    t_closedFunc = p_closedCallback;
    Serial::write( sm_cmdOpenRelay1, sizeof( sm_cmdOpenRelay1 ) );

    t_listenId = listenButtonState( [=]( int p_btnId, bool p_state ){
        if( p_btnId == BTN_BAFFLE_UP && p_state )
        {
            Serial::write( sm_cmdCloseRelay1, sizeof( sm_cmdCloseRelay1 ) );
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
            {
                t_closedFunc();
            });
            unbindListenButtonState( t_listenId );
        }
    } );
}

void DeviceControl::showCoin( const unsigned int p_coin )
{
    std::stringstream t_sstr;
    t_sstr << p_coin;

    unsigned char t_cmd[] = { (unsigned char)0x30, 0x012, 0x05, 0x01, sm_ledNumberMask[0], sm_ledNumberMask[0], sm_ledNumberMask[0], sm_ledNumberMask[0] };

    //for( int i = 0; i <  )

    Serial::write( t_cmd, sizeof( t_cmd ) );
}


void DeviceControl::onButtonStateChanged( int p_btnId, bool p_state )
{
    for( auto item : sm_buttonStateListenPool )
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
        {
            item.second( p_btnId, p_state );
        });
    }
}

 bool DeviceControl::cmdCompare( const unsigned char * p_cmdLeft, const unsigned char * p_cmdRight, size_t p_compareSize )
 {
     bool t_result = true;
     for( int i = 0; i < p_compareSize; ++i )
     {
         if( p_cmdLeft[i] == p_cmdRight[i] )
         {
             continue;
         }
         t_result = false;
         break;
     }

     return t_result;
 }
