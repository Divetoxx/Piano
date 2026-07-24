#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <string>
#include <cmath>

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

const unsigned int DISCR = 48000;
const float AMPLITUDE = 0.35f; 
const float PI = 3.1415926535f;

const float HZS[] = {
    131.0f, 139.0f, 147.0f, 156.0f, 165.0f, 175.0f, 185.0f, 196.0f, 208.0f, 220.0f, 233.0f, 247.0f,
    262.0f, 277.0f, 294.0f, 311.0f, 330.0f, 349.0f, 370.0f, 392.0f, 415.0f, 440.0f, 466.0f, 494.0f,
    523.0f, 554.0f, 587.0f, 622.0f, 659.0f, 698.0f, 740.0f, 784.0f, 831.0f, 880.0f, 932.0f, 988.0f,
    1047.0f, 1109.0f, 1175.0f, 1245.0f, 1319.0f, 1397.0f, 1480.0f, 1568.0f, 1661.0f, 1760.0f, 1865.0f, 1976.0f,
    2093.0f, 2217.0f, 2349.0f, 2489.0f, 2637.0f, 2794.0f, 2960.0f, 3136.0f, 3322.0f, 3520.0f, 3729.0f, 3951.0f,
    4186.0f, 4435.0f, 4699.0f, 4978.0f, 5274.0f, 5588.0f, 5920.0f
};

struct MelodyInfo {
    std::wstring name;
    const std::vector<BYTE>* data;
};

std::vector<std::vector<float>> precomputedSounds;
std::vector<MelodyInfo> melodies;

size_t selectedIdx = 0;
size_t currentNoteIdx = 0;

const int ID_COMBOBOX = 5001;
const int ID_PLAY_BTN = 5002;
const int ID_RESET_BTN = 5003;

struct ActiveVoice {
    int soundIdx = -1;
    size_t sampleOffset = 0;
};
const int MAX_VOICES = 1;
ActiveVoice activeVoices[MAX_VOICES];

HWAVEOUT hWaveOut = nullptr;
const int BUFFER_SAMPLES = 2048;
short audioBuffer1[BUFFER_SAMPLES * 2];
short audioBuffer2[BUFFER_SAMPLES * 2];
WAVEHDR waveHdr1, waveHdr2;
MMRESULT hTimer = 0;

std::vector<float> GenerateNoteData(BYTE midiNote) {
    std::vector<float> samples;
    int numSamples = 24000;
    samples.reserve(numSamples);
    
    int baseIdx = (int)midiNote - 40;
    if (baseIdx < 0) baseIdx = 0;

    float f1 = (baseIdx < 67) ? HZS[baseIdx] : 440.0f;
    float f2 = (baseIdx + 12 < 67) ? HZS[baseIdx + 12] : f1 * 2.0f;
    float f3 = (baseIdx + 28 < 67) ? HZS[baseIdx + 28] : f2 * 2.0f;
    float f4 = (baseIdx + 34 < 67) ? HZS[baseIdx + 34] : f1 * 3.0f;
    float f_sub = f1 / 2.0f;
    
    if (baseIdx >= 12 && (baseIdx - 12) < 67) {
        f_sub = HZS[baseIdx - 12];
    }

    for (int n = 0; n < numSamples; ++n) {
        float angle = 2.0f * PI * (float)n / (float)DISCR;
        float envelope = (cosf(angle) + 1.0f) / 2.0f;
        
        float s = (sinf(angle * f_sub) * 0.6f + 
                   sinf(angle * f1) + 
                   sinf(angle * f2) * 0.7f + 
                   sinf(angle * f3) * 0.5f + 
                   sinf(angle * f4) * 0.4f) * envelope * AMPLITUDE;
        
        samples.push_back(s);
    }
    return samples;
}


