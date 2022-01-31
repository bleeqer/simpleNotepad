#include <windows.h>
#include <tchar.h>
#include <iostream>

using namespace std;

// LPCWSTR Long Pointer to Constant Wide String
// Wide means 2 bytes
// long pointer(far pointer) 32bit 

#define FILE_MENU_SAVE 1
#define FILE_MENU_OPEN 2

static TCHAR szWindowClass[] = _T("DesktopApp");
static TCHAR szTitle[] = _T("윈도우즈 데스크탑 어플리케이션");
static TCHAR text[100];

HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void AddMenus(HWND);

void openFile(HWND);
void saveFile(HWND);

void displayFile(char*);
void writeFile(char*);

HMENU hMenu;
HWND hEdit;

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);
		return 1;
	}

	HWND hWnd = CreateWindowEx(
		0,
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, 400,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd)
	{
		MessageBox(
			NULL,
			_T("Call to CreateWindowEx failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL
		);

		return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
	
}

/// <summary>
/// LRESULT
/// - long type, Win32 환경에서 메시지 처리를 마친 후 OS에게 신호를 주기 위한 값
/// - long 변수의 다른 이름일 뿐임 리턴 값임을 좀 더 명확히 나타내기 위한 것
/// 
/// in this function, you write code to handle messages that the application receives from Windows when "events" occurs
/// </summary>
/// <param name="hWnd"></param>
/// <param name="message"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
//LRESULT CALLBACK WndProc(
//	_In_ HWND   hWnd,
//	_In_ UINT   message,
//	_In_ WPARAM wParam,
//	_In_ LPARAM lParam
//	// _In_ means the parameter must be valid and will not be modified
//);



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	HDC hdc;

	switch (message)
	{
	case WM_CREATE: // this message is passed whenever a window is created
		AddMenus(hWnd);
		//  | WS_VSCROLL | ES_MULTILINE
		hEdit = CreateWindow(TEXT("EDIT"), NULL, WS_VISIBLE | WS_CHILD | ES_MULTILINE, 0, 0, 300, 250, hWnd, NULL, NULL, NULL);
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;

		hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_SIZE:
	{
		int width = LOWORD(lParam);
		int height = LOWORD(lParam);
		int gap = 8;

		MoveWindow(hEdit, gap / 2, gap / 2, width, height, false);


		//PostQuitMessage(0);
		//if (wParam == 1)
		//{
		//	
		//	SendMessage(hEdit, EM_SETSEL, 0, -1);
		//}
		break;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case FILE_MENU_OPEN:
		{
			openFile(hWnd);
			int gwtstat = 0;
			/*LPWSTR *ptrText = text[0];*/
			gwtstat = GetWindowText(hEdit, text, 5000);
			// put code saving the text
			break;
		}
		case FILE_MENU_SAVE:
		{
			saveFile(hWnd);

			break;
		}
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

void AddMenus(HWND hWnd)
{
	hMenu = CreateMenu(); // it returns the created menu and assigned to the global variable
	LPCSTR firstMenu = "File(F)";
	//LPCSTR secondMenu = "Edit(E)";
	//LPCSTR thirdMenu = "Format(O)";
	//LPCSTR fourthMenu = "View(V)";
	//LPCSTR fifthMenu = "Help(H)";

	LPCSTR firstFileMenu = "Save(S)";
	
	LPCSTR secondFileMenu = "Open(O)";

	HMENU hFileMenu = CreateMenu();

	

	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, firstMenu); // when i pass in MF_POPUP as the second argument i should choose which menu i wanna popup when the menu is clicked
	


	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_SAVE, firstFileMenu);
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_OPEN, secondFileMenu);

	//AppendMenu(hMenu, MF_STRING, NULL, secondMenu);
	//AppendMenu(hMenu, MF_STRING, NULL, thirdMenu);
	//AppendMenu(hMenu, MF_STRING, NULL, fourthMenu);
	//AppendMenu(hMenu, MF_STRING, NULL, fifthMenu);

	SetMenu(hWnd, hMenu);
} 

void openFile(HWND hWnd)
{
	OPENFILENAME ofn;

	// name of the file will be stored in this string
	char file_name[5000];

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	//WCHAR fileOption[100];
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = file_name;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 5000;
	ofn.lpstrFilter = _T("All Files\0*.*\0Text Files\0*.TXT\0"); // file type filter
	ofn.nFilterIndex = 1;

	GetOpenFileName(&ofn);

	displayFile(ofn.lpstrFile);
	//MessageBox(NULL, ofn.lpstrFile, "", MB_OK);
}

void displayFile(char* path)
{
	FILE* file;
	file = fopen(path, "rb");
	fseek(file, 0, SEEK_END); // zero bytes away from the end 
	int _size = ftell(file);
	rewind(file);
	char* data = new char[_size + 1]; // stores the data
	fread(data, _size, 1, file);
	data[_size] = '\0';

	SetWindowText(hEdit, data);

	fclose(file);
	
}

void saveFile(HWND hWnd)
{
	OPENFILENAME ofn;

	// name of the file will be stored in this string
	char file_name[5000];

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	//WCHAR fileOption[100];
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = file_name;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 5000;
	ofn.lpstrFilter = _T("Text Files\0*.TXT\0"); // file type filter
	ofn.nFilterIndex = 1;

	GetSaveFileName(&ofn);
	//displayFile(ofn.lpstrFile);
	//MessageBox(NULL, ofn.lpstrFile, "", MB_OK);
	writeFile(ofn.lpstrFile);
}

void writeFile(char* path)
{
	FILE* file;
	file = fopen(path, "w");

	int _size = GetWindowTextLength(hEdit);
	char* data = new char[_size + 1];
	GetWindowText(hEdit, data, _size + 1);

	fwrite(data, _size + 1, 1, file);

	fclose(file);
}

