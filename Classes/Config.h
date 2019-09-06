#ifndef __CONFIG_H__
#define __CONFIG_H__


#define FREE_GAME 0

class Config
{
public:
    static bool init( void );

    static bool setCoinNumber( const int p_coin );
    static int getCoinNumber( void );
private:
    static int sm_CoinNumber; 
};

#endif //__CONFIG_H__