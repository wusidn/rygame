#ifndef __DEVICE_CONTROL_H__
#define __DEVICE_CONTROL_H__

#include <cstddef>
#include <functional>
#include <map>
#include <vector>

class DeviceControl
{
public:
    static bool init( void );

    static bool listenButtonState( std::function< void( int, bool ) > p_callBack );

private:
    static bool sm_needInitDevice;
    static void onButtonStateChanged( int p_btnId, bool p_state );
    static bool cmdCompare( const unsigned char * p_cmdLeft, const unsigned char * p_cmdRight, size_t p_compareSize );

    static unsigned char sm_cmdStartDeviceListeanRes[];
    static unsigned char sm_cmdButtonStateChanged[];

    static std::vector< std::function< void( int, bool ) > > sm_buttonStateListenPool;
};



#endif //__DEVICE_CONTROL_H__