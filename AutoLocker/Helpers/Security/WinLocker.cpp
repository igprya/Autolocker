#include "stdafx.h"
#include "WinLocker.h"

namespace Helpers 
{
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	int WinLocker::Lock()
	{
		if (isLocked) {
			return ECODE_SUCCESS;
		}

		//SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 1);

		printf("Locked\n");

		SecureSceens();

		isLocked = true;

		return ECODE_SUCCESS;
	}

	int WinLocker::Unlock()
	{
		if (!isLocked) {
			return ECODE_SUCCESS;
		}

		//SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, -1);

		printf("Unlocked\n");

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

		// Create the window.

		HWND hwnd = CreateWindowEx(
			0,                              // Optional window styles.
			CLASS_NAME,                     // Window class
			L"Secure window",				// Window text
			WS_OVERLAPPEDWINDOW,            // Window style

											// Size and position
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

			NULL,       // Parent window    
			NULL,       // Menu
			GetModuleHandle(nullptr),  // Instance handle
			NULL        // Additional application data
		);

		if (hwnd == NULL)
		{
			return 0;
		}

		ShowWindow(hwnd, SW_SHOW);

		// Run the message loop.

		MSG msg = {};
		while (GetMessage(&msg, NULL, 0, 0))
		{
			if (!isLocked)
				PostQuitMessage(0);

			TranslateMessage(&msg);
			DispatchMessage(&msg);
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
