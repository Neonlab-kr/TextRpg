#pragma once
#include <iostream>
#include <stdexcept>
#include <string>
#include <mysql.h>

#pragma comment(lib, "libmysql.lib")

#define DB_HOST ""
#define DB_USER ""
#define DB_PASS ""
#define DB_NAME ""

class DBconn
{
private:
    MYSQL* conn_;
    std::string host_;
    std::string user_;
    std::string password_;
    std::string database_;

public:
    DBconn(const std::string& host = DB_HOST, const std::string& user = DB_USER, const std::string& password = DB_PASS, const std::string& database = DB_NAME);
    ~DBconn();

    bool executeQuery(const std::string& query);
    int getAffectedRows();
    MYSQL_RES* storeResult();
};
