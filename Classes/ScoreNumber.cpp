#include "ScoreNumber.h"

bool ScoreNumber::init( void )
{
    if( !Node::init() )
    {
        return false;
    }

    m_numberUp = Sprite::create( "Score/up_0.png" );
    auto t_numberUpSizeHalf = m_numberUp->getContentSize() * 0.5f;
    m_numberUp->setPosition( Vec2( 0.0f, t_numberUpSizeHalf.height + 0.5f ) );
    addChild( m_numberUp );

    // m_numberMid = Sprite::create( "Score/up_0.png" );
    // m_numberMid->setPosition( Vec2( 0.0f, t_numberUpSizeHalf.height ) );
    // addChild( m_numberMid );

    m_numberDown = Sprite::create( "Score/down_0.png" );
    auto t_numberDownSizeHalf = m_numberDown->getContentSize() * 0.5f;
    m_numberDown->setPosition( Vec2( 0.0f, -t_numberUpSizeHalf.height - 0.5f ) );
    addChild( m_numberDown );

    return true;
}


void ScoreNumber::setNumber( unsigned short p_num )
{
    std::stringstream t_sstr;
    t_sstr << "Score/up_" << ( p_num % 10 ) << ".png";
    m_numberUp->setTexture( t_sstr.str() );

    t_sstr.str("");
    t_sstr << "Score/down_" << ( p_num % 10 ) << ".png";
    m_numberDown->setTexture( t_sstr.str() );
}