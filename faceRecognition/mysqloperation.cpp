#include "mysqloperation.h"
#include "mysqloperation.h"


MysqlOperation::MysqlOperation()
{
	
}

void MysqlOperation::connect2database()
{
	//初始化数据库
	if (0 != mysql_library_init(0, NULL, NULL))
	{
		std::cerr << "MysqlOperation: " << "mysql_library_init() failed!" << std::endl;
		return;
	}
	//初始化数据结构
	if (NULL == mysql_init(&mydata))
	{
		std::cerr << "MysqlOperation: " << "mysql_init() failed!" << std::endl;
		return;
	}
	//数据库连接之前的个性化操作，设置字符集
	if (0 != mysql_options(&mydata, MYSQL_SET_CHARSET_NAME, "gbk"))
	{
		std::cerr << "MysqlOperation: " << "mysql_options() failed !" << std::endl;
	}

	//连接数据库
	if (NULL == mysql_real_connect(&mydata, "localhost", "root", "120629", "facedatabase", 3306, NULL, 0))
	{
		std::cerr << "MysqlOperation: " << "mysql_real_connect failed!" << std::endl;
		return;
	}
}

void MysqlOperation::insertOperation(std::string sqlStr, int count)
{
	if (count % 1000 == 0)
	{
		mysql_query(&mydata, "START TRANSACTION");
	}	
	if (0 != mysql_query(&mydata, sqlStr.c_str()))
	{
		std::cerr << "insertOperation: " << "insert data failed !" << std::endl;
		//mysql_close(&mydata);
		return ;
	}
	if ((count + 1) % 1000 == 0)
	{
		mysql_query(&mydata, "COMMIT");
	}
}

void MysqlOperation::queryOperation(std::string sqlStr)
{
	MYSQL_RES *result = NULL;
	if (0 == mysql_query(&mydata, sqlStr.c_str()))
	{
		std::cout << "queryOperation: " << "mysql_query() select data succeed !" << std::endl;

		//一次性取得数据集
		result = mysql_store_result(&mydata);

		//取得并打印行数
		int rowcount = mysql_num_rows(result);
		std::cout << "queryOperation: " << "row count: " << rowcount << std::endl;

		//取得并打印各字段的名称
		unsigned int fieldcount = mysql_num_fields(result);
		MYSQL_FIELD *field = NULL;
		for (unsigned int i = 0; i < fieldcount; i++) {
			field = mysql_fetch_field_direct(result, i);
			std::cout << field->name << "\t\t";
		}
		std::cout << std::endl;

		//打印各行
		MYSQL_ROW row = NULL;
		row = mysql_fetch_row(result);
		while (NULL != row) {
			for (int i = 0; i < fieldcount; i++) {
				std::cout << row[i] << "\t\t";
			}
			std::cout << std::endl;
			row = mysql_fetch_row(result);
		}

	}
	else {
		std::cerr << "queryOperation: " << "mysql_query() select data failed" << std::endl;
		mysql_close(&mydata);
		return;
	}

	mysql_free_result(result);
	//mysql_close(&mydata);
}

MysqlOperation::~MysqlOperation()
{
	mysql_close(&mydata);
	mysql_server_end();
}
