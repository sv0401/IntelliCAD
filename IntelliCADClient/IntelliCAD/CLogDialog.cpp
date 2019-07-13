// CLogDialog.cpp: 구현 파일
//

#include "stdafx.h"
#include "CLogDialog.h"
#include <ctime>
#include <chrono>
#include "IntelliCAD.h"
#include "afxdialogex.h"
#include "Parser.hpp"
#include "System.h"

using namespace std;


// CLogDialog 대화 상자

IMPLEMENT_DYNAMIC(CLogDialog, CDialogEx)

BEGIN_MESSAGE_MAP(CLogDialog, CDialogEx)
END_MESSAGE_MAP()

CLogDialog::CLogDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_LOG, pParent)
{
	System::getInstance().__setLogDlgReference(*this);
}

CLogDialog::~CLogDialog()
{
}

void CLogDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOG_LISTBOX_log, __ddx_list_log);
}


// CLogDialog 메시지 처리기

void CLogDialog::printLog(const tstring &message)
{
	using namespace chrono;

	auto now = system_clock::now();
	time_t now_t = system_clock::to_time_t(now);

	tm now_tm;
	localtime_s(&now_tm, &now_t);

	char buffer[32];
	strftime(buffer, sizeof(buffer), "[%H:%M:%S] ", &now_tm);

	tstring outString =
		(Parser::LPCSTR$tstring(buffer) + message);

	__ddx_list_log.AddString(outString.c_str());
	__ddx_list_log.SetTopIndex(__ddx_list_log.GetCount() - 1);
}