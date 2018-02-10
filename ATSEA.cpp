// DX11.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <Windows.h>
#include <WindowsX.h>
#include <memory.h>
#include "FBX/FBXExporter.h"
#include "count_performance.h"
using namespace std;
#include "ATSEA.h"
#include "Device.h"
#include "WorldManager.h"
#include "Camera.h"
#include "Render.h"
#define MAX_LOADSTRING 100
extern const float PI;
// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void CameraTick();
UINT32 GetMilliSecond();
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	AllocConsole();
	freopen("CONOUT$", "w+t", stdout);
	freopen("CONIN$", "r+t", stdin);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_ATSEA, szWindowClass, MAX_LOADSTRING);
	ATOM result = MyRegisterClass(hInstance);
	if (!result)
	{
		DWORD error = GetLastError();
		std::cout << error << std::endl;
	}
	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
	//初始化所有数据
	WorldManager::Instance().init();
	AS3DVECTOR3 pos(0,  0, 2000);
	AS3DVECTOR3 target(0,0,0);
	AS3DVECTOR3 up(0.0f, 1.0f, 0.0f);
	Camera::Instance().Init(pos, target, up,1.0f,100000.0f,1,0.3f * PI);
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ATSEA));

	// Main message loop:
	int FramesFromGameBegin = 0;
	while (true)
	{
		/*
		while (GetMessage(&msg, NULL, 0, 0))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		*/
		//这个没有消息无需等待
		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				return 0;
			}
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		static DWORD nLastTime = 0;
		// Update and Draw
		BeginCountPerformance(Update)
			static bool firstTime = true;
		if (firstTime) {
			nLastTime = GetMilliSecond();
			firstTime = false;
		}
		float timeDelta = (GetMilliSecond() - nLastTime);
			WorldManager::Instance().update(timeDelta);
			nLastTime = GetMilliSecond();
		EndCountPerformance(Update)
			//
			BeginCountPerformance(Render)
			Render::Instance().RenderFlow();
		EndCountPerformance(Render)
			//WorldManager::Instance().render();
			CameraTick();

		FramesFromGameBegin++;
	}
	return (int) msg.wParam;


}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ATSEA));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_ATSEA);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 800, 800, NULL, NULL, hInstance, NULL);
	  
   if (!hWnd)
   {
      return FALSE;
   }
   // Demo Initialize
	Device::Instance().init(hWnd);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
void OnMouseWheel(INT32);
void UpdateCamera();
void OnKeyDown(INT32);
void OnKeyUp(INT32);
void OnMouseDown(WPARAM btnState, int x, int y, HWND mhMainWnd);
void OnMouseUp(WPARAM btnState, int x, int y);
void OnMouseMove(WPARAM btnState, int x, int y);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), hWnd);
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	case WM_KEYDOWN:
	{
		OnKeyDown(wParam);
		break;
	}
	case WM_KEYUP:
	{
		OnKeyUp(wParam);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		OnMouseWheel(wParam);
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

POINT mLastMousePos;
float mTheta = 1.5f * PI;
float mPhi = 0.25f * PI;
float mRadius = 1000.0f;

XMFLOAT3 mTargetPoint(0, 0, 0);
// move direction flag
static const UINT32 MD_FORWARD = 0x1;
static const UINT32 MD_BACKWARD = 0x2;
static const UINT32 MD_RIGHT = 0x4;
static const UINT32 MD_LEFT = 0x8;
static const UINT32 MD_UP = 0x10;
static const UINT32 MD_DOWN = 0x20;
UINT32 mMoveDir = 0;
float m_MoveSpeed = 1000;
void OnMouseDown(WPARAM btnState, int x, int y,HWND mhMainWnd)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}

void OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0 || (btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		// Update angles based on input to orbit camera around box.
		mTheta -= dx;
		mPhi -= dy;

		// Restrict the angle mPhi.
		if (mPhi < 0.1f)mPhi = 0.1f;
		if (mPhi > PI - 0.1f) mPhi = PI - 0.1f;
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to 0.005 unit in the scene.
		float dx = 0.005f*static_cast<float>(x - mLastMousePos.x);
		float dy = 0.005f*static_cast<float>(y - mLastMousePos.y);

		// Update the camera radius based on input.
		mRadius += (dx - dy)*100;

		// Restrict the radius.
		//if (mRadius < 3.0f) mRadius = 3.0f;
		//if (mRadius > 15.0f) mRadius = 15.0f;
	}
	mLastMousePos.x = x;
	mLastMousePos.y = y;
	UpdateCamera();
}

