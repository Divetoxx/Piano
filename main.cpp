#include <windows.h>
#include <gdiplus.h>
#include <mmsystem.h>
#include <vector>
#include <string>
#include <algorithm>
#include <shlwapi.h>

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

const int FRAME_WIDTH = 695;
const int FRAME_HEIGHT = 782;
const unsigned int TOTAL_FRAMES = 36;

const BYTE IQ_COLOR_R = 43;
const BYTE IQ_COLOR_G = 150;
const BYTE IQ_COLOR_B = 181;

HINSTANCE hInst = nullptr;
HWND hWindow = nullptr;
HBRUSH hBru = nullptr;
HBITMAP hBigSprite = nullptr;
unsigned int splCounter = 0;
unsigned int splIq = 0;
bool finalScoreDisplay = false;

const wchar_t* SZ_CLASS = L"otireuhfjdhfeilk";
const wchar_t* SZ_TITLE = L"Тест Виктора Серебрякова";

struct ValidHit {
    int id;
    std::vector<unsigned int> frames;
};

const std::vector<ValidHit> VALID_HITS = {
    {7001, {24, 25, 29}},
    {7002, {4, 5, 11, 35}},
    {7003, {1, 15, 18, 32}},
    {7004, {7, 14, 22}},
    {7005, {0, 2, 8, 12, 30, 33, 34}},
    {7006, {17, 28, 31}},
    {7007, {9, 10, 19, 20, 23, 26, 27}},
    {7008, {3, 6, 13, 16, 21}}
};

HBITMAP LoadPngFromDisk(const wchar_t* filename) {
    Gdiplus::GpBitmap* gpBitmap = nullptr;
    
    // Загружаем файл с диска через GDI+
    Gdiplus::DllExports::GdipCreateBitmapFromFile(filename, &gpBitmap);

    HBITMAP hBitmap = nullptr;
    if (gpBitmap) {
        // Конвертируем в стандартный HBITMAP для BitBlt
        Gdiplus::DllExports::GdipCreateHBITMAPFromBitmap(gpBitmap, &hBitmap, 0);
        Gdiplus::DllExports::GdipDisposeImage((Gdiplus::GpImage*)gpBitmap);
    }
    return hBitmap;
}


void DrawFinalScoreGdiplus(HWND hwnd) {
    int score = (int)(splIq * 2) + 89;
    if (score > 162) score = 162;

    std::wstring scoreText = std::to_wstring(score);
    std::wstring labelText = L"IQ:";
    HDC hdc = GetDC(hwnd);

    Gdiplus::GpGraphics* graphicsPtr = nullptr;
    Gdiplus::DllExports::GdipCreateFromHDC(hdc, &graphicsPtr);
    
    // Включаем сглаживание шрифтов, чтобы огромные цифры были идеально гладкими
    Gdiplus::DllExports::GdipSetTextRenderingHint(graphicsPtr, Gdiplus::TextRenderingHintAntiAlias);

    // Сборка цвета ARGB (заданный вами сине-зеленый цвет)
    ULONG colorArgb = ((ULONG)255 << 24) | ((ULONG)IQ_COLOR_R << 16) | ((ULONG)IQ_COLOR_G << 8) | (ULONG)IQ_COLOR_B;
    
    Gdiplus::GpSolidFill* brushPtr = nullptr;
    Gdiplus::DllExports::GdipCreateSolidFill(colorArgb, &brushPtr);

    Gdiplus::GpFontFamily* fontFamilyPtr = nullptr;
    Gdiplus::DllExports::GdipCreateFontFamilyFromName(L"Arial", nullptr, &fontFamilyPtr);

    // Создаем формат текста для выравнивания по центру
    Gdiplus::GpStringFormat* stringFormatPtr = nullptr;
    Gdiplus::DllExports::GdipCreateStringFormat(0, 0, &stringFormatPtr);
    Gdiplus::DllExports::GdipSetStringFormatAlign(stringFormatPtr, Gdiplus::StringAlignmentCenter);

    // 1. Рисуем надпись "Ваш IQ:" (Размер 40, жирный)
    Gdiplus::GpFont* labelFontPtr = nullptr;
    Gdiplus::DllExports::GdipCreateFont(fontFamilyPtr, 40.0f, 1, Gdiplus::UnitPixel, &labelFontPtr);
    
    // Центрируем в верхней половине экрана (Ширина окна ~710, поэтому X=0, Width=710 идеально центрирует)
    Gdiplus::RectF labelRect = { 0.0f, 220.0f, 710.0f, 60.0f };
    Gdiplus::DllExports::GdipDrawString(
        graphicsPtr, labelText.c_str(), -1, labelFontPtr, 
        &labelRect, stringFormatPtr, (Gdiplus::GpBrush*)brushPtr
    );

    // 2. Рисуем ОГРОМНЫЕ цифры результата (Размер 180, жирный)
    Gdiplus::GpFont* scoreFontPtr = nullptr;
    Gdiplus::DllExports::GdipCreateFont(fontFamilyPtr, 180.0f, 1, Gdiplus::UnitPixel, &scoreFontPtr);
    
    // Размещаем чуть ниже надписи
    Gdiplus::RectF scoreRect = { 0.0f, 290.0f, 710.0f, 220.0f };
    Gdiplus::DllExports::GdipDrawString(
        graphicsPtr, scoreText.c_str(), -1, scoreFontPtr, 
        &scoreRect, stringFormatPtr, (Gdiplus::GpBrush*)brushPtr
    );

    // Освобождение всех созданных ресурсов GDI+
    Gdiplus::DllExports::GdipDeleteStringFormat(stringFormatPtr);
    Gdiplus::DllExports::GdipDeleteGraphics(graphicsPtr);
    Gdiplus::DllExports::GdipDeleteBrush((Gdiplus::GpBrush*)brushPtr);
    Gdiplus::DllExports::GdipDeleteFont(labelFontPtr);
    Gdiplus::DllExports::GdipDeleteFont(scoreFontPtr);
    Gdiplus::DllExports::GdipDeleteFontFamily(fontFamilyPtr);

    ReleaseDC(hwnd, hdc);
}


