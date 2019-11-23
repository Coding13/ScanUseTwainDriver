
// MyTwainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTwain.h"
#include "MyTwainDlg.h"
#include "afxdialogex.h"
#include "DIB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyTwainDlg 对话框




CMyTwainDlg::CMyTwainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyTwainDlg::IDD, pParent)
	, sourceStr(_T(""))
	, resolutionStr(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyTwainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SelectSource, selectBtn);
	DDX_Control(pDX, IDC_ACQUIRE_BTN, acquireBtn);
	DDX_Control(pDX, IDC_COMBO_SOURCE, sourceCombo);
	DDX_CBString(pDX, IDC_COMBO_SOURCE, sourceStr);
	DDX_Control(pDX, IDC_COMBO_DUPLEX, duplexCombo);
	DDX_Control(pDX, IDC_COMBO_SIZES, sizeCombo);
	DDX_Control(pDX, IDC_COMBO_PIXEL, pixelCombo);
	DDX_Control(pDX, IDC_COMBO_RESOLUTION, resolutionCombo);
	DDX_CBString(pDX, IDC_COMBO_RESOLUTION, resolutionStr);
}

BEGIN_MESSAGE_MAP(CMyTwainDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SelectSource, &CMyTwainDlg::OnBnClickedSelectsource)
	ON_BN_CLICKED(IDC_ACQUIRE_BTN, &CMyTwainDlg::OnBnClickedAcquireBtn)
	ON_CBN_SELCHANGE(IDC_COMBO_SOURCE, &CMyTwainDlg::OnCbnSelchangeComboSource)
	ON_CBN_SELCHANGE(IDC_COMBO_DUPLEX, &CMyTwainDlg::OnCbnSelchangeComboDuplex)
	ON_CBN_SELCHANGE(IDC_COMBO_SIZES, &CMyTwainDlg::OnCbnSelchangeComboSizes)
	ON_CBN_SELCHANGE(IDC_COMBO_PIXEL, &CMyTwainDlg::OnCbnSelchangeComboPixel)
	ON_CBN_SELCHANGE(IDC_COMBO_RESOLUTION, &CMyTwainDlg::OnCbnSelchangeComboResolution)
END_MESSAGE_MAP()


// CMyTwainDlg 消息处理程序

BOOL CMyTwainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// 调用TWAIN 初始化扫描设置
	m_bInit=FALSE;
	if(!m_bInit)
	{
		m_bInit=InitTwain(m_hWnd);
		if(!IsValidDriver())
		{
			AfxMessageBox("Unable to load Twain Driver.");
			selectBtn.EnableWindow(FALSE);
		}
		acquireBtn.EnableWindow(TRUE);
		initCombox();
		acquireBtn.EnableWindow(SourceSelected());
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BOOL CMyTwainDlg::PreTranslateMessage(MSG* pMsg) 
{
	ProcessMessage(*pMsg);
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CMyTwainDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyTwainDlg::OnPaint()
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

void CMyTwainDlg::CopyImage(HANDLE hBitmap,TW_IMAGEINFO& info)
{
	SetImage(hBitmap,info.BitsPerPixel);
}

void CMyTwainDlg::SetImage(HANDLE hBitmap,int bits)
{
		CDIB dib;
		dib.CreateFromHandle(hBitmap,bits);
		SYSTEMTIME sys;     
		long t1=GetTickCount();
		CString str2;
		str2.Format("name%d.bmp",t1);
		dib.SaveDIB(str2,CDIB::BitmapType::BMP);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMyTwainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyTwainDlg::OnBnClickedSelectsource()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if (!m_bInit)
	{
		return;
	}
	acquireBtn.EnableWindow(IsValidDriver());
	selectBtn.EnableWindow(SourceSelected());
}

void CMyTwainDlg::initCombox()
{
	if (CallTwainProc(&m_AppId,NULL,DG_CONTROL,DAT_IDENTITY,MSG_GETFIRST,&m_Source))
	{
		TW_IDENTITY temp_Source=m_Source;
		sourceArry.Add(temp_Source);
		sourceCombo.AddString(m_Source.ProductName);
		while(CallTwainProc(&m_AppId,NULL,DG_CONTROL,DAT_IDENTITY,MSG_GETNEXT,&m_Source)){
			TW_IDENTITY temp_Source=m_Source;
			sourceArry.Add(temp_Source);
			sourceCombo.AddString(m_Source.ProductName);
		}
		m_bSourceSelected=TRUE;
	}else{
		sourceCombo.AddString("没有可用的打印机");
		m_bSourceSelected=FALSE;
	}
	sourceCombo.SetCurSel(0);
	m_Source=sourceArry.GetAt(0);
	int count=sourceArry.GetCount();

	duplexCombo.AddString("单面打印");
	duplexCombo.AddString("双面打印");
	duplexCombo.SetCurSel(0);

	sizeCombo.AddString("A4");
	sizeCombo.AddString("B5");
	sizeCombo.AddString("USLETTER");
	sizeCombo.AddString("USLEGAL");
	sizeCombo.AddString("A5");
	sizeCombo.AddString("B4");
	sizeCombo.AddString("B6");
	sizeCombo.AddString("B");
	sizeCombo.AddString("USLEDGER");
	sizeCombo.AddString("USEXECUTIVE");
	sizeCombo.AddString("A3");
	sizeCombo.AddString("B3");
	sizeCombo.AddString("A6");
	sizeCombo.AddString("C4");
	sizeCombo.AddString("C5");
	sizeCombo.AddString("C6");
	sizeCombo.SetCurSel(0);

	pixelCombo.AddString("黑白");
	pixelCombo.AddString("灰度图");
	pixelCombo.AddString("彩色");
	pixelCombo.SetCurSel(0);

	resolutionCombo.AddString("72");
	resolutionCombo.AddString("150");
	resolutionCombo.AddString("200");
	resolutionCombo.AddString("300");
	resolutionCombo.SetCurSel(1);
}


void CMyTwainDlg::OnBnClickedAcquireBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_bInit)
	{
		return;
	}
	UpdateData(TRUE);
	int resolution= atoi(resolutionStr);
	Acquire(TWCPP_ANYCOUNT,duplexCombo.GetCurSel(),sizeCombo.GetCurSel()+1,pixelCombo.GetCurSel(),resolution);
	acquireBtn.EnableWindow(IsValidDriver());
	selectBtn.EnableWindow(SourceSelected());
}


void CMyTwainDlg::OnCbnSelchangeComboSource()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSourceSelected)
	{
		int i=sourceCombo.GetCurSel();
		m_Source=sourceArry.GetAt(i);
	}
	
}


void CMyTwainDlg::OnCbnSelchangeComboDuplex()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel=duplexCombo.GetCurSel();
	BOOL ret_value=FALSE;
}


void CMyTwainDlg::OnCbnSelchangeComboSizes()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel=sizeCombo.GetCurSel();
	BOOL ret_value=FALSE;
}


void CMyTwainDlg::OnCbnSelchangeComboPixel()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel=pixelCombo.GetCurSel();
	BOOL ret_value=FALSE;
}


void CMyTwainDlg::OnCbnSelchangeComboResolution()
{
	// TODO: 在此添加控件通知处理程序代码
}
