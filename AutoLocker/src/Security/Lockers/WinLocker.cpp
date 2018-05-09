#include "stdafx.h"
#include "WinLocker.h"

namespace Security
{
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	int WinLocker::Lock()
	{
		if (isLocked) {
			return ECODE_SUCCESS;
		}
		isLocked = true;
		SecureSceens();
		return ECODE_SUCCESS;
	}


	int WinLocker::Unlock()
	{
		if (!isLocked) {
			return ECODE_SUCCESS;
		}
		isLocked = false;
		return ECODE_SUCCESS;
	}


	bool WinLocker::IsLocked()
	{
		return isLocked;
	}


	int WinLocker::SecureSceens()
	{
		std::thread* windowThread = new std::thread(&WinLocker::CreateSecutiryWindowLock, this);
		return 0;
	}


	int WinLocker::CreateSecutiryWindowLock()
	{
		const wchar_t CLASS_NAME[] = L"Sample Window Class";

		WNDCLASS wc = {};

		wc.lpfnWndProc = WindowProc;
		wc.hInstance = GetModuleHandle(nullptr);
		wc.lpszClassName = CLASS_NAME;

		RegisterClass(&wc);

		// Create the window
		int vs_left =		GetSystemMetrics(SM_XVIRTUALSCREEN);
		int vs_top =		GetSystemMetrics(SM_YVIRTUALSCREEN);
		int vs_width =		GetSystemMetrics(SM_CXVIRTUALSCREEN);
		int vs_height =		GetSystemMetrics(SM_CYVIRTUALSCREEN);

		HWND hwnd = CreateWindowEx(
			0,                              // Optional window styles.
			CLASS_NAME,                     // Window class
			L"Secure window",				// Window text
			WS_POPUP,						// Window style											
			vs_left, vs_top, vs_width, vs_height, // Size and position
			NULL,							// Parent window    
			NULL,							// Menu
			GetModuleHandle(nullptr),		// Instance handle
			NULL							// Additional application data
		);
		
		if (hwnd == NULL)
		{
			return 0;
		}

		ShowWindow(hwnd, SW_SHOW);
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		// Run the message loop.
		MSG msg = {};
		while (GetMessage(&msg, NULL, 0, 0))
		{
			if (isLocked) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				SendMessage(hwnd, WM_DESTROY, 0, 0);
				GetMessage(&msg, NULL, 0, 0);
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				break;
			}
		}

		return 0;
	}


	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;

			case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hwnd, &ps);

				FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

				EndPaint(hwnd, &ps);
			}
			return 0;

		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}
