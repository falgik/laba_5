// laba_5.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "laba_5.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LABA5, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LABA5));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LABA5));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LABA5);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}



BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    HWND hWnd = CreateWindowW(szWindowClass, L"Лаба#5", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 465, 220, nullptr, nullptr, hInstance, nullptr);



    CreateWindow(L"button", L"гіфка", WS_VISIBLE | WS_CHILD, 50, 60, 150, 50, hWnd, (HMENU)89, NULL, NULL);
    CreateWindow(L"button", L"калькулятор", WS_VISIBLE | WS_CHILD, 250, 60, 150, 50, hWnd, (HMENU)98, NULL, NULL);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}


TCHAR progName1[256] = _T("Aero.exe");
TCHAR progName2[256] = _T("laba_4");

STARTUPINFO startup_info;
PROCESS_INFORMATION  proc_info;
PROCESS_INFORMATION  calc_info[4];

int laba_tm, calc_tm[4];

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

    case WM_CREATE:
    {
        startup_info.cb = sizeof(STARTUPINFO);
        startup_info.dwFlags = STARTF_USESHOWWINDOW;
        startup_info.wShowWindow = SW_SHOWNORMAL;
        SetTimer(hWnd, 1, 1000, nullptr);
    }
    break;

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case 98:
        {
            for (int i = 0; i < 4; i++)
            {
                DWORD exitCode;
                if (GetExitCodeProcess(calc_info[i].hProcess, &exitCode))
                {
                    if (exitCode != STILL_ACTIVE)
                    {
                        CreateProcess(NULL, progName1, NULL, NULL, FALSE, 0, NULL, NULL, &startup_info, &calc_info[i]);
                        calc_tm[i] = 20;
                        break;
                    }
                }
                else
                {
                    CreateProcess(NULL, progName1, NULL, NULL, FALSE, 0, NULL, NULL, &startup_info, &calc_info[i]);
                    calc_tm[i] = 20;
                    break;
                }
            }
        }
        break;

        case 89:
        {
            DWORD exitCode;

            if (GetExitCodeProcess(proc_info.hProcess, &exitCode))
            {
                if (exitCode != STILL_ACTIVE)
                {
                    CreateProcess(NULL, progName2, NULL, NULL, FALSE, 0, NULL, NULL, &startup_info, &proc_info);
                }
                laba_tm = 10;
            }
            else
            {
                CreateProcess(NULL, progName2, NULL, NULL, FALSE, 0, NULL, NULL, &startup_info, &proc_info);
                laba_tm = 10;
                break;
            }
        }
        break;

        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
    {
        PostQuitMessage(0);
    }
    break;

    case WM_TIMER:
    {
        if (laba_tm > 0)
            laba_tm--;

        for (int i = 0; i < 4; i++)
            if (calc_tm[i] > 0)
                calc_tm[i]--;

        if (!laba_tm)
        {
            DWORD exitCode;

            if (GetExitCodeProcess(proc_info.hProcess, &exitCode))
            {
                if (exitCode == STILL_ACTIVE)
                {
                    TerminateProcess(proc_info.hProcess, 0);
                    break;
                }
            }
        }


        for (int i = 0; i < 4; i++)
        {
            if (!calc_tm[i])
            {
                DWORD exitCode;

                if (GetExitCodeProcess(calc_info[i].hProcess, &exitCode))
                    if (exitCode == STILL_ACTIVE)
                        TerminateProcess(calc_info[i].hProcess, 0);
            }
        }

    }
    break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

