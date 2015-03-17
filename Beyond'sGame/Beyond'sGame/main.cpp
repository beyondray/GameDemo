/*----------------------------------------------------------------
【Cpp文件】：main.cpp	    Create by Beyond Ray,2013年2月
（描述）：模型的导入和控制
------------------------------------------------------------------*/

//【头文件】
#include"D3DComMacro.h"
#include"XFileModelClass.h"
#include"DirectInputClass.h"
#include"TerrainClass.h"
#include"CameraClass.h"
#include"SkyBoxClass.h"
#include"TornadoClass.h"
#include"AllocateHierarchyClass.h"
#include"GUIClass.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>

//【库文件】
#pragma comment (lib, "winmm.lib")                                     //为调用 PlaySound 函数
#pragma comment (lib, "d3d9.lib")								       //Direct3D9相关库文件	
#pragma comment (lib, "d3dx9.lib")									   //ID3DX相关库文件
#pragma comment (lib, "dxguid.lib")									   //GUID相关识别
#pragma comment (lib, "dinput8.lib")								   //DirectInput相关库文件

//【宏定义】
#define WINDOW_TITLE	L"致我们永不熄灭的游戏梦想----【我爱杨】"      //窗口标题

//【全局变量声明】
TCHAR		lpClassName[] = TEXT("GDIDemo11-1");					   //声明类名
HINSTANCE   g_hInst = NULL;											   //实例句柄
LPDIRECT3DDEVICE9	g_pd3dDevice9 = NULL;							   //Direct3D设备对象(COM接口对象）

LPD3DXFONT	g_pFont1 =  NULL;										   //字体对象1（COM接口对象)
LPD3DXFONT	g_pFont2 = NULL;										   //字体对象2（COM接口对象)
LPD3DXFONT	g_pFont3 = NULL;										   //字体对象3（COM接口对象)
LPD3DXFONT	g_pFont4 = NULL;										   //字体对象4（COM接口对象)

wchar_t		g_str[50];												   //用于输出的字符串数组

XFileModelClass * g_XFileModel = NULL;								   //X文件模型的定义
XFileModelClass * g_XWaterOrge = NULL;								   //水元素
XFileModelClass * g_XPhoenix = NULL;								   //火凤凰
XFileModelClass * g_XBingjing = NULL;								   //冰晶(英雄联盟英雄）

SkyBoxClass * g_pSkyBox = NULL;										   //定义一个天空盒类对象
TerrainClass * g_pTerrain = NULL;									   //定义一个地形类对象
DirectInputClass * g_pDirectInput = NULL;							   //鼠标和键盘输入类对象
CameraClass * g_pCamera = NULL;										   //定义一个摄像机类对象
TornadoClass * g_Tornado = NULL;									   //定义一个龙卷风粒子类对象

D3DXMATRIX	g_matWorld;												   //定义一个世界变换矩阵
D3DXMATRIX  g_matRX;												   //定义一个绕X轴旋转的旋转矩阵
D3DXMATRIX  g_matRZ;												   //定义一个绕Z轴旋转的旋转矩阵
D3DXMATRIX  g_matRY;												   //定义一个绕Y轴旋转的旋转矩阵
int			g_cxClient = 1280;										   //窗口客户区的宽度
int			g_cyClient = 775;										   //窗口客户区的高度

//-----------------------------------【GUI相关的全局变量声明】---------------------------------------
int			g_cxMouse = 0;											   //鼠标指针X位置
int			g_cyMouse = 0;											   //鼠标指针Y位置
BOOL		g_bLMBDown = false;										   //判断鼠标左键是否按下的标志
BOOL		g_fGUICanRender = true;									   //判断是否需要GUI渲染的标志

GUIClass	* g_pGUIMain = NULL;
GUIClass	* g_pGUIStart = NULL;
GUIClass	* g_pGUILoad = NULL;
GUIClass	* g_pGUIOption = NULL;	
GUIClass    * g_pGUIPresent = NULL;									   //5个GUI类的对象

int			g_currentGUI = GUI_BKGROUND_MAIN;						   //默认的GUI标识
WORD		g_GUIMainFont = -1;
WORD		g_GUIStartFont = -1;
WORD		g_GUILoadFont = -1;
WORD		g_GUIOptionFont = -1;
WORD		g_GUIPresentFont = -1;									  //5个GUI字体对象的索引
//--------------------------------------------------------------------------------------------------

//四个和骨骼动画相关的全局变量
LPD3DXFRAME                 g_pFrameRoot = NULL;
D3DXMATRIX*                 g_pBoneMatrices = NULL;
CAllocateHierarchy*         g_pAllocateHier = NULL;
LPD3DXANIMATIONCONTROLLER   g_pAnimController = NULL;


//【全局函数声明】
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);				   //窗口过程函数
HRESULT Direct3D_Init(HWND hwnd);									   //在该函数中进行Direct3D初始化
HRESULT Object_Init(HWND hwnd);										   //在该函数中进行绘制物体资源的初始化
VOID  Direct3D_Update(HWND hwnd, float fElapsedTime);				   //在该函数中作渲染的更新准备
VOID  Direct3D_Render(HWND hwnd, float fElapsedTime);				   //在该函数中进行Direct3D的渲染
VOID  Direct3D_CleanUp();											   //在该函数中进行COM资源及其他资源的清理
VOID  GUI_Render(HWND hwnd, BOOL& fCanRender);						   //GUI相关场景渲染
VOID  HelpText_Render(HWND);										   //在该函数中渲染帮助文本
VOID  Light_Set(LPDIRECT3DDEVICE9, int);							   //在该函数中进行光源的设置
VOID  CALLBACK GUICallBack(int Id, int State);						   //GUI回调函数
VOID  ClipClient(HWND hwnd, BOOL CursorVisable);					   //该函数将限制鼠标在客户区中
float GetFPS();														   //得到每秒的帧率（Frame Per Second)	

