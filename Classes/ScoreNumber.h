#include "cocos2d.h"


USING_NS_CC;

class ScoreNumber : public Node
{
public:
    CREATE_FUNC( ScoreNumber );


    void setNumber( unsigned short p_num );

protected:
    virtual bool init( void ) override;

private:
    Sprite * m_numberUp, * m_numberMid, * m_numberDown;
};