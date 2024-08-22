#include "DBconn.h"

DBconn::DBconn(const std::string& host, const std::string& user, const std::string& password, const std::string& database) : host_(host), user_(user), password_(password), database_(database)
{
    conn_ = mysql_init(nullptr);
    if (conn_ == nullptr) {
        throw std::runtime_error("mysql_init() failed");
    }

    if (mysql_real_connect(conn_, host_.c_str(), user_.c_str(), password_.c_str(),
        database_.c_str(), 3306, nullptr, 0) == nullptr) {
        std::string error_message = "mysql_real_connect() failed: ";
        error_message += mysql_error(conn_);
        throw std::runtime_error(error_message);
    }
}

DBconn::~DBconn()
{
    if (conn_) {
        mysql_close(conn_);
    }
}

bool DBconn::executeQuery(const std::string& query)
{
    if (mysql_query(conn_, query.c_str()))
    {
        std::string error_message = "mysql_query() failed: ";
        error_message += mysql_error(conn_);
        throw std::runtime_error(error_message);
        return true;
    }
    return false;
}

int DBconn::getAffectedRows()
{
    return mysql_affected_rows(conn_);
}

MYSQL_RES* DBconn::storeResult()
{
    MYSQL_RES* result = mysql_store_result(conn_);
    if (result == nullptr)
    {
        std::string error_message = "mysql_store_result() failed: ";
        error_message += mysql_error(conn_);
        throw std::runtime_error(error_message);
    }
    return result;
}