//------------------------------------------------------------------------------------------------
// 【顶点缓存使用四步曲之一】：设计顶点格式
//------------------------------------------------------------------------------------------------
typedef struct
{
	float x, y, z;
	DWORD color;
}CUSTOMVERTEX, *LPCUSTOMVERTEX;

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

//----------------------------------------【WinMain主函数】-------------------------------------------
//  描述：Windows程序的入口函数，程序从这里开始
//----------------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wndclass;
	HWND	   hwnd;
	MSG		   msg = { 0 };

	g_hInst = hInstance;
	//【窗口绘制四步曲之一】-----------------------------设计窗口类

	wndclass.cbSize = sizeof (WNDCLASSEX);								//结构大小
	wndclass.style = CS_HREDRAW | CS_VREDRAW;							//窗口风格样式
	wndclass.lpfnWndProc = WndProc;										//设置窗口过程地址
	wndclass.cbClsExtra = 0;											//额外的窗口类内存空间
	wndclass.cbWndExtra = 0;											//额外的窗口内存空间
	wndclass.hInstance = hInstance;										//窗口的实例句柄
	wndclass.hIcon = (HICON)::LoadImage(NULL, L"GameMedia/kakashi.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	//加载图标
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);						//加载鼠标指针
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);		//加载窗口背景
	wndclass.lpszMenuName = NULL;										//窗口菜单名称
	wndclass.lpszClassName = lpClassName;								//窗口类名称
	wndclass.hIconSm = 0;												//任务栏中的小图标

	//【窗口绘制四步曲之二】-----------------------------注册窗口类

	if (!RegisterClassEx(&wndclass))
	{
		MessageBox(NULL, TEXT("窗口类注册失败"), lpClassName, MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	//【窗口绘制四步曲之三】-----------------------------创建窗口

	hwnd = CreateWindow(lpClassName, WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, g_cxClient, g_cyClient,
		NULL, NULL, hInstance, NULL);                                  //父窗口句柄，菜单句柄，实例句柄，额外字段


	if (S_OK != Direct3D_Init(hwnd))
	{
		MessageBox(hwnd, _T("Direct3D初始化资源失败！！"), _T("我爱杨的消息窗口"), MB_OK);
		return E_FAIL;
	}

	//【窗口绘制四步曲之四】-----------------------------移动,显示和更新窗口

	MoveWindow(hwnd, 0, 0, g_cxClient, g_cyClient, TRUE);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//播放背景音乐
	PlaySound(_T("GameMedia/天下贰 - 太古铜门.wav"), NULL, SND_ASYNC | SND_FILENAME);

	//--------------------------------------------------------------------------------------
	//  【DirectInput初始化】
	//--------------------------------------------------------------------------------------
	g_pDirectInput = new DirectInputClass();
	g_pDirectInput->KeyBoardInit(g_hInst, hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	g_pDirectInput->MouseInit(g_hInst, hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	//进入游戏消息循环

	static DWORD fCurrentTime = timeGetTime();
	static DWORD fLastTime = timeGetTime();
	static FLOAT fElapsedTime = 0.0f;

	while (msg.message != WM_QUIT)
	{

		fLastTime = fCurrentTime;
		fCurrentTime = timeGetTime();
		fElapsedTime = (fCurrentTime - fLastTime) / 1000.0f;

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (!g_fGUICanRender)
			{
				Direct3D_Update(hwnd, fElapsedTime);
				Direct3D_Render(hwnd, fElapsedTime);
			}
			else
				GUI_Render(hwnd, g_fGUICanRender);
		}
	}

	//注销窗口类

	UnregisterClass(lpClassName, wndclass.hInstance);
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_SIZE:
		g_cxClient = LOWORD(lParam);
		g_cyClient = HIWORD(lParam);
		if (g_cxClient == 0 || g_cyClient == 0)
			break;
		ClipClient(hwnd,false);
		ShowCursor(true);
		return 0;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(hwnd);
		return 0;

	case WM_LBUTTONDOWN:
		g_bLMBDown = true;
		return 0;

	case WM_LBUTTONUP:
		g_bLMBDown = false;
		return 0;

	case WM_MOUSEMOVE:
		g_cxMouse = LOWORD(lParam) + GetSystemMetrics(SM_CXBORDER);
		g_cyMouse = HIWORD(lParam) + GetSystemMetrics(SM_CYCAPTION);
		return 0;

	case WM_PAINT:
		g_fGUICanRender ? GUI_Render(hwnd, g_fGUICanRender): Direct3D_Render(hwnd, 0.0f);
		ValidateRect(hwnd, NULL);
		return 0;

	case WM_DESTROY:
		Direct3D_CleanUp();
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}
//--------------------------------------------【Direct3D_Init】---------------------------------------------
//  描述:在该函数中进行Direct3D初始化
//----------------------------------------------------------------------------------------------------------
HRESULT Direct3D_Init(HWND hwnd)
{
	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之一，创接口】：创建Direct3D接口对象, 以便用该Direct3D对象创建Direct3D设备对象
	//--------------------------------------------------------------------------------------
	LPDIRECT3D9		pD3D9;										//创建Direct3D接口对象
	if (FAILED(pD3D9 = Direct3DCreate9(D3D_SDK_VERSION)))	    //初始化Direct3D接口对象，并与DirectX_SDK版本协商，
		return E_FAIL;											//以便包含的头文件与DirectX Runtime匹配

	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之二,取信息】：获取硬件设备信息
	//--------------------------------------------------------------------------------------
	D3DCAPS9 caps; int vp = 0;
	if (FAILED(pD3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
	{
		return E_FAIL;
	}
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;				//支持硬件顶点运算方式
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;				//支持软件顶点运算方式

	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之三，填内容】：填充D3DPRESENT_PARAMETERS结构体
	//--------------------------------------------------------------------------------------

	D3DPRESENT_PARAMETERS	d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = g_cxClient;							//后台缓冲区宽度
	d3dpp.BackBufferHeight = g_cyClient;						//后台缓冲区高度
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;					//后台缓冲区保存像素格式
	d3dpp.BackBufferCount = 2;									//后台缓冲区数量
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;				//多重采样的类型
	d3dpp.MultiSampleQuality = 0;								//多重采样的格式
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					//指定如何将后台缓冲区内容复制到前台的缓存中
	d3dpp.hDeviceWindow = hwnd;									//当前与设备关联的窗口，被激活的窗口
	d3dpp.Windowed = true;										//对激活窗口表示是否使用窗口模式
	d3dpp.EnableAutoDepthStencil = true;						//自动管理深度缓存，模版缓存
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				//深度缓存，模版缓存格式，(这里为24位深度，8位模版)
	d3dpp.Flags = 0;											//附加属性
	d3dpp.FullScreen_RefreshRateInHz = 0;						//全屏模式时的屏幕刷新率
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; //后台缓冲区和前台缓冲区的最大交换频率


	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之四，创设备】：创建Direct3D设备接口
	//--------------------------------------------------------------------------------------

	if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, vp, &d3dpp, &g_pd3dDevice9)))
	{
		SAFE_RELEASE(pD3D9);
		return E_FAIL;
	}
	SAFE_RELEASE(pD3D9);		//Direct3D接口对象使命完成，我们将其释放掉


	if (S_OK != Object_Init(hwnd))
		return E_FAIL;

	return S_OK;
}
//--------------------------------------------【Object_Init】------------------------------------------------
//  描述：在该函数中进行绘制物体资源的初始化
//-----------------------------------------------------------------------------------------------------------
HRESULT Object_Init(HWND hwnd)
{
	//创建字体
	HR(D3DXCreateFont(g_pd3dDevice9, 36, 0, 0, 0, FALSE, DEFAULT_CHARSET,     //创建微软雅黑字体
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("Clambria"), &g_pFont1));

	HR(D3DXCreateFont(g_pd3dDevice9, 36, 0, 0, 0, FALSE, DEFAULT_CHARSET,	  //创建Fixeds字体
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("楷体"), &g_pFont2));

	HR(D3DXCreateFont(g_pd3dDevice9, 25, 0, 700, 0, FALSE, DEFAULT_CHARSET,	  //创建黑体字体
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("仿宋"), &g_pFont3));

	HR(D3DXCreateFont(g_pd3dDevice9, 30, 0, 0, 0, FALSE, DEFAULT_CHARSET,     //创建微软雅黑字体
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("Clambria"), &g_pFont4));

	srand(GetTickCount());	//用操作系统启动到现在的时间来初始化随机种子

	//初始化主模型
	g_XFileModel = new XFileModelClass(g_pd3dDevice9);
	g_XFileModel->XFileModelLoadFromFile(_T("XFile/FlyDragon.X"));

	// 创建骨骼动画
	g_pAllocateHier = new CAllocateHierarchy();
	D3DXLoadMeshHierarchyFromX(L"XFile/FlyDragon.X", D3DXMESH_MANAGED, g_pd3dDevice9,
		g_pAllocateHier, NULL, &g_pFrameRoot, &g_pAnimController);
	SetupBoneMatrixPointers(g_pFrameRoot, g_pFrameRoot);

	//因为这个X文件中仅有一个默认的振翅动作，所以以下代码可用可不用
	 	//LPD3DXANIMATIONSET pAnimationSet = NULL;
	 	//g_pAnimController->GetAnimationSetByName("FlyDragon", &pAnimationSet);
	 	//g_pAnimController->SetTrackAnimationSet((UINT)1.0, pAnimationSet);

	//初始化水元素
	g_XWaterOrge = new XFileModelClass(g_pd3dDevice9);
	g_XWaterOrge->XFileModelLoadFromFile(TEXT("XFile/WaterOrge.X"));

	//初始化火凤凰
	g_XPhoenix = new XFileModelClass(g_pd3dDevice9);
	g_XPhoenix->XFileModelLoadFromFile(TEXT("XFile/Phoenix.X"));

	//初始化冰晶
	g_XBingjing = new XFileModelClass(g_pd3dDevice9);
	g_XBingjing->XFileModelLoadFromFile(_T("XFile/bingjing.X"));

	//初始化龙卷风粒子
	g_Tornado = new TornadoClass(g_pd3dDevice9);

	TCHAR *ppFileName[] =
	{
		TEXT("GameMedia/wind_00x00.jpg"),
		TEXT("GameMedia/wind_00x01.jpg"),
		TEXT("GameMedia/wind_00x02.jpg"),
		TEXT("GameMedia/wind_00x03.jpg"),
		TEXT("GameMedia/wind_00x04.jpg"),
		TEXT("GameMedia/wind_01x00.jpg"),
		TEXT("GameMedia/wind_01x01.jpg"),
		TEXT("GameMedia/wind_01x02.jpg"),
		TEXT("GameMedia/wind_01x03.jpg"),
		TEXT("GameMedia/wind_01x04.jpg"),
		TEXT("GameMedia/Particles_00x00.jpg"),
		TEXT("GameMedia/Particles_00x03.jpg"),
		TEXT("GameMedia/Particles_00x04.jpg"),
		TEXT("GameMedia/Particles_01x00.jpg"),
		TEXT("GameMedia/Particles_01x01.jpg"),
		TEXT("GameMedia/Particles_01x02.jpg"),
		TEXT("GameMedia/Particles_01x03.jpg"),
		TEXT("GameMedia/Particles_01x04.jpg"),
		TEXT("GameMedia/Particles_02x00.jpg"),
		TEXT("GameMedia/Particles_02x02.jpg"),
		TEXT("GameMedia/Particles_02x03.jpg"),
		TEXT("GameMedia/Particles_02x04.jpg"),
		TEXT("GameMedia/Particles_03x00.jpg"),
		TEXT("GameMedia/Particles_03x01.jpg"),
		TEXT("GameMedia/Particles_03x02.jpg"),
		TEXT("GameMedia/Particles_03x03.jpg"),

	};
	g_Tornado->LoadTextureFromFile(ppFileName, 26);
	g_Tornado->TornadoInit(256.0f, 90.0f);

	//初始化地形
	g_pTerrain = new TerrainClass(g_pd3dDevice9);
	g_pTerrain->LoadTerrainFromFile(TEXT("GameMedia/heighmap.raw"), TEXT("GameMedia/Terrain.jpg"));
	g_pTerrain->TerrainInit(3.0f, 200, 200, 200, 400.0f, 65.0f);

	//初始化天空盒
	g_pSkyBox = new SkyBoxClass(g_pd3dDevice9);
	g_pSkyBox->SkyBoxInit(200000);
	g_pSkyBox->LoadTextureFromFile(TEXT("GameMedia/FullMoonFront2048.png"), TEXT("GameMedia/FullMoonBack2048.png"),
		TEXT("GameMedia/FullMoonLeft2048.png"), TEXT("GameMedia/FullMoonRight2048.png"), 
		TEXT("GameMedia/FullMoonTop2048.png"), TEXT("GameMedia/FullMoonBottom2048.png"));

	//初始化摄像机
	g_pCamera = new CameraClass(g_pd3dDevice9);
	g_pCamera->SetCameraPos(&D3DXVECTOR3(0.0f, 3000.0f, -50.0f));
	g_pCamera->SetLookatPos(&D3DXVECTOR3(0.0f, 3000.0f, 0.0f));
	g_pCamera->SetMatrixView();
	g_pCamera->SetMatrixProj();

	//设置渲染状态
	g_pd3dDevice9->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pd3dDevice9->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(255, 255, 255));

	//初始单位化矩阵
	D3DXMatrixIdentity(&g_matRX);
	D3DXMatrixIdentity(&g_matRY);
	D3DXMatrixIdentity(&g_matRZ);
	D3DXMatrixIdentity(&g_matWorld);

//----------------------------------------【GUI相关资源的添加】----------------------------------------------
	//GUI对象的初始化
	g_pGUIMain = new GUIClass(g_pd3dDevice9, g_cxClient, g_cyClient);
	g_pGUIStart = new GUIClass(g_pd3dDevice9, g_cxClient, g_cyClient);
	g_pGUILoad = new GUIClass(g_pd3dDevice9, g_cxClient, g_cyClient);
	g_pGUIOption = new GUIClass(g_pd3dDevice9, g_cxClient, g_cyClient);
	g_pGUIPresent = new GUIClass(g_pd3dDevice9, g_cxClient, g_cyClient);

	//添加GUI字体对象
	g_pGUIMain->CreateTextFont(_T("微软雅黑"), 36, &g_GUIMainFont);
	g_pGUIStart->CreateTextFont(_T("Clambria"), 36, &g_GUIStartFont);
	g_pGUILoad->CreateTextFont(_T("仿宋"), 36, &g_GUILoadFont);
	g_pGUIOption->CreateTextFont(_T("微软雅黑"), 36, &g_GUIOptionFont);
	g_pGUIPresent->CreateTextFont(_T("楷体"), 36, &g_GUIPresentFont);

	//添加背景图片
	if (!g_pGUIMain->AddBackground(TEXT("GUI/main.jpg")))	return E_FAIL;
	if (!g_pGUIStart->AddBackground(TEXT("GUI/startgame.jpg")))	return E_FAIL;
	if (!g_pGUILoad->AddBackground(TEXT("GUI/loadgame.jpg")))	return E_FAIL;
	if (!g_pGUIOption->AddBackground(TEXT("GUI/option.jpg")))	return E_FAIL;
	if (!g_pGUIPresent->AddBackground(TEXT("GUI/present.jpg"))) return E_FAIL;


	//---------------------------------【主菜单main页面相关的页面布局代码】---------------------------------
	// 添加静态文本到页面中
	if (!g_pGUIMain->AddStaticText(TEXT("Version 我爱杨V2.0倾情制造版"), GUI_STATIC_TEXT,
		D3DCOLOR_RGBA(255, 255, 255, 210), 850, 720, g_GUIMainFont)) return false;

	// 添加4个按钮，分别是开始游戏，载入进度，选项和退出游戏，每个按钮对应3幅图
	if (!g_pGUIMain->AddButton(GUI_BUTTON_START, 585, 340, TEXT("GUI/startDown.png"),
		TEXT("GUI/StartUp.png"), TEXT("GUI/startOver.png"))) return false;

	if (!g_pGUIMain->AddButton(GUI_BUTTON_LOAD, 585, 385, TEXT("GUI/loadDown.png"),
		TEXT("GUI/loadUp.png"), TEXT("GUI/loadOver.png"))) return false;

	if (!g_pGUIMain->AddButton(GUI_BUTTON_OPTION, 585, 430, L"GUI/optionsDown.png",
		L"GUI/optionsUp.png", L"GUI/optionsOver.png")) return false;

	if (!g_pGUIMain->AddButton(GUI_BUTTON_QUIT, 585, 475, L"GUI/quitDown.png",
		L"GUI/quitUp.png", L"GUI/quitOver.png")) return false;


	//------------------------【开始新游戏start页面相关的页面布局代码】------------------------
	// 添加按钮到页面中

	if (!g_pGUIStart->AddButton(GUI_BUTTON_BACK, 750, 400, L"GUI/backDown.png", L"GUI/backUp.png",
		L"GUI/backOver.png")) return false;

	if (!g_pGUIStart->AddButton(GUI_BUTTON_FLYMODE, 300, 200, L"GUI/飞行模式down.png", L"GUI/飞行模式up.png",
		L"GUI/飞行模式over.png")) return false;

	//------------------------【载入游戏load页面相关的页面布局代码】--------------------------
	// 添加按钮到页面中
	if (!g_pGUILoad->AddButton(GUI_BUTTON_BACK, 750, 400, L"GUI/backDown.png", L"GUI/backUp.png",
		L"GUI/backOver.png")) return false;


	//------------------------【游戏设置option页面相关的页面布局代码】------------------------
	// 添加按钮到页面中
	if (!g_pGUIOption->AddButton(GUI_BUTTON_BACK, 750, 400, L"GUI/backDown.png", L"GUI/backUp.png",
		L"GUI/backOver.png")) return false;
	//添加静态文本到页面中
	if (!g_pGUIOption->AddStaticText(L"这里是Option页面", GUI_STATIC_TEXT,
		D3DCOLOR_XRGB(33, 55, 255), 540, 60, g_GUIOptionFont)) return false;

	//------------------------【读取画面loading页面相关的页面布局代码】------------------------
	//添加静态文本到页面中
	if (!g_pGUIPresent->AddStaticText(
		TEXT("永远都不要否认自己存在的意义和价值\n")
		TEXT("因为\n")
		TEXT("你是自由的\n\n")
		TEXT("永远都不要放弃对力量和信念的追求和渴望\n")
		TEXT("因为\n")
		TEXT("你是神圣的\n\n")
		TEXT("真龙已死\n")
		TEXT("雏儿不足为惧\n")
		TEXT("我们靠自己的力量一起征服天下吧\n")
		TEXT("我们将和幽都的力量一起\n")
		TEXT("横扫这个腐朽而又堕落的凡间\n")
		TEXT("把真正的自由和力量散布到整个大荒\n\n")
		TEXT("天下  我来了\n")
		TEXT("             ---《玄玑变》"),
		GUI_STATIC_TEXT,
		D3DCOLOR_XRGB(255, 255, 200), 80, 90, g_GUIPresentFont)) return false;

	//添加按钮到页面中
	if (!g_pGUIPresent->AddButton(GUI_BUTTON_SKIP, 1070, 700, L"GUI/跳过Down.png", L"GUI/跳过Up.png",
		L"GUI/跳过Over.png")) return false;
//-------------------------------------------------------------------------------------------------------

	return S_OK;
}
//--------------------------------------------【Direct3D_Update】--------------------------------------------
//  描述：在该函数中更新与画面渲染无关的动作
//-----------------------------------------------------------------------------------------------------------
VOID Direct3D_Update(HWND hwnd,float fElapsedTime)
{

	//龙卷风粒子的更新
	g_Tornado->TornadoUpdate(fElapsedTime);

	//获取鼠标和键盘键位状态
	g_pDirectInput->GetKeyboardInput();
	g_pDirectInput->GetMouseInput();

	//根据键盘1和2键情况，分别设置线框或实体渲染状态
	if (g_pDirectInput->IsKeyboardDown(DIK_1))
		g_pd3dDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	if (g_pDirectInput->IsKeyboardDown(DIK_2))
		g_pd3dDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//摄像机翱翔控制
	if (g_pDirectInput->IsKeyboardDown(DIK_R))	g_pCamera->TranslateAlongY(20.0f);
	if (g_pDirectInput->IsKeyboardDown(DIK_F))  g_pCamera->TranslateAlongY(-20.0f);
	if (g_pDirectInput->IsKeyboardDown(DIK_A))  g_pCamera->TranslateAlongX(-20.0f);
	if (g_pDirectInput->IsKeyboardDown(DIK_D))  g_pCamera->TranslateAlongX(20.0f);
	if (g_pDirectInput->IsKeyboardDown(DIK_S))  g_pCamera->TranslateAlongZ(-50.0f);
	if (g_pDirectInput->IsKeyboardDown(DIK_W))  g_pCamera->TranslateAlongZ(50.0f);


	//初始化单位矩阵
	D3DXMatrixIdentity(&g_matRX);
	D3DXMatrixIdentity(&g_matRY);
	D3DXMatrixIdentity(&g_matRZ);

	//键盘旋转操作
	if (g_pDirectInput->IsKeyboardDown(DIK_Q))	g_matRZ *= g_pCamera->CameraRotationZ(0.005f);
	else if (g_pDirectInput->IsKeyboardDown(DIK_E))	g_matRZ *= g_pCamera->CameraRotationZ(-0.005f);

	//键盘视点察看（由于在下面代码行中鼠标不动时，矩阵会被单位化，造成特殊效果）
	if (g_pDirectInput->IsKeyboardDown(DIK_DOWN))	 g_pCamera->CameraRotationX(0.005f);
	else if (g_pDirectInput->IsKeyboardDown(DIK_UP))	 g_pCamera->CameraRotationX(-0.005f);

	if (g_pDirectInput->IsKeyboardDown(DIK_RIGHT))	g_pCamera->CameraRotationY(0.005f);
	else if (g_pDirectInput->IsKeyboardDown(DIK_LEFT))	 g_pCamera->CameraRotationY(-0.005f);

	//鼠标旋转操作
	if(g_pDirectInput->GetMouselX()!= 0)
		g_matRY = g_pCamera->CameraRotationY(g_pDirectInput->GetMouselX() * 0.001f);
	if(g_pDirectInput->GetMouselY() != 0)
		g_matRX = g_pCamera->CameraRotationX(g_pDirectInput->GetMouselY() * 0.001f);

	//计算并设置取景变换矩阵
	g_pCamera->SetMatrixView();

}

