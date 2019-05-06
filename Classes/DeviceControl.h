#ifndef __DEVICE_CONTROL_H__
#define __DEVICE_CONTROL_H__

#include <cstddef>
#include <functional>
#include <map>


#define BTN_START 0x07
#define BTN_COIN  0x03
#define BTN_GOAL  0x06


class DeviceControl
{
public:
    static bool init( void );

    static int listenButtonState( std::function< void( int, bool ) > p_callBack );
    static bool unbindListenButtonState( const int p_id );

    static void openBaffle( std::function< void(void) > p_opendCallback );
    static void closeBaffle( std::function< void(void) > p_closedCallback );

private:

    static bool sm_needInitDevice;
    static void onButtonStateChanged( int p_btnId, bool p_state );
    static bool cmdCompare( const unsigned char * p_cmdLeft, const unsigned char * p_cmdRight, size_t p_compareSize );

    static unsigned char sm_cmdStartDeviceListeanRes[];
    static unsigned char sm_cmdButtonStateChanged[];

    static unsigned char sm_cmdOpenRelay1[], sm_cmdCloseRelay1[];

    static std::map< int, std::function< void( int, bool ) > > sm_buttonStateListenPool;
};



#endif //__DEVICE_CONTROL_H__