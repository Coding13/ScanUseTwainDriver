
// MyTwainDlg.h : 头文件
//

#pragma once
#include "twaincpp.h"
#include "afxwin.h"

// CMyTwainDlg 对话框
class CMyTwainDlg : public CDialogEx,public CTwain
{
// 构造
public:
	CMyTwainDlg(CWnd* pParent = NULL);	// 标准构造函数
	void CopyImage(HANDLE hBitmap,TW_IMAGEINFO& info);
	void SetImage(HANDLE hBitmap,int bits);
// 对话框数据
	enum { IDD = IDD_MYTWAIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSelectsource();
	CButton selectBtn;
	CButton acquireBtn;
	afx_msg void OnBnClickedAcquireBtn();
	afx_msg void OnCbnSelchangeComboSource();
	CComboBox sourceCombo;
	CString sourceStr;
	BOOL m_bInit;

private:
	void initCombox();
public:
	afx_msg void OnCbnSelchangeComboDuplex();
	CComboBox duplexCombo;
	CComboBox sizeCombo;
	CComboBox pixelCombo;
	afx_msg void OnCbnSelchangeComboSizes();
	afx_msg void OnCbnSelchangeComboPixel();
	CComboBox resolutionCombo;
	CString resolutionStr;
	CArray<TW_IDENTITY,TW_IDENTITY> sourceArry;
	afx_msg void OnCbnSelchangeComboResolution();
};
