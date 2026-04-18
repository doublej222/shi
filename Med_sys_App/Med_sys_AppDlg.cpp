
// Med_sys_AppDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Med_sys_App.h"
#include "Med_sys_AppDlg.h"
#include "afxdialogex.h"
#include "Med_Manage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMedsysAppDlg 对话框



CMedsysAppDlg::CMedsysAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MED_SYS_APP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMedsysAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMedsysAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMedsysAppDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_Name, &CMedsysAppDlg::OnEnChangeName)
	ON_EN_CHANGE(IDC_Psw, &CMedsysAppDlg::OnEnChangePsw)
END_MESSAGE_MAP()


// CMedsysAppDlg 消息处理程序

BOOL CMedsysAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMedsysAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMedsysAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMedsysAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMedsysAppDlg::OnBnClickedOk()
{
	CString m_username, m_userpsw;

	GetDlgItemText(IDC_Name, m_username);
	GetDlgItemText(IDC_Psw, m_userpsw);

	if (m_username.IsEmpty()) {
		MessageBox(_T("请输入用户名"));
		return;
	}
	if (m_userpsw.IsEmpty()) {
		MessageBox(_T("请输入密码"));
		return;
	}
	if (!SQL.Connect_MySQL()){
		MessageBox(_T("数据库连接失败！"));
		return;
	}

	if (SQL.CheckLogin(m_username, m_userpsw)) {
		MessageBox(_T("登录成功！"));

		// 隐藏当前登录窗口
		ShowWindow(SW_HIDE);

		// 打开药品管理界面
		Med_Manage dlg;
		dlg.DoModal();

		// 关闭登录窗口
		EndDialog(IDOK);
	}
	else
	{
		MessageBox(_T("用户名或密码错误！"));
	}
}
void CMedsysAppDlg::OnEnChangeName()
{
	// 在这里添加逻辑
}

void CMedsysAppDlg::OnEnChangePsw()
{
	// 在这里添加逻辑
}