void MixAudio(short* outputBuffer, int numSamples) {
    for (int i = 0; i < numSamples; ++i) {
        float mixedSample = 0.0f;

        for (int v = 0; v < MAX_VOICES; ++v) {
            if (activeVoices[v].soundIdx != -1) {
                const auto& soundData = precomputedSounds[activeVoices[v].soundIdx];
                size_t offset = activeVoices[v].sampleOffset;

                if (offset < soundData.size()) {
                    mixedSample += soundData[offset];
                    activeVoices[v].sampleOffset++;
                } else {
                    activeVoices[v].soundIdx = -1;
                }
            }
        }

        if (mixedSample > 1.0f) mixedSample = 1.0f;
        if (mixedSample < -1.0f) mixedSample = -1.0f;

        short pcm = (short)(mixedSample * 32767.0f);
        outputBuffer[i * 2] = pcm;     
        outputBuffer[i * 2 + 1] = pcm; 
    }
}

void AddNoteToMixer(int soundIdx) {
    // Шаг 1: Ищем абсолютно свободный слот
    for (int v = 0; v < MAX_VOICES; ++v) {
        if (activeVoices[v].soundIdx == -1) {
            activeVoices[v].soundIdx = soundIdx;
            activeVoices[v].sampleOffset = 0;
            return;
        }
    }

    // Шаг 2: Если все 16 слотов заняты, ищем ноту, которая почти доиграла до конца
    int oldestVoiceIdx = 0;
    size_t maxOffset = 0;

    for (int v = 0; v < MAX_VOICES; ++v) {
        if (activeVoices[v].sampleOffset > maxOffset) {
            maxOffset = activeVoices[v].sampleOffset;
            oldestVoiceIdx = v;
        }
    }

    // Заменяем самую старую ноту, чтобы переход был максимально незаметным
    activeVoices[oldestVoiceIdx].soundIdx = soundIdx;
    activeVoices[oldestVoiceIdx].sampleOffset = 0;
}

void PlayNext() {
    if (melodies[selectedIdx].data == nullptr) return;
    const std::vector<BYTE>& melody = *(melodies[selectedIdx].data);
    if (melody.empty()) return;

    if (currentNoteIdx < melody.size()) {
        BYTE note = melody[currentNoteIdx];
        int soundIdx = (int)note - 40;
        if (soundIdx >= 0 && soundIdx < (int)precomputedSounds.size()) {
            AddNoteToMixer(soundIdx);
            currentNoteIdx++;
        }
    } else {
        currentNoteIdx = 0;
    }
}

// Новый безопасный таймер: проверяет буферы каждые 10 миллисекунд и дозаливает звук
void CALLBACK TimerCallback(UINT uID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2) {
    if (!hWaveOut) return;

    if (waveHdr1.dwFlags & WHDR_DONE) {
        MixAudio((short*)waveHdr1.lpData, BUFFER_SAMPLES);
        waveOutWrite(hWaveOut, &waveHdr1, sizeof(WAVEHDR));
    }
    if (waveHdr2.dwFlags & WHDR_DONE) {
        MixAudio((short*)waveHdr2.lpData, BUFFER_SAMPLES);
        waveOutWrite(hWaveOut, &waveHdr2, sizeof(WAVEHDR));
    }
}

