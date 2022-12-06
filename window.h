#pragma once
#include <Windows.h>
#include <windowsx.h>
#include "drawing.h"
#include <dwmapi.h>
#include <chrono>
#include "clock.h"
#include "input.h"
#include "driver/include.h"
#pragma comment(lib, "dwmapi.lib")

using render_callback_t = void(*)();

namespace w
{
	//window related info
	HWND hwnd = 0;
	int width = 800;
	int height = 800;
	DWORD flags = 0;
	//WS_OVERLAPPEDWINDOW

	//state of the window (active or not)
	bool active = false;

	//mouse postion relative to window
	float m_x = 0, m_y = 0;

	//frames per second
	float fps;
	float frametime;
	float border = 20.f;

	LRESULT CALLBACK wnd_proc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
		case WM_SETCURSOR:
		{
			SetCursor(NULL);
			d::m_pdevice->SetCursorProperties(0, 0, d::m_pcursorsurface);
			d::m_pdevice->ShowCursor(true);
			return 1;
		}

		case WM_MOUSEMOVE:
		{
			m_x = LOWORD(lparam), m_y = HIWORD(lparam);
			break;
		}

		case WM_LBUTTONDOWN:
		{
			if (m_x < width && m_y < height - (height - border))
			{
				SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, lparam);
			}
			break;
		}
		case WM_NCHITTEST:
		{
			LRESULT hit = DefWindowProc(hWnd, msg, wparam, lparam);

			POINT hit_pos = { GET_X_LPARAM(lparam),GET_Y_LPARAM(lparam) };
			ScreenToClient(hWnd, &hit_pos);

			if (hit_pos.x < border && hit_pos.y < border)
			{
				return HTTOPLEFT;
			}
			else if (hit_pos.x < border && hit_pos.y > height - border)
			{
				return HTBOTTOMLEFT;
			}
			else if (hit_pos.x > width - border && hit_pos.y < border)
			{
				return HTTOPRIGHT;
			}
			else if (hit_pos.x > width - border && hit_pos.y > height - border)
			{
				return HTBOTTOMRIGHT;
			}
			else if (hit_pos.x > width - border)
			{
				return HTRIGHT;
			}
			else if (hit_pos.x < border)
			{
				return HTLEFT;
			}
			else if (hit_pos.y > height - border)
			{
				return HTBOTTOM;
			}

			return hit;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			ExitProcess(4);
			break;
		}

		case WM_SIZE:
		{
			width = LOWORD(lparam);
			height = HIWORD(lparam);
		
			d::on_resize(width, height);
			break;
		}

		default:
		{
			return DefWindowProc(hWnd, msg, wparam, lparam);
			break;
		}
		}

		return 0;
	}

	bool create()
	{
		WNDCLASSEX wc =
		{
			sizeof(WNDCLASSEX),
			CS_HREDRAW | CS_VREDRAW,
			wnd_proc,
			0,
			0,
			nullptr,
			LoadIcon(nullptr, IDI_APPLICATION),
			LoadCursor(nullptr, IDC_ARROW),
			nullptr,
			nullptr,
			" ",
			LoadIcon(nullptr, IDI_APPLICATION)
		};

		if (RegisterClassEx(&wc) == 0) return false;

		RECT wnd = { 0, 0, width, height };
		RECT dt;
		GetClientRect(GetDesktopWindow(), &dt);
		//AdjustWindowRect(&wnd, WS_POPUP, FALSE);
		int w = (wnd.right - wnd.left);
		int h = (wnd.bottom - wnd.top);
		int x = ((dt.right - dt.left) - w) / 2.f;
		int y = ((dt.bottom - dt.top) - h) / 2.f;

		hwnd = CreateWindow(" ", " ", flags, x, y, w, h, NULL, NULL, NULL, NULL);
		if (hwnd == NULL) return false;
		SetWindowLong(hwnd, GWL_STYLE, WS_POPUP);

		EnableWindow(hwnd, true);
		ShowWindow(hwnd, SW_SHOW);

		//fixes gay flags
		SetWindowPos(hwnd, 0, 0, 0, w::width, w::height, SWP_NOMOVE);

		return true;
	}

	bool start_window()
	{
		return create();
	}

	bool start_directx(HWND hwnd)
	{
		return d::startup(hwnd);
	}

	void startup(render_callback_t render_callback)
	{
		MSG msg = { 0 };
		while (msg.message != WM_QUIT && !input::key_pressed(VK_END))
		{
			while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			active = GetFocus() == hwnd;
			using namespace std::chrono;
			auto start = high_resolution_clock::now();
			d::m_pdevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 99, 99, 99), 0, 0);
			d::m_pdevice->BeginScene();
			render_callback();
			d::m_pdevice->EndScene();
			d::m_pdevice->Present(NULL, NULL, NULL, NULL);
			auto end = high_resolution_clock::now();

			duration<double> diff = duration_cast<duration<double>>(end - start);
			microseconds diff_ms = duration_cast<microseconds>(diff);
			double diff_milliseconds = diff_ms.count() / 1000.f;

			c::deltatime = diff_milliseconds;

			fps = 1000.f / diff_milliseconds;
			frametime = diff_milliseconds;
		}

		driver::exit();
	}
}