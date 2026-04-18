// UserManageDlg.cpp: 实现文件
//

#include "pch.h"
#include "Med_sys_App.h"
#include "afxdialogex.h"
#include "UserManageDlg.h"
#include "SQL_link.h" 

// UserManageDlg 对话框

IMPLEMENT_DYNAMIC(UserManageDlg, CDialogEx)

UserManageDlg::UserManageDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UserManageDlg, pParent)
	, u_name(_T(""))
	, u_psw(_T(""))
{

}

UserManageDlg::~UserManageDlg()
{
}

void UserManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT6, u_name);
	DDX_Text(pDX, IDC_EDIT7, u_psw);
	DDX_Control(pDX, IDC_LIST3, m_userList);
}


BEGIN_MESSAGE_MAP(UserManageDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &UserManageDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &UserManageDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// UserManageDlg 消息处理程序
// 增加
void UserManageDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    if (u_name.IsEmpty() || u_psw.IsEmpty()) { // 如果你改成了 CString
        MessageBox(_T("账号或密码不能为空！"));
        return;
    }

    SQL_link sql;
    if (sql.Connect_MySQL()) {
        if (sql.AddAdmin(u_name, u_psw)) {
            MessageBox(_T("管理员添加成功！"));
            RefreshList(); // 刷新列表
        }
        else {
            MessageBox(_T("添加失败，可能账号已存在！"));
        }
    }
}
// 删除
void UserManageDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
    int nIndex = m_userList.GetSelectionMark();
    if (nIndex == -1) {
        MessageBox(_T("请先在列表中选择要删除的账号！"));
        return;
    }

    CString selUser = m_userList.GetItemText(nIndex, 0);
    if (MessageBox(_T("确定要删除管理员 ") + selUser + _T(" 吗？"), _T("警告"), MB_YESNO) == IDYES) {
        SQL_link sql;
        if (sql.Connect_MySQL()) {
            if (sql.DeleteAdmin(selUser)) {
                MessageBox(_T("删除成功！"));
                RefreshList();
            }
        }
    }
}


// 在 OnInitDialog 中初始化
BOOL UserManageDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 设置列表样式
    m_userList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    m_userList.InsertColumn(0, _T("管理员账号"), LVCFMT_CENTER, 150);
    m_userList.InsertColumn(1, _T("登录密码"), LVCFMT_CENTER, 150);

    RefreshList(); // 初始加载数据
    return TRUE;
}

// 刷新数据函数
void UserManageDlg::RefreshList()
{
    m_userList.DeleteAllItems();
    SQL_link sql;
    if (sql.Connect_MySQL()) {
        // 明确指明 vector 的类型
        std::vector<SQL_link::AdminInfo> admins = sql.GetAllAdmins();
        for (int i = 0; i < (int)admins.size(); ++i) {
            m_userList.InsertItem(i, admins[i].username);
            m_userList.SetItemText(i, 1, admins[i].password);
        }
    }
}