void RenderUI(HWND hwnd, HDC hdc) {
    Gdiplus::Graphics graphics(hdc);
    graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
    Gdiplus::SolidBrush textBrush(Gdiplus::Color(255, 255, 0, 0));
    Gdiplus::SolidBrush darkBrush(Gdiplus::Color(255, 0, 0, 0));
    Gdiplus::FontFamily fontFamily(L"Arial");
    Gdiplus::StringFormat centerFormat;
    centerFormat.SetAlignment(Gdiplus::StringAlignmentCenter);

    Gdiplus::Font titleFont(&fontFamily, 46.0f, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
    Gdiplus::RectF titleRect(0.0f, 20.0f, 700.0f, 60.0f);
    graphics.DrawString(L"Piano", -1, &titleFont, titleRect, &centerFormat, &textBrush);

    Gdiplus::Font subFont(&fontFamily, 26.0f, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
    Gdiplus::RectF subRect(0.0f, 100.0f, 700.0f, 40.0f);
    graphics.DrawString(L"Выберите мелодию:", -1, &subFont, subRect, &centerFormat, &darkBrush);

    Gdiplus::Font hintFont(&fontFamily, 24.0f, Gdiplus::FontStyleItalic, Gdiplus::UnitPixel);
    Gdiplus::RectF hintRect(0.0f, 650.0f, 700.0f, 40.0f);
    graphics.DrawString(L"Подсказка: жмите ПРОБЕЛ", -1, &hintFont, hintRect, &centerFormat, &darkBrush);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            HWND hCombo = CreateWindowW(L"COMBOBOX", nullptr, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL,
                100, 160, 500, 700, hwnd, (HMENU)ID_COMBOBOX, GetModuleHandle(nullptr), nullptr);
            HFONT hFont = CreateFontW(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, RUSSIAN_CHARSET, 0, 0, 0, 0, L"Arial");
            SendMessageW(hCombo, WM_SETFONT, (WPARAM)hFont, TRUE);
            for (const auto& m : melodies) SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)m.name.c_str());
            SendMessageW(hCombo, CB_SETCURSEL, 0, 0);

            HWND hBtnPlay = CreateWindowW(L"BUTTON", L"ИГРАТЬ НОТУ", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                100, 240, 500, 150, hwnd, (HMENU)ID_PLAY_BTN, GetModuleHandle(nullptr), nullptr);
            HFONT hBtnFont = CreateFontW(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, RUSSIAN_CHARSET, 0, 0, 0, 0, L"Arial");
            SendMessageW(hBtnPlay, WM_SETFONT, (WPARAM)hBtnFont, TRUE);

            HWND hBtnReset = CreateWindowW(L"BUTTON", L"Сброс", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                250, 430, 200, 50, hwnd, (HMENU)ID_RESET_BTN, GetModuleHandle(nullptr), nullptr);
            HFONT hBtnResetFont = CreateFontW(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, RUSSIAN_CHARSET, 0, 0, 0, 0, L"Arial");
            SendMessageW(hBtnReset, WM_SETFONT, (WPARAM)hBtnResetFont, TRUE);
            break;
        }
        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            int wmEvent = HIWORD(wParam);
            if (wmId == ID_COMBOBOX && wmEvent == CBN_SELCHANGE) {
                selectedIdx = SendMessageW((HWND)lParam, CB_GETCURSEL, 0, 0);
                currentNoteIdx = 0;
                SetFocus(hwnd);
            }
            if (wmId == ID_PLAY_BTN) { PlayNext(); SetFocus(hwnd); }
            if (wmId == ID_RESET_BTN) { currentNoteIdx = 0; SetFocus(hwnd); }
            break;
        }
        case WM_KEYDOWN: {
            if (wParam == VK_SPACE) PlayNext();
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            HBRUSH hBgBrush = CreateSolidBrush(RGB(255, 255, 255));
            FillRect(hdc, &clientRect, hBgBrush);
            DeleteObject(hBgBrush);
            RenderUI(hwnd, hdc);
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY: {
            // Исправлено: Гарантированное, безопасное уничтожение всех аудио-служб Windows
            if (hTimer) {
                timeKillEvent(hTimer);
                hTimer = 0;
            }
            if (hWaveOut) {
                waveOutReset(hWaveOut);
                waveOutUnprepareHeader(hWaveOut, &waveHdr1, sizeof(WAVEHDR));
                waveOutUnprepareHeader(hWaveOut, &waveHdr2, sizeof(WAVEHDR));
                waveOutClose(hWaveOut);
                hWaveOut = nullptr;
            }
            PostQuitMessage(0);
            return 0;
        }
        default:
            return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
    return 0;
}

