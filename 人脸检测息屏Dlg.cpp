
// 人脸检测息屏Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "人脸检测息屏.h"
#include "人脸检测息屏Dlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CEvent start_event;
int terminate_flag = 0;//用于保存是否结束程序
int started_flag = 0;//用于保存线程是否退出，即是否重复点击“开始”按钮

//该文件存在于OpenCV安装目录下的\sources\data\haarcascades内，需要将该xml文件复制到当前工程目录下
string face_cascade_name = "haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;

DWORD WINAPI FaceDetect(LPVOID lpParam) 
{
	//如果需要调用其它函数就要调用这个This指针
	C人脸检测息屏Dlg* pThis = (C人脸检测息屏Dlg*)lpParam;

	int pre1 = 1, pre2 = 1;//检测是否有人脸的部分需要用到

	VideoCapture cap(0);//打开默认的摄像头  
	if (!cap.isOpened())//检查是否成功打开  
		return -1;

	Mat image;

	if (!face_cascade.load(face_cascade_name)) {
		perror("无法加载级联分类器文件！\n");
		return -1;
	}
	int nTick = 0;

	while (1)
	{
		WaitForSingleObject(start_event, INFINITE);
		start_event.SetEvent();
		if (terminate_flag == -1)
		{
			terminate_flag = 0;
			_endthreadex(0);
		};

		if (!cap.isOpened())//等摄像头打开
			continue;
		Mat frame;
		nTick = getTickCount();
		cap >> frame;//从摄像头获取新的一帧  

		if (frame.data == NULL)//等到捕获到数据
		{  
			continue;
		}

		cvtColor(frame, image, CV_BGR2BGRA);

		if (!face_cascade.load(face_cascade_name))
		{
			perror("级联分类器错误，可能未找到文件，拷贝该文件到工程目录下！\n");
			return -1;
		}

		/************************************以下部分为人脸检测、标记与控制屏幕部分***************************************/
			std::vector<Rect> faces;
			Mat face_gray;

			cvtColor(image, face_gray, CV_BGR2GRAY);//rgb类型转换为灰度类型  
			equalizeHist(face_gray, face_gray);//直方图均衡化  

			face_cascade.detectMultiScale(face_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(1, 1));

			for (int i = 0; i < faces.size(); i++) {
				Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
				ellipse(image, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(255, 0, 0), 2, 7, 0);
			}

			if ((pre2 == 0) && (pre1 == 0) && (faces.size() == 0))//关闭显示器
				WinExec("nircmd.exe monitor async_off", SW_HIDE);

			if (faces.size() > 0)//开启显示器
				WinExec("nircmd.exe monitor async_on", SW_HIDE);

			pre2 = pre1;
			pre1 = faces.size();

			imshow("人脸检测结果", image);
			waitKey(30);
		/****************************************************************************************************************/
	}
}

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


// C人脸检测息屏Dlg 对话框



C人脸检测息屏Dlg::C人脸检测息屏Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C人脸检测息屏Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOCK, SuspendButton);
}

BEGIN_MESSAGE_MAP(C人脸检测息屏Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &C人脸检测息屏Dlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_LOCK, &C人脸检测息屏Dlg::OnBnClickedLock)
	ON_BN_CLICKED(IDC_STOP, &C人脸检测息屏Dlg::OnBnClickedStop)
END_MESSAGE_MAP()


// C人脸检测息屏Dlg 消息处理程序

BOOL C人脸检测息屏Dlg::OnInitDialog()
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void C人脸检测息屏Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void C人脸检测息屏Dlg::OnPaint()
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
HCURSOR C人脸检测息屏Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void C人脸检测息屏Dlg::OnBnClickedStart()
{
	// TODO: 在此添加控件通知处理程序代码

	if (started_flag == 0)//判断是否重复点击“开始”按钮
	{
		started_flag = 1;

		HANDLE hThreadSend;         //创建独立线程发送数据  
		DWORD ThreadSendID;

		start_event.SetEvent();

		hThreadSend = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FaceDetect, (LPVOID)this, 0, &ThreadSendID);
		CloseHandle(hThreadSend);
	}
}


void C人脸检测息屏Dlg::OnBnClickedLock()
{
	// TODO: 在此添加控件通知处理程序代码

	CString buttonText;
	SuspendButton.GetWindowText(buttonText);

	if (buttonText.Compare(_T("锁定屏幕")) == 0)
	{
		start_event.ResetEvent();
		SuspendButton.SetWindowTextW(_T("自动控制屏幕"));

	}
	else
	{
		start_event.SetEvent();
		SuspendButton.SetWindowText(_T("锁定屏幕"));
	}
}


void C人脸检测息屏Dlg::OnBnClickedStop()
{
	// TODO: 在此添加控件通知处理程序代码

	started_flag = 0;
	terminate_flag = -1;
}