void UpdateCamera()
{
	float PosX = mRadius*sinf(mPhi)*cosf(mTheta) + mTargetPoint.x;
	float PosZ = mRadius*sinf(mPhi)*sinf(mTheta) + mTargetPoint.z;
	float PosY = mRadius*cosf(mPhi) + mTargetPoint.y;

	// Build the view matrix.
	AS3DVECTOR3 pos(PosX, PosY, PosZ);
	AS3DVECTOR3 target(mTargetPoint.x, mTargetPoint.y, mTargetPoint.z);
	AS3DVECTOR3 up(0.0f, 1.0f, 0.0f);

	Camera::Instance().LookAt(pos, target, up);
}
void OnKeyDown(INT32 key)
{
	switch (key)
	{
	case 'W':
	{
		mMoveDir |= MD_FORWARD;
		break;
	}
	case 'S':
	{
		mMoveDir |= MD_BACKWARD;
		break;
	}
	case 'A':
	{
		mMoveDir |= MD_LEFT;
		break;
	}
	case 'D':
	{
		mMoveDir |= MD_RIGHT;
		break;
	}
	case 'Z':
	{
		mMoveDir |= MD_DOWN;
		break;
	}
	case ' ':
	{
		mMoveDir |= MD_UP;
		break;
	}
	};
}
void OnMouseWheel(INT32 key)
{
	short zDelta = GET_WHEEL_DELTA_WPARAM(key);
	mRadius += zDelta;
	if (mRadius < 10) mRadius = 10;
	else if (mRadius > 10000)
		mRadius = 10000;
	UpdateCamera();
}
void OnKeyUp(INT32 key)
{
	switch (key)
	{
	case 'W':
	{
		mMoveDir &= ~MD_FORWARD;
		break;
	}
	case 'S':
	{
		mMoveDir &= ~MD_BACKWARD;
		break;
	}
	case 'A':
	{
		mMoveDir &= ~MD_LEFT;
		break;
	}
	case 'D':
	{
		mMoveDir &= ~MD_RIGHT;
		break;
	}
	case 'Z':
	{
		mMoveDir &= ~MD_DOWN;
		break;
	}
	case ' ':
	{
		mMoveDir &= ~MD_UP;
		break;
	}
	case '1':
	{
		WorldManager::Instance().SwitchShaderMode();
		break;
	}
	case '2':
	{
		WorldManager::Instance().ModifyRainIntensify(true);
		break;
	}
	case '3':
	{
		WorldManager::Instance().ModifyRainIntensify(false);
		break;
	}
	case '4':
	{
		WorldManager::Instance().PlayRainDrop();
		break;
	}

	};
}



UINT64 GetMicroSecond()
{
	static LARGE_INTEGER liFrequency;
	static bool bFirstTime = true;

	if (bFirstTime)
	{
		bFirstTime = false;
		::QueryPerformanceFrequency(&liFrequency);
	}

	LARGE_INTEGER liCounter;
	::QueryPerformanceCounter(&liCounter);

	UINT64 uSecond = liCounter.QuadPart / liFrequency.QuadPart;
	UINT64 uRemainder = liCounter.QuadPart % liFrequency.QuadPart;

	return uSecond * 1000000 + uRemainder * 1000000 / liFrequency.QuadPart;
}
UINT32 GetMilliSecond()
{
	return (UINT32)(GetMicroSecond() / 1000);
}
void CameraTick()
{
	
	static UINT32 nLastTime = 0;
	UINT32 nTickTime = 0;
	
	nTickTime = GetMilliSecond() - nLastTime;
	/*
	if (nTickTime < 15)
	{
		Sleep(15 - nTickTime);
		nTickTime = GetMilliSecond() - nLastTime;
	}
	nLastTime = GetMilliSecond();

	if (nTickTime <= 15)
	{
		Sleep(15 - nTickTime);
	}
	*/
	nLastTime = GetMilliSecond();
	
	if (mMoveDir == 0)
		return;
	AS3DVECTOR3 dir = Camera::Instance().GetDir();
	float length = sqrt(dir.x * dir.x + dir.z * dir.z);
	dir.x = dir.x / length;
	dir.z = dir.z / length;
	AS3DVECTOR3 right = Camera::Instance().GetRight();
	if (mMoveDir & MD_FORWARD)
	{
		mTargetPoint.x += m_MoveSpeed * dir.x * nTickTime / 1000;
		mTargetPoint.z += m_MoveSpeed * dir.z * nTickTime / 1000;
	}
	if (mMoveDir & MD_BACKWARD)
	{
		mTargetPoint.x -= m_MoveSpeed * dir.x * nTickTime / 1000;
		mTargetPoint.z -= m_MoveSpeed * dir.z * nTickTime / 1000;
	}
	if (mMoveDir & MD_LEFT)
	{
		mTargetPoint.x -= m_MoveSpeed * right.x * nTickTime / 1000;
		mTargetPoint.z -= m_MoveSpeed * right.z * nTickTime / 1000;
	}
	if (mMoveDir & MD_RIGHT)
	{
		mTargetPoint.x += m_MoveSpeed * right.x * nTickTime / 1000;
		mTargetPoint.z += m_MoveSpeed * right.z * nTickTime / 1000;
	}
	if (mMoveDir & MD_UP)
	{
		mTargetPoint.y += m_MoveSpeed * nTickTime / 1000;
	}
	if (mMoveDir & MD_DOWN)
	{
		mTargetPoint.y -= m_MoveSpeed * nTickTime / 1000;
	}
	UpdateCamera();
}