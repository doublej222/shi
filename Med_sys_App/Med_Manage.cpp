// Med_Manage.cpp: 实现文件
//

#include "pch.h"
#include "Med_sys_App.h"
#include "afxdialogex.h"
#include "Med_Manage.h"
#include <afxcmn.h>
#include <atlconv.h>
#include "Info_Dlg.h"
#include "SeekDlg.h"
#include <algorithm>
#include "UserManageDlg.h"
#include "resource.h"
// Med_Manage 对话框

IMPLEMENT_DYNAMIC(Med_Manage, CDialogEx)

Med_Manage::Med_Manage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Med_Manage, pParent)
	, m_rule1(_T(""))
{

}

Med_Manage::~Med_Manage()
{
}

void Med_Manage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_CBString(pDX, IDC_COMBO2, m_rule1);
}

BOOL Med_Manage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_list.InsertColumn(0, _T("编号"), LVCFMT_CENTER, 120);
	m_list.InsertColumn(1, _T("名称"), LVCFMT_CENTER, 200);
	m_list.InsertColumn(2, _T("类型"), LVCFMT_CENTER, 160);
	m_list.InsertColumn(3, _T("数量"), LVCFMT_CENTER, 160);
	m_list.InsertColumn(4, _T("价格"), LVCFMT_CENTER, 160);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO2);
	pCombo->ResetContent(); // 先清空，防止重复添加
	pCombo->AddString(_T("编号：从低到高"));
	pCombo->AddString(_T("编号：从高到低"));
	pCombo->AddString(_T("数量：从低到高"));
	pCombo->AddString(_T("数量：从高到低"));
	pCombo->AddString(_T("价格：从低到高"));
	pCombo->AddString(_T("价格：从高到低"));
	return TRUE;
}
static CString Utf8ToCString(const std::string& str)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	CString result;
	wchar_t* buffer = new wchar_t[len];

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer, len);
	result = buffer;

	delete[] buffer;
	return result;
}
void Med_Manage::ShowData()
{
	m_list.DeleteAllItems();

	for (int i = 0; i < (int)Info.size(); ++i)
	{
		CString id, num, rmb;
		id.Format(_T("%d"), Info[i].m_id);
		num.Format(_T("%d"), Info[i].m_num);
		rmb.Format(_T("%d"), Info[i].m_rmb);

		// 先转换到 CString，然后再调用 InsertItem / SetItemText
		CString name = Utf8ToCString(Info[i].m_name);
		CString kind = Utf8ToCString(Info[i].m_kind);

		m_list.InsertItem(i, id);  
		m_list.SetItemText(i, 1, name);
		m_list.SetItemText(i, 2, kind);
		m_list.SetItemText(i, 3, num);
		m_list.SetItemText(i, 4, rmb);

	}

	Update_statistics();
}

BEGIN_MESSAGE_MAP(Med_Manage, CDialogEx)
	ON_BN_CLICKED(IDOK, &Med_Manage::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &Med_Manage::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &Med_Manage::OnBnClicked_search_drug)
	ON_BN_CLICKED(IDC_BUTTON4, &Med_Manage::OnBnClicked_edit_drug)
	ON_BN_CLICKED(IDC_BUTTON5, &Med_Manage::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &Med_Manage::OnBnClicked_add_drug)
	ON_BN_CLICKED(IDC_BUTTON2, &Med_Manage::OnBnClicked_delete_drug)
	ON_CBN_SELCHANGE(IDC_COMBO2, &Med_Manage::OnCbnSelchange_sort_rule)
	ON_BN_CLICKED(IDC_BUTTON7, &Med_Manage::OnBnClicked_user_manage)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &Med_Manage::OnLvnItemchangedList1)
END_MESSAGE_MAP()


// Med_Manage 消息处理程序

void Med_Manage::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
// 导入
void Med_Manage::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (SQL.Connect_MySQL()) {
		Info = SQL.Get_All_Info();
		//MessageBox(TEXT("fuck"));
		ShowData();
		return;
	}
	else {
		MessageBox(_T("数据库连接失败！"));
	}
}