//--------------------------------------------【Direct3D_Render】--------------------------------------------
//  描述：在该函数中进行Direct3D的渲染
//-----------------------------------------------------------------------------------------------------------
VOID Direct3D_Render(HWND hwnd, float fElapsedTime)
{
	//----------------------------------------------------------------------------------
	//【Direct3D渲染五步曲之一】：清屏操作
	//----------------------------------------------------------------------------------
	g_pd3dDevice9->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 100, 200), 1.0f, 0);

	//----------------------------------------------------------------------------------
	//【Direct 3D渲染五步曲之二】：开始场景
	//----------------------------------------------------------------------------------
	g_pd3dDevice9->BeginScene();

	//----------------------------------------------------------------------------------
	//【Direct 3D渲染五步曲之三】：正式绘制
	//----------------------------------------------------------------------------------

	//渲染主飞行模型
	static FLOAT fscale = 0.015f;
	static D3DXMATRIX matTrans, matScaling;
	static D3DXMATRIX matWorld = g_XFileModel->SetTransformNotExactly(fscale,
		0.0f, 0.0f, 0.0f, -9, 270, 0);
	D3DXMatrixTranslation(&matTrans, g_pCamera->GetLookatPos()->x,
		g_pCamera->GetLookatPos()->y - 15.0f, g_pCamera->GetLookatPos()->z);

	fscale = 1.0f + g_pDirectInput->GetMouselZ() * 0.0001f;
	D3DXMatrixScaling(&matScaling, fscale, fscale, fscale);

	matWorld = matScaling * matWorld * g_matRY * g_matRX * g_matRZ;
	g_pd3dDevice9->SetTransform(D3DTS_WORLD, &(matWorld * matTrans));
	//g_XFileModel->RenderModel();

	// 更新骨骼动画
	g_pAnimController->AdvanceTime(1.4f * fElapsedTime, NULL);  //设置骨骼动画的时间
	UpdateFrameMatrices(g_pFrameRoot, &(matWorld * matTrans));   //更新框架中的变换矩阵
	DrawFrame(g_pd3dDevice9, g_pFrameRoot);

	//渲染水元素
	g_XWaterOrge->SetTransform(1000.0f, 0.0f, 20000.0f, 50000.0f, 0, 248);
	g_XWaterOrge->RenderModel();

	//渲染火凤凰
	g_XPhoenix->SetTransform(57.0f, -13000.0f, 10000.0f, 25000.0f, -27, 60);
	g_XPhoenix->RenderModel();

	//渲染冰晶
	g_XBingjing->SetTransform(140.0f, 13000.0f, 6000.0f, 25000.0f, 0, 160);
	g_XBingjing->RenderModel();

	//渲染地形
	D3DXMatrixTranslation(&g_matWorld, 0.0f, -10000.0f, 0.0f);
	g_pd3dDevice9->SetTransform(D3DTS_WORLD, &g_matWorld);
	g_pTerrain->TerrainRender(&g_matWorld, false);

	//渲染天空
	D3DXMatrixTranslation(&g_matWorld, 0.0f, 15000.0f, 0.0f);
	D3DXMatrixRotationY(&g_matRY, -D3DX_PI * timeGetTime() / 180000);
	g_matWorld = g_matRY * g_matWorld;
	g_pSkyBox->SkyBoxRender(&g_matWorld, false, 6);

	//渲染龙卷风粒子
	g_Tornado->TornadoRender();

	//渲染帮助文本：
	HelpText_Render(hwnd);

	//----------------------------------------------------------------------------------
	//【Direct 3D渲染五步曲之四】：结束场景
	//----------------------------------------------------------------------------------
	g_pd3dDevice9->EndScene();

	//----------------------------------------------------------------------------------
	//【Direct 3D渲染五步曲之五】：翻转与显示
	//----------------------------------------------------------------------------------
	g_pd3dDevice9->Present(NULL, NULL, NULL, NULL);
	//源矩形区域，目标矩形区域，当前绘制的窗口，最小更新区域

}

