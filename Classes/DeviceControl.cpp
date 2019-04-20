#include "DeviceControl.h"

#include "Serial.h"
#include <thread>
#include <iostream>
#include "cocos2d.h"
#include "MainScene.h"

USING_NS_CC;

bool DeviceControl::sm_needInitDevice = true;

unsigned char DeviceControl::sm_cmdStartDeviceListeanRes[] = { (unsigned char)0x00, 0x01, 0x01, 0xFF };
unsigned char DeviceControl::sm_cmdButtonStateChanged[] = { (unsigned char)0x00, 0x0A, 0x02 };


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
