#include "Config.h"
#include "DataBase.h"
#include <iostream>
#include <sstream>

#define CONFIG_ID "ty_config_01"

int Config::sm_CoinNumber = 0;

bool Config::init( void )
{
    if( !DataBase::init() )
    {
        return false;
    }

    const char * t_createSql = "CREATE TABLE IF NOT EXISTS  Config( "\
                                "Id TEXT PRIMARY KEY NOT NULL, "\
                                "CoinNumber INTEGER NOT NULL"\
                                " )";

    if( !DataBase::exec( t_createSql ) )
    {
        std::cout << "create config table final" << std::endl;
        return false;
    }


    const char * t_selectSql = "SELECT * FROM Config WHERE Id=\"" CONFIG_ID "\"";


    auto t_data = DataBase::query( t_selectSql );

    if( t_data.size() <= 0 )
    {
        const char * t_insertSql = "INSERT INTO Config(Id, CoinNumber)  VALUES( \"" CONFIG_ID "\", 0 )";
        if( !DataBase::exec( t_insertSql ) )
        {
            std::cout << "init config data final" << std::endl;
            return false;
        }
    }else{
        sm_CoinNumber = atoi( t_data[0]["CoinNumber"].c_str() );
    }

    return true;
}


bool Config::setCoinNumber( const int p_coin )
{
    if( p_coin == sm_CoinNumber )
    {
        return true;
    }

    std::stringstream t_updateSqlSStr;

    t_updateSqlSStr << "UPDATE Config SET CoinNumber = " << p_coin << " WHERE Id = \"" << CONFIG_ID << "\"";

    if( !DataBase::exec( t_updateSqlSStr.str() ) )
    {
        return false;
    }

    sm_CoinNumber = p_coin;

    return true;
}

int Config::getCoinNumber( void )
{
    return sm_CoinNumber;
}