const std::vector<BYTE> MELODY1 = {79,80,82,82,82,82,82,84,82,79,80,82,82,82,82,82,84,82,87,86,87,86,84,77,77,80,82,84,84,84,84,84,86,84,80,82,84,84,84,84,84,86,84,89,87,86,84,81,82,79,79,80,82,82,82,82,82,84,82,79,80,82,82,82,82,82,84,82,91,89,87,86,83,84,84,92,89,84,89,84,89,84,89,91,87,83,84,83,84,83,84,87,84,80,79,77,82,79,82,87,87,87,87,87,87,87,77,82,87,87,87,87,87,87,87,89,91,92,84,86,91,87};
const std::vector<BYTE> MELODY2 = {76,75,76,77,76,72,72,71,69,72,76,76,81,76,79,78,77,74,83,81,80,77,76,74,76,74,72,69,71,72,71,69,71,74,77,76,74,72,71,72,69,72,76,81,84,81,83,84,83,81,83,86,89,88,86,84,83,81,80,81,83,80,83,81,68,71,76,74,72,77,76,74,72,71,69,68,64,68,71,76,74,72,69,77,76,74,72,71,69,68,52,59,57,56,59,62,60,59,62,65,64,62,68,71,69,68,71,74,72,71,74,77,76,76,76,76,76,76,76,76,76,76,75,76,77,76,72,72,71,69,72,76,76,81,76,79,78,77,74,83,81,80,77,76,74,76,74,72,69,71,72,71,69,71,74,77,76,74,72,71,72,69,72,76,81,84,81,83,84,83,81,83,86,89,88,86,84,83,81,80,81,83,80,83,81,69,64,66,68,69,71,72,69,71,64,68,69,71,72,74,71,72,67,69,71,72,74,76,72,74,67,71,72,74,76,77,74,76,75,76,77,76,74,72,72,71,69,72,71,71,76,64,68,69,68,69,71,72,71,72,74,76,72,71,71,76,64,68,69,68,72,71,71,69};
const std::vector<BYTE> MELODY3 = {52,60,59,57,56,57,52,64,62,60,59,60,57,65,64,62,60,59,60,62,65,64,62,60,62,59,52,60,59,57,56,57,52,64,62,60,59,60,57,65,64,62,60,59,60,62,65,64,62,60,62,59,64,64,55,55,55,55,53,52,53,53,62,62,53,53,53,53,52,50,52,52,47,50,53,57,62,60,59,57,57,56,47,50,53,57,62,60,59,57,57,56,57};
const std::vector<BYTE> MELODY4 = {63,64,62,60,59,57,68,69,67,65,65,61,62,60,59,57,55,66,67,65,64,64,63,64,62,60,59,57,68,69,67,65,65,64,62,64,69,57,59,64,52,57};
const std::vector<BYTE> MELODY5 = {72,69,64,64,69,72,71,69,71,68,64,69,70,69,67,64,62,61,61,62,64,67,65,64,65,77,74,70,70,74,77,76,74,76,72,69,72,69,75,72,72,71,69,71,76,77,76,77,76,77,76,77,52,57,59,60,59,57,52,52,57,59,60,64,67,65,59,59,60,62,60,62,64,62,64,65,64,62,60,59,59,60,55,60,62,60,59,57,56};
const std::vector<BYTE> MELODY6 = {74,67,69,71,72,74,67,67,76,72,74,76,78,79,67,67,72,74,72,71,69,71,72,71,69,67,66,67,69,71,67,71,69,74,67,69,71,72,74,67,67,76,72,74,76,78,79,67,67,72,74,72,71,69,71,72,71,69,67,69,71,69,67,66,67};
const std::vector<BYTE> MELODY7 = {74,72,70,69,67,67,66,75,74,72,70,69,69,67,79,77,79,75,77,74,75,77,75,77,74,75,72,74,75,74,75,72,74,70,72,74,79,81,82,81,79,78,79,77,62,63,65,63,62,60,72,75,79,74,70,74,79,72,68,70,72,70,68,67,67,69,70,69,67,66,66,67,69,67,66,67};
const std::vector<BYTE> MELODY8 = {62,63,64,72,64,72,64,72,72,74,75,76,72,74,76,71,74,72,62,63,64,72,64,72,64,72,69,67,66,69,72,76,74,72,69,74,62,63,64,72,64,72,64,72,72,74,75,76,72,74,76,71,74,72,72,74,76,72,74,76,72,74,72,76,72,74,76,72,74,72,76,72,74,76,71,74,72};
const std::vector<BYTE> MELODY9 = {60,69,69,67,69,65,60,60,60,69,69,70,67,72,72,62,62,70,70,69,67,65,60,69,69,67,69,65,72,62,62,70,70,69,67,65,60,69,69,67,69,65};
const std::vector<BYTE> MELODY10 = {81,80,81,83,81,69,73,76,76,74,74,74,73,74,76,74,64,71,74,74,73,73,81,78,76,75,75,75,81,78,76,75,75,75,81,78,80,76,73,81,80,78,76,78,76,81,80,81,83,81,69,73,76,76,74,74,74,73,74,76,74,64,71,74,74,73,73,81,78,76,75,75,75,81,78,76,75,75,75,81,78,80,76,73,81,80,78,76,78,76,84,76,83,76,76,76,81,76,80,76,76,76,84,76,83,76,76,76,81,76,80,76,76,76,81,80,81,83,81,69,73,76,76,74,74,74,73,74,76,74,64,71,74,74,73,73,74,71,69,68,68,68,74,71,69,68,68,68,74,71,73,69,66,74,73,71,69,71,69,84,76,83,76,76,76,81,76,80,76,76,76,84,76,83,76,76,76,81,76,80,76,76,76,81,80,81,83,81,69,73,76,76,74,74,74,73,74,76,74,64,71,74,74,73,73,74,71,69,68,68,68,74,71,69,68,68,68,74,71,73,69,66,74,73,71,69,71,69};
const std::vector<BYTE> MELODY11 = {74,74,71,71,74,74,69,69,71,72,74,76,78,74,74,74,71,71,74,74,69,69,81,80,81,83,76,81,74,83,83,81,79,79,78,78,79,81,78,76,74,79,79,79,76,76,79,79,74,74,74,76,79,74,81,79};
const std::vector<BYTE> MELODY12 = {83,83,83,83,83,83,83,86,79,81,83,84,84,84,84,84,83,83,83,83,81,81,83,81,86,83,83,83,83,83,83,83,86,79,81,83,84,84,84,84,84,83,83,83,86,86,84,81,79};
const std::vector<BYTE> MELODY13 = {65,77,75,75,73,73,72,72,70,70,72,65,65,77,75,75,73,73,72,72,70,70,72,72,65,70,82,80,80,78,78,77,77,68,80,78,78,77,77,75,73,75,72,69,70,77,89,87,87,85,85,84,84,82,82,84,77,77,89,87,87,85,85,84,84,82,82,84,84,77,82,94,92,92,90,90,89,89,80,92,90,90,89,89,87,85,87,84,81,82,49,53,58,60,61,61,49,53,58,60,61,61,54,54,58,63,65,66,66,54,58,63,65,66,66,68,65,77,75,75,73,73,72,72,70,70,72,65,65,77,75,75,73,73,72,72,70,70,72,72,65,70,82,80,80,78,78,77,77,68,80,78,78,77,77,75,73,75,72,69,70,77,89,87,87,85,85,84,84,82,82,84,77,77,89,87,87,85,85,84,84,82,82,84,84,77,82,94,92,92,90,90,89,89,80,92,90,90,89,89,87,85,87,84,81,82};
const std::vector<BYTE> MELODY14 = {71,69,68,69,72,74,72,71,72,76,77,76,75,76,83,81,80,81,83,81,80,81,84,81,84,83,81,79,81,83,81,79,81,83,81,79,78,76,71,69,68,69,72,74,72,71,72,76,77,76,75,76,83,81,80,81,83,81,80,81,84,81,84,83,81,79,81,83,81,79,81,83,81,79,78,76,76,77,79,79,81,79,77,76,74,67,76,77,79,79,81,79,77,76,74,72,74,76,76,77,76,74,72,71,64,72,74,76,76,77,76,74,72,71,71,69,68,69,72,74,72,71,72,76,77,76,75,76,83,81,80,81,83,81,80,81,84,81,83,84,83,81,80,81,76,77,74,72,71,69,69,71,73,69,71,73,71,69,68,66,68,69,71,68,64,69,71,73,69,71,73,71,69,68,66,71,68,64,69,85,86,85,83,81,83,81,80,78,81,80,78,77,78,80,77,73,75,77,73,78,77,78,80,81,80,81,83,85,84,85,84,85,86,85,83,81,83,81,80,78,81,80,78,76,78,80,76,73,75,76,73,75,76,78,75,72,73,75,72,73,85,86,85,83,81,83,81,80,78,81,80,78,77,78,80,77,73,75,77,73,78,77,78,80,81,80,81,83,85,84,85,84,85,86,85,83,81,83,81,80,78,81,80,78,76,78,80,76,73,75,76,73,75,76,78,75,72,73,75,72,73,76,74,73,71,69,71,73,74,76,78,80,81,81,80,78,76,76,74,73,71,69,71,73,74,76,78,80,81,82,83,76,74,73,71,69,71,73,74,76,78,80,81,81,80,78,76,76,74,73,71,73,76,69,73,71,74,68,71,69};
const std::vector<BYTE> MELODY15 = {62,62,66,63,62,66,66,69,67,66,67,67,70,69,67,66,63,66,63,66,62,62,66,63,62,66,66,69,67,66,67,67,70,69,67,66,63,66,63,62,66,66,63,62,62,62,63,63,62,60,60,60,60,63,62,60,60,67,66,63,66,63,62,66,66,63,62,62,62,63,63,62,60,60,60,60,63,62,60,60,67,66,63,66,63,62,67,67,67,67,67,67,67,67,70,69,67,70,69,67,67,67,70,69,67,70,69,67,69,69,72,70,69,72,70,69,69,69,72,70,69,72,70,69,69,69,74,69,69,74,62,62,74,72,70,69,67};
const std::vector<BYTE> MELODY16 = {72,64,64,71,69,71,63,63,71,62,62,69,68,69,61,61,69,60,60,69,68,69,59,59,59,62,64,65,65,64,62,62,60,60,64,62,60,59,57,60,59,72,64,64,71,69,71,63,63,71,62,62,69,68,69,61,61,69,60,60,69,68,69,59,59,59,62,64,65,64,62,64,65,69,68,69,71,64,72};
const std::vector<BYTE> MELODY17 = {69,74,76,77,79,81,81,82,81,82,86,81,79,79,76,77,77,76,74,76,77,74,69,74,76,77,79,81,81,82,81,82,86,81,79,79,76,77,77,76,74,76,77,74,74,86,86,86,84,86,84,82,81,74,86,86,86,84,86,84,82,81,79,79,76,77,77,79,81,82,84,81,79,77,79,79,77,77,77,76,74,76,77,74,79,79,76,77,77,79,81,82,84,81,79,77,79,79,77,77,77,76,74,76,77,74};
const std::vector<BYTE> MELODY18 = {69,70,69,67,69,72,70,69,67,67,65,69,67,65,65,67,69,65,67,69,70,69,67,69,74,72,74,72,72,70,69,70,70,72,70,69,70,74,72,70,69,69,72,70,66,67,67,69,70,67,74,72,74,72,70,70,69,68,68,69,69,74,76,74,73,76,70,68,69,77,76,74,76,74,73,76,70,72,72,74,72,74,72,71,72,71,72,74,76,77,79,81,81,81,81,82,81,79,77,79,77,76,74,76,74,72,70,72,70,69,67,74,72,71,72,76,77,72,69,69,67,66,67,71,72,70,67,65};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    melodies = {
        { L"Вальс-шутка, Дмитрий Шостакович", &MELODY1 },
        { L"Полонез 13, Михаил Огинский", &MELODY2 },
        { L"Город Золотой, Владимир Вавилов", &MELODY3 },
        { L"Шербурские зонтики, Мишеля Леграна", &MELODY4 },
        { L"Бабье лето, Джо Дассен", &MELODY5 },
        { L"Менуэт, Иоганн Себастьян Бах", &MELODY6 },
        { L"Адажио, Ремо Джадзотто", &MELODY7 },
        { L"Артист эстрады, Скотт Джоплин", &MELODY8 },
        { L"В лесу родилась елочка, Леонид Бекман", &MELODY9 },
        { L"Менуэт, Луиджи Боккерини", &MELODY10 },
        { L"Америка прекрасна, Сэмюэл Уорд", &MELODY11 },
        { L"Джингл Белс, Джеймс Лорд Пирпонт", &MELODY12 },
        { L"Тот самый Мюнхгаузен, Рыбников", &MELODY13 },
        { L"Турецкий марш, Моцарта", &MELODY14 },
        { L"Хава нагила, еврейская песня", &MELODY15 },
        { L"Где-то далеко, Микаэл Таривердиев", &MELODY16 },
        { L"Атиква, Джузеппе Ченчи", &MELODY17 },
        { L"Каста Дива, Винченцо Беллини", &MELODY18 }
};