// 查询功能
void Med_Manage::OnBnClicked_search_drug()
{
	SeekDlg dlg;
	if (dlg.DoModal() == IDOK) {
		dlg.t_name.Trim();
		dlg.t_kind.Trim();

		CString sSearchName = dlg.t_name;
		CString sSearchKind = dlg.t_kind;

		vector<drugs_info> searchResults;

		for (const auto& info : Info) {
			CString dbName = Utf8ToCString(info.m_name);
			CString dbKind = Utf8ToCString(info.m_kind);
			dbName.Trim();
			dbKind.Trim();

			bool nameMatch = true;
			bool kindMatch = true;

			// 比较
			if (!sSearchName.IsEmpty()) {
				if (dbName.Find(sSearchName) == -1) { // 找不到返回 -1
					nameMatch = false;
				}
			}

			if (!sSearchKind.IsEmpty()) {
				if (dbKind.Find(sSearchKind) == -1) {
					kindMatch = false;
				}
			}

			// 只有同时满足输入的条件才加入
			if (nameMatch && kindMatch) {
				searchResults.push_back(info);
			}
		}

		// 显示结果
		if (searchResults.empty()) {
			MessageBox(_T("未找到符合条件的相关药品！"), _T("提示"));
		}
		else {
			m_list.DeleteAllItems();
			for (size_t i = 0; i < searchResults.size(); ++i) {
				CString id, num, rmb;
				id.Format(_T("%d"), searchResults[i].m_id);
				num.Format(_T("%d"), searchResults[i].m_num);
				rmb.Format(_T("%d"), searchResults[i].m_rmb);

				// 这里直接转换并显示
				m_list.InsertItem(i, id);
				m_list.SetItemText(i, 1, Utf8ToCString(searchResults[i].m_name));
				m_list.SetItemText(i, 2, Utf8ToCString(searchResults[i].m_kind));
				m_list.SetItemText(i, 3, num);
				m_list.SetItemText(i, 4, rmb);
			}
		}
	}
}
// 修改功能
void Med_Manage::OnBnClicked_edit_drug()
{
	// 获取当前选中的行索引
	int nIndex = m_list.GetSelectionMark();
	if (nIndex == -1)
	{
		MessageBox(_T("请先选择要修改的药品！"));
		return;
	}

	Info_Dlg dlg;
	// 将现有的数据填入对话框
	dlg.n_id = Info[nIndex].m_id;
	dlg.n_name = Utf8ToCString(Info[nIndex].m_name); // 使用你代码里定义的 Utf8ToCString
	dlg.n_kind = Utf8ToCString(Info[nIndex].m_kind);
	dlg.n_num = Info[nIndex].m_num;
	dlg.n_rmb = Info[nIndex].m_rmb;

	// 弹出对话框
	if (dlg.DoModal() == IDOK)
	{
		if (!SQL.Connect_MySQL()) {
			MessageBox(_T("数据库连接失败！"));
			return;
		}

		// 构建修改后的对象 (CString -> UTF8 std::string)
		drugs_info updatedInfo(
			dlg.n_id,
			(std::string)CT2A(dlg.n_name, CP_UTF8),
			(std::string)CT2A(dlg.n_kind, CP_UTF8),
			dlg.n_num,
			dlg.n_rmb
		);

		// 调用数据库修改函数
		int oldId = Info[nIndex].m_id;

		if (SQL.UpdateDrug(oldId, updatedInfo))
		{
			MessageBox(_T("修改成功！"));
			// 刷新界面
			Info = SQL.Get_All_Info();
			ShowData();
		}
	}
}
void Med_Manage::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

// 添加功能
void Med_Manage::OnBnClicked_add_drug()
{
	Info_Dlg dlg;

	if (dlg.DoModal() == IDOK)
	{
		if (!SQL.Connect_MySQL())
		{
			MessageBox(_T("数据库连接失败！"));
			return;
		}

		// CString -> std::string（UTF-8）
		std::string name = CT2A(dlg.n_name, CP_UTF8);
		std::string kind = CT2A(dlg.n_kind, CP_UTF8);

		drugs_info info(dlg.n_id, name, kind, dlg.n_num, dlg.n_rmb);

		if (SQL.AddDrug(info))
		{
			MessageBox(_T("添加成功！"));
			Info = SQL.Get_All_Info();
			ShowData();
		}
	}
}

