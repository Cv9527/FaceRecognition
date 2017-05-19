#pragma once

#include <mysql.h>
#include <string>
#include <iostream>
class MysqlOperation
{
public:
	MysqlOperation();

	void connect2database();

	void insertOperation(std::string sqlStr, int count);

	void queryOperation(std::string sqlStr);

	~MysqlOperation();

public:
	MYSQL mydata;
};

