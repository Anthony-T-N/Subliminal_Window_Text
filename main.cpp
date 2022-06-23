// https://stackoverflow.com/questions/29091028/windows-api-write-to-screen-as-on-screen-display

#define STRICT 1
#define WIN32_LEAN_AND_MEAN
#include <SDKDDKVer.h>
#include <windows.h>
#include <string>
#include <iostream>

#pragma comment(lib, "wsock32.lib")

// Forward declarations: (Note: Allows for functions to be mentioned in advance of the main function.
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Entry point
int main(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpCmdLine*/, int nCmdShow) 
{
    const wchar_t k_WndClassName[] = L"OverlayWindowClass";

    // Register window class
    WNDCLASSEXW wcex = { 0 };
    wcex.cbSize = sizeof(wcex);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = ::LoadCursorW(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)::GetStockObject(BLACK_BRUSH);
    wcex.lpszClassName = k_WndClassName;
    ::RegisterClassExW(&wcex);

    HWND hWnd = ::CreateWindowExW(WS_EX_TOPMOST | WS_EX_LAYERED,
                                    k_WndClassName,
                                    L"Overlay Window",
                                    WS_POPUP | WS_VISIBLE,
                                    CW_USEDEFAULT, CW_USEDEFAULT,
                                    800, 600,
                                    NULL, NULL,
                                    hInstance,
                                    NULL);
    // Semi-transparent window. Mask out background color
    ::SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 128, LWA_ALPHA | LWA_COLORKEY);


    ::ShowWindow(hWnd, nCmdShow);
    ::UpdateWindow(hWnd);

    // Main message loop:
    MSG msg = { 0 };
    while (::GetMessageW(&msg, NULL, 0, 0) > 0)
    {
        std::cout << "HELLO Friend" << "\n";
        ::TranslateMessage(&msg);
        ::DispatchMessageW(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps = { 0 };
            HDC hDC = ::BeginPaint(hWnd, &ps);
            RECT rc = { 0 };
            ::GetClientRect(hWnd, &rc);
            ::SetTextColor(hDC, RGB(255, 255, 255));
            ::SetBkMode(hDC, TRANSPARENT);
            ::DrawTextExW(hDC, L"Over Here!", -1, &rc,
                DT_SINGLELINE | DT_CENTER | DT_VCENTER, NULL);
            ::EndPaint(hWnd, &ps);
        }
        return 0;

        case WM_NCHITTEST:
            return HTCAPTION;

        case WM_DESTROY:
            ::PostQuitMessage(0);
            return 0;

        default:
            break;
    }
    return ::DefWindowProc(hWnd, message, wParam, lParam);
}
