#include "pch.h"
#include "SQL_link.h"


bool SQL_link::Connect_MySQL()
{
	// 初始化 MySQL 连接对象
    memset(&mysqlCon, 0, sizeof(mysqlCon));  
    mysql_init(&mysqlCon);

    mysql_options(&mysqlCon, MYSQL_SET_CHARSET_NAME, "utf8mb4");
    if (!mysql_real_connect(&mysqlCon, "localhost", "root", "732631jjx", "uers_sys", 3306, NULL, 0)) {
        //AfxMessageBox(_T("连接数据库失败！"));
        //mysql_query(&mysqlCon, "SET NAMES 'GB2312'");
        return false;
    }
    else {
        //AfxMessageBox(L"连接数据库成功！");
        mysql_query(&mysqlCon, "SET NAMES 'utf8mb4'"); 
    }
    return true;
}
bool SQL_link::Connect_MySQL_()
{
    mysql_init(&mysqlCon);
    mysql_options(&mysqlCon, MYSQL_SET_CHARSET_NAME, "utf8mb4");
    if (!mysql_real_connect(&mysqlCon, "localhost", "root", "732631jjx", "drugs_info", 3306, NULL, 0)) {
        //AfxMessageBox(_T("连接数据库失败！"));
        //mysql_query(&mysqlCon, "SET NAMES 'GB2312'");
        return false;
    }
    else {
        //AfxMessageBox(L"连接数据库成功！");
        mysql_query(&mysqlCon, "SET NAMES 'utf8mb4'");
    }
    return true;
}

bool SQL_link::CheckLogin(CString username, CString password)
{
    CStringA sql;

    CStringA userA(username);
    CStringA pswA(password);

    sql.Format("SELECT * FROM users_info WHERE m_username='%s' AND m_userpsw='%s'",
        userA.GetString(), pswA.GetString());
    // 执行SQL
    if (mysql_query(&mysqlCon, sql))
    {
        AfxMessageBox(_T("查询失败！"));
        return false;
    }

    // 获取结果
    MYSQL_RES* res = mysql_store_result(&mysqlCon);

    if (res == NULL)
    {
        AfxMessageBox(_T("获取结果失败！"));
        return false;
    }

    // 判断是否有数据
    int row = mysql_num_rows(res);

    mysql_free_result(res);

    if (row > 0)
    {
        return true;   // 登录成功
    }
    else
    {
        return false;  // 登录失败
    }
}

//vector<drugs_info> SQL_link::Get_All_Info()
//{
//    vector<drugs_info> Info;
//   snprintf(sql, SQL_MAX, "SELECT * FROM drugs_info");
//   // 查询数据
//   int ret = mysql_real_query(&mysqlCon, sql, (unsigned long)strlen(sql));
//   // 获取结果
//   if (ret){
//           AfxMessageBox(_T("查询失败！"));
//           return Info;
//   }
//   // 获取结果
//   res = mysql_store_result(&mysqlCon);
//
//   while (row = mysql_fetch_row(res)) {
//       drugs_info druginfo(
//           atoi(row[0]),           // 编号
//           row[1],                 // 名称
//           row[2],                 // 类型
//           atoi(row[3]),           // 数量
//           atoi(row[4])            // 价格
//       );
//	   Info.push_back(druginfo);
//   }
//   mysql_free_result(res);
//
//   return Info;
//}
vector<drugs_info> SQL_link::Get_All_Info()
{
    vector<drugs_info> Info;

    snprintf(sql, SQL_MAX, "SELECT * FROM `drugs_info`");

    int ret = mysql_real_query(&mysqlCon, sql, (unsigned long)strlen(sql));
    if (ret)
    {
        CString msg;
        msg.Format(_T("查询失败：%S"), mysql_error(&mysqlCon));
        AfxMessageBox(msg);
        return Info;
    }

    res = mysql_store_result(&mysqlCon);
    if (res == NULL)
    {
        CString msg;
        msg.Format(_T("获取结果失败：%S"), mysql_error(&mysqlCon));
        AfxMessageBox(msg);
        return Info;
    }

    while ((row = mysql_fetch_row(res)) != nullptr)
    {
        drugs_info druginfo(
            atoi(row[0]),
            row[1] ? row[1] : "",
            row[2] ? row[2] : "",
            row[3] ? atoi(row[3]) : 0,
            row[4] ? atoi(row[4]) : 0
        );
        Info.push_back(druginfo);
    }

    mysql_free_result(res);
    return Info;
}

