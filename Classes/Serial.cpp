#include "Serial.h"
#include <thread>
#include <iostream>
#include <cstdlib>
#include <fcntl.h>
#include <termios.h>
#include <string>
#include <cstring>
#include <sys/time.h>
#include <functional>

using namespace std;

#define SERIAL_FILE "/dev/ttyUSB0"
#define BAUD_RATE 57600
#define BYTE_SIZE 8
#define CHECK_TYPE 'N'
#define STOP_SIZE 1

int set_opt(int fd,int nSpeed,int nBits,char nEvent,int nStop);

int Serial::sm_fd = -1;
list< Serial::READ_LISTENER_FUNC > Serial::sm_readDataListenerList;

bool Serial::open( void )
{
    int ret;
    unsigned char write_buf[] = { (unsigned char)0xF0, 0x01, 0x01, 0x01, 0xFF, 0x02 };

    sm_fd = ::open( SERIAL_FILE, O_RDWR|O_NOCTTY);
    if(sm_fd == -1)
    {
        cout << "open " << SERIAL_FILE << " fianl" << endl;
        return false;
    }

    ret = set_opt(sm_fd,BAUD_RATE,BYTE_SIZE,CHECK_TYPE,STOP_SIZE);
    if(ret == -1)
    {
        cout << "set serial opt final" << endl;
        return false;
    }

    write( write_buf, sizeof( write_buf ) );

    unsigned char Rx_Data[100];
    unsigned char cmd_buffer[ 20 ];
    int cmd_buffer_cursor = 0;
    int t_cmd_len = -1;

    while(1)
    {
        ret = read( sm_fd, Rx_Data, 100);
        if( ret >0 )
        {
            for( int i = 0; i < ret; ++i )
            {

                if( cmd_buffer_cursor == 0 && Rx_Data[i] != 0x0F )
                {
                    continue;
                }

                cmd_buffer[cmd_buffer_cursor++] = Rx_Data[i];

                if( cmd_buffer_cursor == 4 )
                {
                    t_cmd_len = 5 + Rx_Data[i];
                }

                if( cmd_buffer_cursor > 5 && cmd_buffer_cursor == t_cmd_len )
                {

                    int t_check = 0;

                    for( int n = 1; n < t_cmd_len - 1; ++n )
                    {
                        t_check += cmd_buffer[n];
                    }

                    if( (t_check << 24) >> 24 == cmd_buffer[t_cmd_len - 1] )
                    {

                        cout << "----> cmd:";
                        for( int n = 0; n < t_cmd_len; ++n )
                        {
                            cout << (unsigned short)cmd_buffer[n] << " ";
                        }
                        cout << endl;

                    }

                    cmd_buffer_cursor = 0;
                }
            }
        }
    }

    return true;
}

bool Serial::write( const unsigned char * p_data, size_t p_dataSize )
{
    if( sm_fd == -1 )
    {
        return false;
    }
    ::write( sm_fd, p_data, p_dataSize );

    return true;
}

int set_opt(int fd,int nSpeed,int nBits,char nEvent,int nStop)
{
    struct termios newtio,oldtio;
    if(tcgetattr(fd,&oldtio)!=0)
    {
        perror("error:SetupSerial 3\n");
        return -1;
    }
    bzero(&newtio,sizeof(newtio));
    //使能串口接收
    newtio.c_cflag |= CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;

    newtio.c_lflag &=~ICANON;//原始模式

    //newtio.c_lflag |=ICANON; //标准模式

    //设置串口数据位
    switch(nBits)
    {
        case 7:
            newtio.c_cflag |= CS7;
            break;
        case 8:
            newtio.c_cflag |=CS8;
            break;
    }
    //设置奇偶校验位
    switch(nEvent)

    {
        case 'O':
            newtio.c_cflag |= PARENB;
            newtio.c_cflag |= PARODD;
            newtio.c_iflag |= (INPCK | ISTRIP);
            break;
        case 'E':
            newtio.c_iflag |= (INPCK | ISTRIP);
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &= ~PARODD;
            break;
        case 'N':
            newtio.c_cflag &=~PARENB;
            break;
    }
    //设置串口波特率
    switch(nSpeed)
    {
        case 2400:
            cfsetispeed(&newtio,B2400);
            cfsetospeed(&newtio,B2400);
            break;
        case 4800:
            cfsetispeed(&newtio,B4800);
            cfsetospeed(&newtio,B4800);
            break;
        case 9600:
            cfsetispeed(&newtio,B9600);
            cfsetospeed(&newtio,B9600);
            break;
        case 57600:
            cfsetispeed(&newtio,B57600);
            cfsetospeed(&newtio,B57600);
            break;
        case 115200:
            cfsetispeed(&newtio,B115200);
            cfsetospeed(&newtio,B115200);
            break;
        case 460800:
            cfsetispeed(&newtio,B460800);
            cfsetospeed(&newtio,B460800);
            break;
        default:
            cfsetispeed(&newtio,B9600);
            cfsetospeed(&newtio,B9600);
            break;
    }
    //设置停止位
    if(nStop == 1)
        newtio.c_cflag &= ~CSTOPB;
    else if(nStop == 2)
        newtio.c_cflag |= CSTOPB;
    newtio.c_cc[VTIME] = 1;
    newtio.c_cc[VMIN] = 0;
    tcflush(fd,TCIFLUSH);

    if(tcsetattr(fd,TCSANOW,&newtio)!=0)
    {
        perror("com set error\n");
        return -1;
    }
    return 0;
}