#pragma once
#include "afxdialogex.h"


// Info_Dlg 对话框

class Info_Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(Info_Dlg)

public:
	Info_Dlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Info_Dlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_InfoDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int n_id;
	CString n_name;
	CString n_kind;
	int n_num;
	int n_rmb;
	afx_msg void OnBnClickedOk();
};