// 删除功能
void Med_Manage::OnBnClicked_delete_drug()
{
	int nIndex = m_list.GetNextItem(-1, LVNI_SELECTED);
	if (nIndex == -1)
	{
		MessageBox(_T("请先选择要删除的药品！"));
		return;
	}

	CString idText = m_list.GetItemText(nIndex, 0);
	int id = _ttoi(idText);

	if (id <= 0)
	{
		MessageBox(_T("选中的编号无效！"));
		return;
	}

	UINT ret = MessageBox(_T("确认要删除这条信息吗？"), _T("提示"), MB_YESNO | MB_ICONQUESTION);
	if (ret != IDYES)
	{
		return;
	}

	if (!SQL.Connect_MySQL())
	{
		MessageBox(_T("数据库连接失败！"));
		return;
	}

	if (!SQL.DeleteDrug(id))
	{
		return;
	}

	MessageBox(_T("删除成功！"));
	Info = SQL.Get_All_Info();
	ShowData();
}

// 排序
void Med_Manage::OnCbnSelchange_sort_rule()
{
	if (Info.empty()) return;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO2);
	int nIndex = pCombo->GetCurSel();
	if (nIndex == CB_ERR) return;

	CString strRule;
	// 获取选中的文字内容
	pCombo->GetLBText(nIndex, strRule); 

	// 根据选中的文字内容执行对应的排序 Lambda
	if (strRule == _T("编号：从低到高")) {
		std::sort(Info.begin(), Info.end(), [](const drugs_info& a, const drugs_info& b) {
			return a.m_id < b.m_id;
			});
	}
	else if (strRule == _T("编号：从高到低")) {
		std::sort(Info.begin(), Info.end(), [](const drugs_info& a, const drugs_info& b) {
			return a.m_id > b.m_id;
			});
	}
	else if (strRule == _T("数量：从低到高")) {
		std::sort(Info.begin(), Info.end(), [](const drugs_info& a, const drugs_info& b) {
			return a.m_num < b.m_num;
			});
	}
	else if (strRule == _T("数量：从高到低")) {
		std::sort(Info.begin(), Info.end(), [](const drugs_info& a, const drugs_info& b) {
			return a.m_num > b.m_num;
			});
	}
	else if (strRule == _T("价格：从低到高")) {
		std::sort(Info.begin(), Info.end(), [](const drugs_info& a, const drugs_info& b) {
			return a.m_rmb < b.m_rmb;
			});
	}
	else if (strRule == _T("价格：从高到低")) {
		std::sort(Info.begin(), Info.end(), [](const drugs_info& a, const drugs_info& b) {
			return a.m_rmb > b.m_rmb;
			});
	}

	// 排序后刷新列表
	ShowData();
}

// 管理信息
void Med_Manage::OnBnClicked_user_manage()
{
	// TODO: 在此添加控件通知处理程序代码
	UserManageDlg dlg;
	dlg.DoModal();
}

void Med_Manage::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

// 数据统计函数
void Med_Manage::Update_statistics()
{
	if (Info.empty())
	{
		SetDlgItemText(IDC_EDIT_TOTAL_KIND, _T("0"));
		SetDlgItemText(IDC_EDIT_TOTAL_NUM, _T("0"));
		SetDlgItemText(IDC_EDIT_TOTAL_VAL, _T("0.00"));
		return;
	}

	int nTotalKinds = (int)Info.size();
	long long nTotalNum = 0;
	double dTotalValue = 0.0;

	// 遍历内存中的数据进行计算
	for (const auto& item : Info)
	{
		nTotalNum += item.m_num;
		dTotalValue += (double)item.m_num * item.m_rmb;
	}
   
	// 格式化数据
	CString strKinds, strNum, strValue;
	strKinds.Format(_T("%d"), nTotalKinds);
	strNum.Format(_T("%lld"), nTotalNum);
	strValue.Format(_T("%.2f"), dTotalValue);

	// 更新到界面上的编辑框中
	SetDlgItemText(IDC_EDIT_TOTAL_KIND, strKinds);
	SetDlgItemText(IDC_EDIT_TOTAL_NUM, strNum);
	SetDlgItemText(IDC_EDIT_TOTAL_VAL, strValue);
}