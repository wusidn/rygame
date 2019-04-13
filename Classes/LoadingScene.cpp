#include "LoadingScene.h"
#include "Serial.h"
#include <thread>
#include <iostream>

USING_NS_CC;
using namespace cocos2d::ui;


bool LoadingScene::sm_needInitDevice = true;

unsigned char LoadingScene::sm_cmdStartDeviceListeanRes[] = { (unsigned char)0x00, 0x01, 0x01, 0xFF };

bool LoadingScene::init( void )
{
    if( !Scene::init() )
    {
        return false;
    }

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


 bool LoadingScene::cmdCompare( const unsigned char * p_cmdLeft, const unsigned char * p_cmdRight, size_t p_compareSize )
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

