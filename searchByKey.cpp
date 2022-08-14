// searchByKey.cpp : implementation file
//

#include "pch.h"
#include "dictionaryGUI.h"
#include "afxdialogex.h"
#include "searchByKey.h"

IMPLEMENT_DYNAMIC(searchByKey, CDialogEx)

searchByKey::searchByKey(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
}

searchByKey::~searchByKey()
{
}

void searchByKey::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, edt_key);
	DDX_Control(pDX, IDC_SEARCH, btn_search);
	DDX_Control(pDX, IDC_LIST1, list_ctrl_def);
	DDX_Control(pDX, IDC_COMBO_TYPE, comboBox_type);
	DDX_Control(pDX, IDC_BUTTON_CHOOSE, btn_choose);
	DDX_Control(pDX, IDC_EDIT_CHOSEN_TYPE, edt_chosen);
	DDX_Control(pDX, IDC_EDIT_ORDER, idc_order);
}


BEGIN_MESSAGE_MAP(searchByKey, CDialogEx)
	ON_BN_CLICKED(IDOK, &searchByKey::OnBnClickedOk)
	ON_BN_CLICKED(IDC_SEARCH, &searchByKey::OnBnClickedSearch)
	ON_BN_CLICKED(IDC_BUTTON_CHOOSE, &searchByKey::OnBnClickedButtonChoose)
	ON_BN_CLICKED(IDCANCEL, &searchByKey::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &searchByKey::OnBnClickedButton1)
END_MESSAGE_MAP()


// searchByKey message handlers


void searchByKey::OnBnClickedOk()
{
}


void searchByKey::OnBnClickedSearch()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CString check;
	GetDlgItemText(IDC_EDIT_CHOSEN_TYPE, check);
	if (check.GetLength() < 1) {
		AfxMessageBox(_T("Choose the type of dictionary !"));
	}
	else {
		list_ctrl_def.DeleteAllItems();

		CString key;
		GetDlgItemText(IDC_EDIT1, key);
		CT2CA pszConvertedAnsiString(key);
		string k(pszConvertedAnsiString);
		Node* lastChar = dict.searchByKey(dict.tree[type], k);
		if (!lastChar || !lastChar->isLastChar) {
			MessageBox(_T("The keyword does not exist !"));
		}
		else {
			saveToHistoryFile(lastChar, k);
			keyWord = k; pLastChar = lastChar;
			for (int i = 0; i < lastChar->def.size(); i++) {
				CString index;
				index.Format(_T("%d"), i + 1);
				list_ctrl_def.InsertItem(i, index);
				CString def(lastChar->def[i].c_str());
				list_ctrl_def.SetItemText(i, 1, def);
			}
		}
	}
}

void searchByKey::OnBnClickedButtonChoose()
{
	// TODO: Add your control notification handler code here

	int comboBox1SelectedItem = comboBox_type.GetCurSel();
	CString str;
	if (comboBox1SelectedItem != LB_ERR) {
		comboBox_type.GetLBText(comboBox1SelectedItem, str);
	}
	if (str == _T("Emotional")) type = 0;
	if (str == _T("Slang")) type = 1;
	if (str == _T("English to English")) type = 2;
	if (str == _T("English to Vietnamese")) type = 3;
	if (str == _T("Vietnamese to English")) type = 4;

	SetDlgItemText(IDC_EDIT_CHOSEN_TYPE, str);
	list_ctrl_def.DeleteAllItems();
	list_ctrl_def.DeleteColumn(0);
	list_ctrl_def.DeleteColumn(0);
	list_ctrl_def.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	list_ctrl_def.InsertColumn(0, _T("Definition"), LVCFMT_LEFT, 3000);
	list_ctrl_def.InsertColumn(0, _T("Number"), LVCFMT_LEFT, 100);
}



void searchByKey::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
	CdictionaryGUIDlg mainMenu;
	mainMenu.DoModal();
}


void searchByKey::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString check;
	GetDlgItemText(IDC_EDIT_ORDER, check);
	int ord = _tstof(check);
	if (check.GetLength() < 1) {
		AfxMessageBox(_T("Input the order !"));
	}
	else if (ord < 1 || ord > pLastChar->def.size()) {
		AfxMessageBox(_T("Invalid order !"));
	}
	else {
		dict.addFavWord(keyWord, pLastChar->def, ord, type);
		MessageBox(_T("Adding successfully !"));
	}
}