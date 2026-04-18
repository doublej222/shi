#pragma once
#include "afxdialogex.h"
#include <string>
#include "SQL_link.h"
#include "drugs_info.h"
using namespace std;

// Med_Manage 对话框

class Med_Manage : public CDialogEx
{
	DECLARE_DYNAMIC(Med_Manage)

public:
	Med_Manage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Med_Manage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Med_Manage };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	void ShowData();

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	vector<drugs_info> Info;
	SQL_link SQL;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton2();
	CString m_rule1;
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);

	void UpdateStatistics(); // 数据统计函数
};