precomputedSounds.reserve(60);
for (BYTE midiNote = 40; midiNote < 100; ++midiNote) {
precomputedSounds.push_back(GenerateNoteData(midiNote));
}

WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 2, DISCR, DISCR * 4, 4, 16, 0 };
waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
waveHdr1.lpData = (LPSTR)audioBuffer1; waveHdr1.dwBufferLength = BUFFER_SAMPLES * 4;
waveHdr2.lpData = (LPSTR)audioBuffer2; waveHdr2.dwBufferLength = BUFFER_SAMPLES * 4;
waveOutPrepareHeader(hWaveOut, &waveHdr1, sizeof(WAVEHDR));
waveOutPrepareHeader(hWaveOut, &waveHdr2, sizeof(WAVEHDR));

waveOutWrite(hWaveOut, &waveHdr1, sizeof(WAVEHDR));
waveOutWrite(hWaveOut, &waveHdr2, sizeof(WAVEHDR));

hTimer = timeSetEvent(10, 10, TimerCallback, 0, TIME_PERIODIC);

ULONG_PTR gpToken;
Gdiplus::GdiplusStartupInput gpInput;
Gdiplus::GdiplusStartup(&gpToken, &gpInput, nullptr);

WNDCLASSW wc = {};
wc.hInstance = hInstance;
wc.lpszClassName = L"PianoAppClass";
wc.lpfnWndProc = WndProc;
wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
wc.hIcon = LoadIconW(hInstance, MAKEINTRESOURCEW(1));
RegisterClassW(&wc);

RECT r = { 0, 0, 700, 800 };
AdjustWindowRect(&r, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);
HWND hwnd = CreateWindowExW(0, L"PianoAppClass", L"Piano",WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,CW_USEDEFAULT, CW_USEDEFAULT, r.right - r.left, r.bottom - r.top, nullptr, nullptr, hInstance, nullptr);

MSG msg = {};
while (GetMessageW(&msg, nullptr, 0, 0)) {
TranslateMessage(&msg);
DispatchMessageW(&msg);
}

Gdiplus::GdiplusShutdown(gpToken);
return (int)msg.wParam;
}