//--------------------------------------------【Direct3D_CleanUp】------------------------------------------------
//  描述：在该函数中进行COM资源及其他资源的清理
//------------------------------------------------------------------------------------------------------------
VOID Direct3D_CleanUp()
{
	SAFE_DELETE(g_pDirectInput);			//输入输出类对象内存释放

	SAFE_DELETE(g_XFileModel);
	SAFE_DELETE(g_XWaterOrge);
	SAFE_DELETE(g_XPhoenix);
	SAFE_DELETE(g_XBingjing);				//释放4个模型堆内存
	SAFE_DELETE(g_Tornado);					//删除TornadoClass类对象
	SAFE_DELETE(g_pSkyBox);					//删除SkyBoxClass类对象
	SAFE_DELETE(g_pTerrain);				//删除TerrainClass类对象
	SAFE_DELETE(g_pDirectInput);			//删除DirectInputClass类对象

	SAFE_RELEASE(g_pd3dDevice9);			//释放Direct3D设备接口
	SAFE_RELEASE(g_pFont1);					
	SAFE_RELEASE(g_pFont2);
	SAFE_RELEASE(g_pFont3);
	SAFE_RELEASE(g_pFont4);					//释放4个字体接口对象
}

//---------------------------------------------【GUI_Render()】-----------------------------------------------
// Desc:GUI相关场景的渲染
//------------------------------------------------------------------------------------------------------------
VOID GUI_Render(HWND hwnd, BOOL& fCanRender)
{
	//----------------------------------------------------------------------------------
	//【Direct3D渲染五步曲之一】：清屏操作
	//----------------------------------------------------------------------------------
	g_pd3dDevice9->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 100, 200), 1.0f, 0);

	//----------------------------------------------------------------------------------
	//【Direct 3D渲染五步曲之二】：开始场景
	//----------------------------------------------------------------------------------
	g_pd3dDevice9->BeginScene();

	//----------------------------------------------------------------------------------
	//【Direct 3D渲染五步曲之三】：正式绘制
	//----------------------------------------------------------------------------------
	switch (g_currentGUI)
	{
	case GUI_BKGROUND_MAIN:
		ProcessGUI(g_pGUIMain, g_bLMBDown, g_cxMouse, g_cyMouse, GUICallBack);
		break;

	case GUI_BKGROUND_START:
		ProcessGUI(g_pGUIStart, g_bLMBDown, g_cxMouse, g_cyMouse, GUICallBack);
		break;

	case GUI_BKGROUND_LOAD:
		ProcessGUI(g_pGUILoad, g_bLMBDown, g_cxMouse, g_cyMouse, GUICallBack);
		break;

	case GUI_BKGROUND_OPTION:
		ProcessGUI(g_pGUIOption, g_bLMBDown, g_cxMouse, g_cyMouse, GUICallBack);
		break;

	case GUI_BKGROUND_PRESENT:
		ProcessGUI(g_pGUIPresent, g_bLMBDown, g_cxMouse, g_cyMouse, GUICallBack);
		break;

	case GUI_BKGROUND_RENDER:
		ClipClient(hwnd, false);
		PlaySound(TEXT("GameMedia/青鸟-《火影忍者》.wav"), NULL, SND_ASYNC | SND_FILENAME);
		fCanRender = FALSE;
		return;

	}

	//----------------------------------------------------------------------------------
	//【Direct 3D渲染五步曲之四】：结束场景
	//----------------------------------------------------------------------------------
	g_pd3dDevice9->EndScene();

	//----------------------------------------------------------------------------------
	//【Direct 3D渲染五步曲之五】：翻转与显示
	//----------------------------------------------------------------------------------
	g_pd3dDevice9->Present(NULL, NULL, NULL, NULL);
	//源矩形区域，目标矩形区域，当前绘制的窗口，最小更新区域
}

