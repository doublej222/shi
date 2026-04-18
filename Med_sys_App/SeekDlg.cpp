// SeekDlg.cpp: 实现文件
//

#include "pch.h"
#include "Med_sys_App.h"
#include "afxdialogex.h"
#include "SeekDlg.h"


// SeekDlg 对话框

IMPLEMENT_DYNAMIC(SeekDlg, CDialogEx)

SeekDlg::SeekDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SeekDlg, pParent)
	, t_name(_T(""))
    , t_kind(_T(""))
{

}

SeekDlg::~SeekDlg()
{
}

void SeekDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, t_name);
    DDX_Text(pDX, IDC_EDIT8, t_kind);
}


BEGIN_MESSAGE_MAP(SeekDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &SeekDlg::OnBnClicked_Seek_Ok)
	ON_EN_CHANGE(IDC_EDIT8, &SeekDlg::OnEnChangeEdit8)
END_MESSAGE_MAP()


// SeekDlg 消息处理程序

void SeekDlg::OnBnClicked_Seek_Ok()
{
    UpdateData(TRUE);

	// 同时判断药品名称和类别是否都为空
    if (t_name.IsEmpty() && t_kind.IsEmpty())
    {
        MessageBox(_T("请输入药品名称或类别进行查询！"), _T("提示"), MB_OK | MB_ICONINFORMATION);
        return;
    }

    CDialogEx::OnOK();
}
void SeekDlg::OnEnChangeEdit8()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
 