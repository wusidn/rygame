#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class LoadingScene : public Scene
{
public:
    CREATE_FUNC( LoadingScene );
    
protected:
    virtual bool init( void ) override;
    
private:
    static bool sm_needInitDevice;

    static bool cmdCompare( const unsigned char * p_cmdLeft, const unsigned char * p_cmdRight, size_t p_compareSize );

    static unsigned char sm_cmdStartDeviceListeanRes[];
};

#endif //__LOADING_SCENE_H__