//------------------------------------------------------------------------------------------------------------
//  描述：封装了光源设置的函数，可以选择点光源，平行光，聚光灯
//------------------------------------------------------------------------------------------------------------
VOID Light_Set(LPDIRECT3DDEVICE9 pd3dDevice9, int iType)
{
	static D3DLIGHT9 light9;
	::ZeroMemory(&light9, sizeof(light9));

	switch (iType)
	{
	case 1://点光源
		light9.Type = D3DLIGHT_POINT;
		light9.Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		light9.Range = 300.0f;
		light9.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		light9.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		light9.Specular = D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f);
		light9.Attenuation0 = 1.0f;
		light9.Attenuation1 = 0.0f;
		light9.Attenuation2 = 0.0f;
		break;

	case 2://平行光
		light9.Type = D3DLIGHT_DIRECTIONAL;
		light9.Direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		light9.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
		light9.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		light9.Specular = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		break;

	case 3://聚光灯
		light9.Type = D3DLIGHT_SPOT;
		light9.Position = D3DXVECTOR3(0.0f, -50.0f, 0.0f);
		light9.Direction = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		light9.Range = 300.0f;
		light9.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.5f);
		light9.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		light9.Specular = D3DXCOLOR(0.3f, 0.3f, 1.0f, 1.0f);
		light9.Attenuation0 = 1.0f;
		light9.Attenuation1 = 0.0f;
		light9.Attenuation2 = 0.0f;
		light9.Falloff = 0.1f;
		light9.Phi = D3DX_PI / 3.0f;
		light9.Theta = D3DX_PI / 6.0f;
		break;
	}
	pd3dDevice9->SetLight(0, &light9);
	pd3dDevice9->LightEnable(0, true);
}