void ProcessStep(HWND hwnd, int id) {
    if (splCounter >= TOTAL_FRAMES) return;
    unsigned int currentCounterValue = splCounter;

    for (const auto& hit : VALID_HITS) {
        if (id == hit.id) {
            auto it = std::find(hit.frames.begin(), hit.frames.end(), currentCounterValue);
            if (it != hit.frames.end()) {
                splIq += 1;
                break;
            }
        }
    }

    splCounter += 1;

    // 2. Исправлено: условие теперь строго до 36 (TOTAL_FRAMES)
    if (splCounter < TOTAL_FRAMES) {
        std::wstring titleText = L"Задание " + std::to_wstring(splCounter + 1) + L" из 36";
        SetWindowTextW(hwnd, titleText.c_str());
        InvalidateRect(hwnd, nullptr, FALSE);
    } else {
        // Сработает только ПОСЛЕ того, как пользователь кликнул на 36-м задании
        finalScoreDisplay = true;
        SetWindowTextW(hwnd, L"Результат теста");
        InvalidateRect(hwnd, nullptr, FALSE);
    }
}



LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CTLCOLORSTATIC: {
            HDC hdc = (HDC)wParam;
            SetBkMode(hdc, TRANSPARENT);
            return (LRESULT)GetStockObject(HOLLOW_BRUSH);
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            if (finalScoreDisplay) {
                // Если тест окончен, просто заливаем экран белым цветом
                RECT rect;
                GetClientRect(hwnd, &rect);
                FillRect(hdc, &rect, hBru);
                EndPaint(hwnd, &ps);

                // Рисуем крупные цифры результата поверх белого экрана
                DrawFinalScoreGdiplus(hwnd);
            } else {
                // Если тест еще идет, рисуем текущее задание из спрайта
                HDC memDc = CreateCompatibleDC(hdc);
                HGDIOBJ oldObj = SelectObject(memDc, hBigSprite);
                
                int currentY = (int)splCounter * FRAME_HEIGHT;
                BitBlt(hdc, 0, 0, FRAME_WIDTH, FRAME_HEIGHT, memDc, 0, currentY, SRCCOPY);
                
                SelectObject(memDc, oldObj);
                DeleteDC(memDc);
                EndPaint(hwnd, &ps);
            }
            return 0;
        }

        case WM_COMMAND: {
            int id = (int)(wParam & 0xFFFF);
            if (id >= 7001 && id <= 7008) {
                ProcessStep(hwnd, id);
            }
            return 0;
        }
        case WM_DESTROY: {
            if (hBigSprite) DeleteObject(hBigSprite);
            if (hBru) DeleteObject(hBru);
            PostQuitMessage(0);
            return 0;
        }
        default:
            return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance;

    ULONG_PTR gpToken;
    Gdiplus::GdiplusStartupInput gpInput;
    Gdiplus::GdiplusStartup(&gpToken, &gpInput, nullptr);

    hBigSprite = LoadPngFromDisk(L"Victor.png");
    if (!hBigSprite) {
        MessageBoxW(nullptr, L"Файл Victor.png не найден в папке с программой!", L"Ошибка", MB_OK | MB_ICONERROR);
        return 0;
    }

    hBru = CreateSolidBrush(RGB(255, 255, 255));

    WNDCLASSW wc = {};
    wc.hInstance = hInst;
    wc.lpszClassName = SZ_CLASS;
    wc.lpfnWndProc = WndProc;
    wc.hbrBackground = hBru;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIconW(hInst, MAKEINTRESOURCEW(1));

    RegisterClassW(&wc);

    hWindow = CreateWindowExW(
        0, SZ_CLASS, L"Задание 1 из 36",
        WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 710, 820,
        nullptr, nullptr, hInst, nullptr
    );

    struct Coord { int x, y, id; };
    Coord coords[] = {
        {225, 15, 7001}, {460, 30, 7002}, {520, 250, 7003}, {495, 500, 7004},
        {300, 600, 7005}, {65, 610, 7006}, {0, 350, 7007}, {0, 100, 7008}
    };

    for (const auto& c : coords) {
        CreateWindowExW(
            0, L"STATIC", nullptr,
            SS_NOTIFY | WS_CHILD | WS_VISIBLE,
            c.x, c.y, 170, 170, hWindow, (HMENU)(INT_PTR)c.id, hInst, nullptr
        );
    }

    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    Gdiplus::GdiplusShutdown(gpToken);

    return (int)msg.wParam;
}
