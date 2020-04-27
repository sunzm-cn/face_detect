
// 人脸检测息屏Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"

#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>   
#include <stdio.h>   
#include <stdlib.h>   

#pragma comment( lib, "opencv_core2413.lib" )  
#pragma comment( lib, "opencv_highgui2413.lib" )  
#pragma comment( lib, "opencv_imgproc2413.lib" )  

using namespace std;
using namespace cv;

DWORD WINAPI FaceDetect(LPVOID lpParam);

// C人脸检测息屏Dlg 对话框
class C人脸检测息屏Dlg : public CDialogEx
{
// 构造
public:
	C人脸检测息屏Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton StopButton;
	CButton SuspendButton;
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedLock();
	afx_msg void OnBnClickedStop();
};