//----------------------------------------【HelpText_Render()函数】-------------------------------------------
// Desc:帮助文本信息的输出
//------------------------------------------------------------------------------------------------------------
VOID HelpText_Render(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);

	//在窗口右上角输出帧率
	int icharCount = swprintf_s(g_str, 20, _T("FPS:%0.3f"), GetFPS());
	g_pFont1->DrawText(NULL, g_str, icharCount, &rect, DT_RIGHT | DT_TOP, D3DCOLOR_XRGB(255, 10, 10));

	//显示当前模型坐标：
	icharCount = swprintf_s(g_str, TEXT("模型坐标:\n(%0.f, %0.f, %0.f)"),
		g_pCamera->GetLookatPos()->x, g_pCamera->GetLookatPos()->y, g_pCamera->GetLookatPos()->z);
	g_pFont4->DrawText(NULL, g_str, icharCount, &rect, DT_LEFT | DT_TOP, D3DXCOLOR(0.1f, 1.0f, 0.1f, 0.8f));

	//输出帮助文本信息
	rect.top = 600;
	g_pFont2->DrawText(NULL, TEXT("ず我愛杨ぢ的指引："), -1, &rect,
		DT_LEFT | DT_NOCLIP | DT_SINGLELINE, D3DCOLOR_RGBA(255, 255, 0, 255));

	rect.top += 36;
	g_pFont3->DrawText(NULL, TEXT(" W:向前飞翔， S:向后飞翔"), -1, &rect,
		DT_LEFT | DT_NOCLIP | DT_SINGLELINE, D3DCOLOR_RGBA(0, 255, 100, 255));

	rect.top += 29;
	g_pFont3->DrawText(NULL, TEXT(" A:向左飞翔， D:向右飞翔"), -1, &rect,
		DT_LEFT | DT_NOCLIP | DT_SINGLELINE, D3DCOLOR_RGBA(0, 255, 255, 255));

	rect.top += 29;
	g_pFont3->DrawText(NULL, TEXT(" R:腾空，     F:降落"), -1, &rect,
		DT_LEFT | DT_NOCLIP | DT_SINGLELINE, D3DCOLOR_RGBA(80, 80, 255, 225));

	rect.top += 29;
	g_pFont3->DrawText(NULL, TEXT(" Q:向左滑翔， E:向右滑翔"), -1, &rect,
		DT_LEFT | DT_NOCLIP | DT_SINGLELINE, D3DCOLOR_RGBA(255, 80, 80, 255));

}

