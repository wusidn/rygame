#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "unistd.h"
#include <functional>
#include <list>

class Serial
{
public:

    typedef std::function<void(const unsigned char * p_data, const size_t p_dataSize )> READ_LISTENER_FUNC;

    static bool open( void );

    static bool write( const unsigned char * p_data, size_t p_dataSize );

    static bool bindRecvDataFunc( READ_LISTENER_FUNC p_func );

private:

    static bool sendRecvDataEvent( const unsigned char * p_data, size_t p_dataSize );
    static unsigned char computeCheckBit( const unsigned char * p_data, size_t p_dataSize );

    static std::list< READ_LISTENER_FUNC > sm_readDataListenerList;
    static int sm_fd;
};


#endif //__SERIAL_H__