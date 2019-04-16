#include "DataBase.h"
#include <iostream>

sqlite3 * DataBase::sm_dataBase = nullptr;

DataBase::QueryBack * DataBase::sm_queryBack = nullptr;

const std::string DataBase::sm_dbVersion = "v1.1";

std::mutex DataBase::sm_mutex;

bool DataBase::init( void )
{
    int t_openRet = sqlite3_open( "ry_db", &sm_dataBase );

    if( t_openRet )
    {
        std::cout << "open sqlite final" << std::endl;
        return false;
    }

    return true;
}

int DataBase::callback( void *NotUsed, int argc, char **argv, char **azColName )
{

    std::map< std::string, std::string > t_row;
    int i;
    for( i=0; i<argc; i++ )
    {
        t_row[ azColName[i] ] = argv[i] ? argv[i] : "NULL";
    }
    sm_queryBack->push_back( t_row );

    return 0;
}

bool DataBase::exec( const std::string & p_sql )
{
    char * t_ErrMsg = nullptr;
    // sm_mutex.lock();
    int t_res = sqlite3_exec( sm_dataBase, p_sql.c_str(), nullptr, 0, &t_ErrMsg);
    // sm_mutex.unlock();
    if( t_res != SQLITE_OK )
    {
        printf( "SQL error: %s\n", t_ErrMsg);
        sqlite3_free( t_ErrMsg );
        return false;
    }

    return true;
}

DataBase::QueryBack DataBase::query( const std::string & p_sql )
{
    QueryBack t_result;
    sm_queryBack = &t_result;
    char * t_ErrMsg = nullptr;
    int t_res = sqlite3_exec( sm_dataBase, p_sql.c_str(), callback, 0, &t_ErrMsg);
    if( t_res != SQLITE_OK )
    {
        printf( "SQL error: %s\n", t_ErrMsg);
        sqlite3_free( t_ErrMsg );
    }

    return t_result;
}