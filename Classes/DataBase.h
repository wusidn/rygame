#ifndef __DATA_BASE_H__
#define __DATA_BASE_H__

#include "sqlite3.h"
#include <string>
#include <vector>
#include <map>
#include <mutex>

class DataBase
{
public:
    static const std::string sm_dbVersion;
    typedef std::vector< std::map<std::string, std::string> > QueryBack;

    static bool init( void );

    static bool exec( const std::string & p_sql );
    static QueryBack query( const std::string & p_sql );

private:
    static int callback( void *NotUsed, int argc, char **argv, char **azColName );
    
    static sqlite3 * sm_dataBase;
    static QueryBack * sm_queryBack;
    static std::mutex sm_mutex;
};

#endif //__DATA_BASE_H__