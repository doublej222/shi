// Info_Dlg.cpp: 实现文件
//

#include "pch.h"
#include "Med_sys_App.h"
#include "afxdialogex.h"
#include "Info_Dlg.h"


// Info_Dlg 对话框

IMPLEMENT_DYNAMIC(Info_Dlg, CDialogEx)

Info_Dlg::Info_Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_InfoDlg, pParent)
	, n_id()
	, n_name(_T(""))
	, n_kind(_T(""))
	, n_num()
	, n_rmb()
{

}

Info_Dlg::~Info_Dlg()
{
}

void Info_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT5, n_id);
	DDX_Text(pDX, IDC_EDIT3, n_name);
	DDX_Text(pDX, IDC_EDIT4, n_kind);
	DDX_Text(pDX, IDC_EDIT6, n_num);
	DDX_Text(pDX, IDC_EDIT7, n_rmb);
}


BEGIN_MESSAGE_MAP(Info_Dlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &Info_Dlg::OnBnClickedOk)
END_MESSAGE_MAP()


// Info_Dlg 消息处理程序

void Info_Dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (n_name == _T("") || n_kind == _T(""))
	{
		MessageBox(_T("请输入药品名称和类型！"), _T("提示"), MB_OK | MB_ICONINFORMATION);
		return;
	}
	if (n_id <= 0 || n_num <= 0 || n_rmb <= 0)
	{
		MessageBox(_T("请输入正确的药品编号、数量和价格！"), _T("提示"), MB_OK | MB_ICONINFORMATION);
		return;
	}
	CDialogEx::OnOK();
}
