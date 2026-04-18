#pragma once
#include "afxdialogex.h"


// UserManageDlg 对话框

class UserManageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(UserManageDlg)

public:
	UserManageDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~UserManageDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UserManageDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString u_name;
	CString u_psw;

	CListCtrl m_userList;

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	BOOL OnInitDialog();
	void RefreshList();
};