//-----------------------------------------------【GUICallBack()函数】----------------------------------------
// Desc:GUI回调函数（更改GUI对象标识和鼠标状态）
//------------------------------------------------------------------------------------------------------------
VOID CALLBACK GUICallBack(int Id, int State)
{
	switch (Id)
	{
	case GUI_BUTTON_BACK:
		if (State == GUI_LBUTTON_DOWN)
			g_currentGUI = GUI_BKGROUND_MAIN;
		break;

	case GUI_BUTTON_START:
		if (State == GUI_LBUTTON_DOWN)
			g_currentGUI = GUI_BKGROUND_START;
		break;

	case GUI_BUTTON_LOAD:
		if (State == GUI_LBUTTON_DOWN)
			g_currentGUI = GUI_BKGROUND_LOAD;
		break;

	case GUI_BUTTON_OPTION:
		if (State == GUI_LBUTTON_DOWN)
			g_currentGUI = GUI_BKGROUND_OPTION;
		break;

	case GUI_BUTTON_FLYMODE:
		if (State == GUI_LBUTTON_DOWN)
		{
			g_currentGUI = GUI_BKGROUND_PRESENT;
			PlaySound(TEXT("GameMedia/天下贰 - 玄玑变.wav"), NULL, SND_ASYNC | SND_FILENAME);
		}
		break;

	case GUI_BUTTON_SKIP:
		if (State == GUI_LBUTTON_DOWN)
			g_currentGUI = GUI_BKGROUND_RENDER;
		break;

	case GUI_BUTTON_QUIT:
		if (State == GUI_LBUTTON_DOWN)
			PostQuitMessage(0);
		break;

	}
}

