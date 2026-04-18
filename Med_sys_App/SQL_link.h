#pragma once

#include <mysql.h>  
#include <vector>
#include "drugs_info.h"
#define SQL_MAX 1024   // 定义SQL语句最大长度

class SQL_link
{
public:
    MYSQL_RES* res;
    MYSQL_ROW row;
    MYSQL mysqlCon;
	char sql[SQL_MAX];    // 存储SQL语句的缓冲区

	bool Connect_MySQL();        // 连接数据库
	bool Connect_MySQL_();
	bool CheckLogin(CString username, CString password);  // 检查登录信息
	
	vector<drugs_info> Get_All_Info();       // 获取所有信息
	bool AddDrug(const drugs_info& info);    //添加药品
	bool DeleteDrug(int id);                 //删除药品
	bool UpdateDrug(int oldId, const drugs_info& new_info);    //更新药品

	// 定义一个结构体方便传递用户信息
	struct AdminInfo {
		CString username;
		CString password;
	};

	// ... 在 SQL_link 类中添加 ...
	vector<AdminInfo> GetAllAdmins();           // 获取所有管理员
	bool AddAdmin(CString name, CString psw);   // 增加管理员
	bool DeleteAdmin(CString name);             // 删除管理员
};

