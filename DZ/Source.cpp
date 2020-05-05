#include <Windows.h>
#include <tchar.h>
#include <cmath>
#include <commdlg.h>
#include "resource.h"

#define PI 3.14159265

COLORREF stdColor = RGB(0, 0, 255);//цвет графика

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);//Оконная процедура главного окна

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;
	MSG msg;
	WNDCLASS wc;
	TCHAR szClassName[] = _T("DZ");
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = szClassName;
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClass(&wc))
		return 0;
	hWnd = CreateWindow(szClassName, _T("Домашнее задание"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 800,
		NULL, NULL, hInstance, NULL);
	
	ShowWindow(hWnd, nCmdShow);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int sx, sy;//клиентские координаты
	HDC hdc;
	PAINTSTRUCT ps;
	static HPEN hPen;
	const int K = 1000;
	static CHOOSECOLOR choiceColor;
	static COLORREF acrCustClr[16];
	const double r = K*2+100;
	const double step = 0.02;
	const double stepForText = K / 10.;
	TCHAR txt[][4] = { _T("0.1"), _T("0.2"), _T("0.3"), _T("0.4"), _T("0.5"), _T("0.6"), _T("0.7"), _T("0.8"), _T("0.9"), _T("1.0") };
	TCHAR txtAngle[][5] = {_T("60°"), _T("240°"), _T("30°"), _T("210°"), _T("330°"), _T("150°"), _T("300°"), _T("120°") };
	switch (msg)
	{
	case WM_CREATE:
		choiceColor.lStructSize = sizeof(CHOOSECOLOR); //инициализация структуры выбора цвета
		choiceColor.hwndOwner = hWnd;
		choiceColor.rgbResult = stdColor;
		choiceColor.Flags = CC_RGBINIT | CC_FULLOPEN;
		choiceColor.lpCustColors = (LPDWORD)acrCustClr;
		break;
	case WM_SIZE:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_QUIT:
			DestroyWindow(hWnd);
			break;
		case ID_CHOICE://нажатие пункта выбора цвета
			if (ChooseColor(&choiceColor))
			{
				stdColor = choiceColor.rgbResult;
				InvalidateRect(hWnd, NULL, true);
			}
			break;
		}
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		SetBkMode(hdc, TRANSPARENT);
		SetMapMode(hdc, MM_ANISOTROPIC);//установка режима
		SetWindowExtEx(hdc, r, -r, NULL);//установка логического размера вывода
		SetViewportExtEx(hdc, sx, sy, NULL);//установка физического размера
		SetViewportOrgEx(hdc, sx / 2, sy / 2, NULL);//установка начала координат
		MoveToEx(hdc, -r / 2, 0, NULL);
		LineTo(hdc, r / 2, 0);//вывод оси X
		MoveToEx(hdc, 0, r / 2, NULL);
		LineTo(hdc, 0, -r / 2);//вывод оси Y
		TextOut(hdc, K*cos(0), 50+K*sin(0), _T("0°"), _tcsclen(_T("0°")));
		TextOut(hdc, K*cos(PI / 2), K*sin(PI / 2), _T("90°"), _tcsclen(_T("90°")));
		TextOut(hdc, K*cos(PI), 50+K*sin(PI), _T("180°"), _tcsclen(_T("180°")));
		TextOut(hdc, K*cos(1.5*PI), K*sin(1.5*PI), _T("270°"), _tcsclen(_T("270°")));
		if (hPen) DeleteObject(hPen);
		hPen = CreatePen(PS_SOLID, NULL, RGB(189, 181, 181));//установка ручки выбранным цветом
		hPen = (HPEN)SelectObject(hdc, hPen);
		for (double a = 0; a < 2 * PI; a += step)//вспомогательные окружности
		{
			MoveToEx(hdc, 0.1*K*cos(a), 0.1*K*sin(a), NULL);
			LineTo(hdc, 0.1*K*cos(a + step), 0.1*K*sin(a + step));
			MoveToEx(hdc, 0.2*K*cos(a), 0.2*K*sin(a), NULL);
			LineTo(hdc, 0.2*K*cos(a + step), 0.2*K*sin(a + step));
			MoveToEx(hdc, 0.3*K*cos(a), 0.3*K*sin(a), NULL);
			LineTo(hdc, 0.3*K*cos(a + step), 0.3*K*sin(a + step));
			MoveToEx(hdc, 0.4*K*cos(a), 0.4*K*sin(a), NULL);
			LineTo(hdc, 0.4*K*cos(a + step), 0.4*K*sin(a + step));
			MoveToEx(hdc, 0.5*K*cos(a), 0.5*K*sin(a), NULL);
			LineTo(hdc, 0.5*K*cos(a + step), 0.5*K*sin(a + step));
			MoveToEx(hdc, 0.6*K*cos(a), 0.6*K*sin(a), NULL);
			LineTo(hdc, 0.6*K*cos(a + step), 0.6*K*sin(a + step));
			MoveToEx(hdc, 0.7*K*cos(a), 0.7*K*sin(a), NULL);
			LineTo(hdc, 0.7*K*cos(a + step), 0.7*K*sin(a + step));
			MoveToEx(hdc, 0.8*K*cos(a), 0.8*K*sin(a), NULL);
			LineTo(hdc, 0.8*K*cos(a + step), 0.8*K*sin(a + step));
			MoveToEx(hdc, 0.9*K*cos(a), 0.9*K*sin(a), NULL);
			LineTo(hdc, 0.9*K*cos(a + step), 0.9*K*sin(a + step));
			MoveToEx(hdc, K*cos(a), K*sin(a), NULL);
			LineTo(hdc, K*cos(a + step), K*sin(a + step));
		}
		int num = 0;
		for (double i = 3; i <= 6; i+=3, num+=2)
		{
			MoveToEx(hdc, -K * cos(PI / i), -K * sin(PI / i), NULL);
			LineTo(hdc, K*cos(PI / i), K*sin(PI / i));
			TextOut(hdc, K*cos(PI / i), K*sin(PI / i), txtAngle[num], _tcsclen(txtAngle[num]));
			TextOut(hdc, -K*cos(PI / i), -K*sin(PI / i), txtAngle[num+1], _tcsclen(txtAngle[num+1]));
		}
		for (double i = -6; i <= -3; i += 3, num += 2)
		{
			MoveToEx(hdc, -K * cos(PI / i), -K * sin(PI / i), NULL);
			LineTo(hdc, K*cos(PI / i), K*sin(PI / i));
			TextOut(hdc, K*cos(PI / i), K*sin(PI / i), txtAngle[num], _tcsclen(txtAngle[num]));
			TextOut(hdc, -K * cos(PI / i), -K * sin(PI / i), txtAngle[num + 1], _tcsclen(txtAngle[num + 1]));
		}
		SelectObject(hdc, hPen);
		FillRgn(hdc, CreateEllipticRgn(8, 8, -8, -8), CreateSolidBrush(RGB(0,0,0)));//точка в центре координат
		TextOut(hdc, -30, -10, _T("0"), 1);
		num = 0;
		for (double i = stepForText; i < K + stepForText; i += stepForText, num++)//вывод меток 
		{
			MoveToEx(hdc, i, 5, NULL);
			LineTo(hdc, i, -5);
			TextOut(hdc, i-stepForText/4., -10, txt[num], _tcsclen(txt[num]));
		}
		if (hPen) DeleteObject(hPen);
		hPen = CreatePen(PS_SOLID, NULL, stdColor);//установка ручки выбранным цветом
		hPen = (HPEN)SelectObject(hdc, hPen);
		for (double a = 0.0; a < 2 * PI; a += step)//график
		{
			MoveToEx(hdc, K*a*0.1*cos(a), K*a*0.1*sin(a), NULL);
			LineTo(hdc, K*(a + step)*0.1*cos(a + step), K*(a + step)*0.1*sin(a + step));
		}
		SelectObject(hdc, hPen);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}