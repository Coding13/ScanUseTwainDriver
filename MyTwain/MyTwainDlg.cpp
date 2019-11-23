
// MyTwainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyTwain.h"
#include "MyTwainDlg.h"
#include "afxdialogex.h"
#include "DIB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMyTwainDlg �Ի���




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


// CMyTwainDlg ��Ϣ�������

BOOL CMyTwainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// ����TWAIN ��ʼ��ɨ������
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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMyTwainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// ����ͼ��
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMyTwainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyTwainDlg::OnBnClickedSelectsource()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
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
		sourceCombo.AddString("û�п��õĴ�ӡ��");
		m_bSourceSelected=FALSE;
	}
	sourceCombo.SetCurSel(0);
	m_Source=sourceArry.GetAt(0);
	int count=sourceArry.GetCount();

	duplexCombo.AddString("�����ӡ");
	duplexCombo.AddString("˫���ӡ");
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

	pixelCombo.AddString("�ڰ�");
	pixelCombo.AddString("�Ҷ�ͼ");
	pixelCombo.AddString("��ɫ");
	pixelCombo.SetCurSel(0);

	resolutionCombo.AddString("72");
	resolutionCombo.AddString("150");
	resolutionCombo.AddString("200");
	resolutionCombo.AddString("300");
	resolutionCombo.SetCurSel(1);
}


void CMyTwainDlg::OnBnClickedAcquireBtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_bSourceSelected)
	{
		int i=sourceCombo.GetCurSel();
		m_Source=sourceArry.GetAt(i);
	}
	
}


void CMyTwainDlg::OnCbnSelchangeComboDuplex()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sel=duplexCombo.GetCurSel();
	BOOL ret_value=FALSE;
}


void CMyTwainDlg::OnCbnSelchangeComboSizes()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sel=sizeCombo.GetCurSel();
	BOOL ret_value=FALSE;
}


void CMyTwainDlg::OnCbnSelchangeComboPixel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sel=pixelCombo.GetCurSel();
	BOOL ret_value=FALSE;
}


void CMyTwainDlg::OnCbnSelchangeComboResolution()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