//-----------------------------------------------【ClipClient()函数】-----------------------------------------
// Desc:该函数将限制鼠标在客户区中
//------------------------------------------------------------------------------------------------------------
VOID ClipClient(HWND hwnd, BOOL CursorVisable)
{
	POINT lt, rb;
	RECT rect;
	GetClientRect(hwnd, &rect);
	lt.x = rect.left;
	lt.y = rect.top;
	rb.x = rect.right;
	rb.y = rect.bottom;

	ClientToScreen(hwnd, &lt);
	ClientToScreen(hwnd, &rb);

	rect.left = lt.x;
	rect.top = lt.y;
	rect.right = rb.x;
	rect.bottom = rb.y;

	ClipCursor(&rect);
	ShowCursor(CursorVisable);

}

//--------------------------------------------【GetFPS()函数】-----------------------------------------------
//  描述:计算每秒画面刷新的帧数
//-----------------------------------------------------------------------------------------------------------
float GetFPS()
{
	static float fps = 0;					//每秒的帧率
	static int	 iframeCount = 0;			//图形绘制的帧数
	static DWORD dwlastTime = 0;			//上次获取的时间
	static DWORD dwcurrentTime;				//当前获取的时间

	iframeCount++;

	dwcurrentTime = timeGetTime();

	if ((dwcurrentTime - dwlastTime) > 1000.0f)
	{
		fps = (float)iframeCount * 1000.0f / (dwcurrentTime - dwlastTime);
		dwlastTime = dwcurrentTime;
		iframeCount = 0;
	}

	return fps;
}