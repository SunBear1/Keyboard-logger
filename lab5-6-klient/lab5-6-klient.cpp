#ifndef UNICODE
#define UNICODE
#endif
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define GWL_HINSTANCE

#include <winsock2.h>
#include <stdio.h>
#include <windows.h>

// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
using namespace std;

unsigned int total, textCount, bitmapCount, metafilepictCount, sylkCount, difCount, tiffCount, oemtextCount, dibCount, paletteCount;

int uFormat;

long FAR PASCAL WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static UINT auPriorityList[] = {
		CF_TEXT,
		CF_BITMAP,
		CF_METAFILEPICT,
		CF_SYLK,
		CF_DIF,
		CF_TIFF,
		CF_OEMTEXT,
		CF_DIB,
		CF_PALETTE,
		CF_HDROP
};


void send2Server(char* text)
{
	HWND hwnd = FindWindowExA(0, 0, "schowek_Receiver", 0);
	if (hwnd != 0)
	{
		COPYDATASTRUCT cd = {};
		cd.dwData = 1;
		cd.cbData = (DWORD)(strlen(text) + 1);
		cd.lpData = text;
		SendMessageA(hwnd, WM_COPYDATA, 0, (LPARAM)(&cd));
	}
}



int wmain(void)
{
	char t[] = "pr_lab5";
	SetConsoleTitleA(t);
	HWND hwndConsole = FindWindowA(NULL, t);

	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hwndConsole, GWL_HINSTANCE);

	WNDCLASS wc = { 0 };
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = TEXT("pr_lab5");

	RegisterClass(&wc);

	HWND hwndWindow = CreateWindowW(TEXT("pr_lab5"), TEXT("pr_lab5"), WS_MINIMIZE, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);

	ShowWindow(hwndWindow, SW_SHOWMINIMIZED);
	UpdateWindow(hwndWindow);

	MSG msg;
	while (GetMessage(&msg, hwndWindow, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

long FAR PASCAL WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	static HWND hwndNextViewer;
	HDC hdc;
	PAINTSTRUCT ps;
	HGLOBAL hglb;
	LPSTR text_from_clipboard;

	switch (uMsg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		switch (uFormat) {
		case CF_TEXT:
			while (!OpenClipboard(hwnd));
			hglb = GetClipboardData(uFormat);
			text_from_clipboard = (LPSTR)GlobalLock(hglb);

			SOCKET s;
			struct sockaddr_in sa;
			WSADATA wsas;
			WORD wersja;
			wersja = MAKEWORD(2, 0);
			WSAStartup(wersja, &wsas);
			s = socket(AF_INET, SOCK_STREAM, 0);
			memset((void*)(&sa), 0, sizeof(sa));
			sa.sin_family = AF_INET;
			sa.sin_port = htons(2000);
			sa.sin_addr.s_addr = inet_addr("127.0.0.1");

			if (connect(s, (struct sockaddr FAR*) & sa, sizeof(sa)) == SOCKET_ERROR)
			{
				printf("\nBlad polaczenia!");
				return 0;
			}

			int dlugosc_textu;
			char text_storage[2000];
			dlugosc_textu = strlen(text_from_clipboard);
			for (int i = 0; i <= dlugosc_textu; i++) {
				text_storage[i] = text_from_clipboard[i];
			}
			while (true)
			{
				dlugosc_textu = strlen(text_storage);
				dlugosc_textu++;
				if (send(s, text_storage, dlugosc_textu, 0) == SOCKET_ERROR)
				{
					cout << "Serwer zostal zamkniety" << endl;
					break;
				}
				else
					break;
			}
			closesocket(s);
			WSACleanup();
			GlobalUnlock(hglb);
			CloseClipboard();
			textCount++;
			break;
		case CF_HDROP:
			cout << "pliczek" << endl;
			break;
		case CF_BITMAP:
			bitmapCount++;
			break;
		case CF_METAFILEPICT:
			metafilepictCount++;
			break;
		case CF_SYLK:
			sylkCount++;
			break;
		case CF_DIF:
			difCount++;
			break;
		case CF_TIFF:
			tiffCount++;
			break;
		case CF_OEMTEXT:
			oemtextCount++;
			break;
		case CF_DIB:
			dibCount++;
			break;
		case CF_PALETTE:
			paletteCount++;
			break;
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_CREATE:
		hwndNextViewer = SetClipboardViewer(hwnd);
		break;
	case WM_DESTROY:
		ChangeClipboardChain(hwnd, hwndNextViewer);
		PostQuitMessage(0);
		break;
	case WM_DRAWCLIPBOARD:
		uFormat = GetPriorityClipboardFormat(auPriorityList, 4);
		InvalidateRect(hwnd, NULL, TRUE);
		UpdateWindow(hwnd);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return (LRESULT)NULL;
}