bool SQL_link::AddDrug(const drugs_info& info)
{
    snprintf(sql, SQL_MAX,
        "INSERT INTO drugs_info VALUES (%d, '%s', '%s', %d, %d)",
        info.m_id,
        info.m_name.c_str(),
        info.m_kind.c_str(),
        info.m_num,
        info.m_rmb);

    int ret = mysql_real_query(&mysqlCon, sql, (unsigned long)strlen(sql));
    if (ret)
    {
        CString msg;
        CString err = CA2T(mysql_error(&mysqlCon));  
        msg.Format(_T("添加失败：%s"), (LPCTSTR)err);
        AfxMessageBox(msg);
        return false;
    }

    return true;
}

bool SQL_link::DeleteDrug(int id)
{
    snprintf(sql, SQL_MAX,
        "DELETE FROM `drugs_info` WHERE `med_id` = %d",
        id);

    int ret = mysql_real_query(&mysqlCon, sql, (unsigned long)strlen(sql));
    if (ret)
    {
        CString msg;
        msg.Format(_T("删除失败：%S"), mysql_error(&mysqlCon));
        AfxMessageBox(msg);
        return false;
    }

    return true;
}

bool SQL_link::UpdateDrug(int oldId, const drugs_info& info)
{
    // 1. 构建 SQL 语句
    // 注意：这里确保表名和字段名是用反引号 ` 包裹的，防止与保留字冲突
    snprintf(sql, SQL_MAX,
        "UPDATE `drugs_info` SET `med_id`=%d, `med_name`='%s', `med_kind`='%s', `med_num`=%d, `med_rmb`=%d WHERE `med_id`=%d",
        info.m_id,
        info.m_name.c_str(),
        info.m_kind.c_str(),
        info.m_num,
        info.m_rmb,
        oldId);

    // 2. 执行查询
    int ret = mysql_real_query(&mysqlCon, sql, (unsigned long)strlen(sql));
    if (ret)
    {
        // 3. 获取 MySQL 的原始错误信息
        const char* pError = mysql_error(&mysqlCon);

        // 转换为 Unicode 以便在 MFC 弹窗显示
        CString strError = CA2T(pError, CP_UTF8);
        CString strSql = CA2T(sql, CP_UTF8);

        CString msg;
        msg.Format(_T("数据库执行失败！\n\n错误原因：%s\n\n执行的SQL语句：\n%s"), strError, strSql);

        // 使用 AfxMessageBox 弹出详细错误
        AfxMessageBox(msg, MB_ICONERROR);
        return false;
    }

    return true;
}
vector<SQL_link::AdminInfo> SQL_link::GetAllAdmins() {
    vector<AdminInfo> admins;
    if (mysql_query(&mysqlCon, "SELECT m_username, m_userpsw FROM users_info")) return admins;
    MYSQL_RES* res = mysql_store_result(&mysqlCon);
    if (res) {
        while (MYSQL_ROW row = mysql_fetch_row(res)) {
            AdminInfo info;
            info.username = CA2T(row[0]);
            info.password = CA2T(row[1]);
            admins.push_back(info);
        }
        mysql_free_result(res);
    }
    return admins;
}

bool SQL_link::AddAdmin(CString name, CString psw) {
    CStringA sql;
    // 注意：这里的字段名要和你的数据库一致
    sql.Format("INSERT INTO users_info (m_username, m_userpsw) VALUES ('%s', '%s')",
        (LPCSTR)CT2A(name), (LPCSTR)CT2A(psw));
    return mysql_query(&mysqlCon, sql) == 0;
}

bool SQL_link::DeleteAdmin(CString name) {
    CStringA sql;
    sql.Format("DELETE FROM users_info WHERE m_username = '%s'", (LPCSTR)CT2A(name));
    return mysql_query(&mysqlCon, sql) == 0;
}