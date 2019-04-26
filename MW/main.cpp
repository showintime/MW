/*
inlcude&progma here
*/

#include "windows.h"
#include "stdio.h"
#include "io.h"



/*
macro define here
*/



//使用的常量
#define WINDOWSWIDTH 800
#define WINDOWSHEIGHT 600
#define WINDOWSPOSITIONX 100
#define WINDOWSPOSITIONY 50


#define WINDOWSTITLE L"MyNotepad"
#define HELPWINDOWSTITLE L"帮助"

//注册的窗口的类的名字
#define MAINWINDOWSCLASSNAME L"MainWindowsClassName"
#define HELPWINDOWSCLASSNAME L"HelpWindowsClassName"







//菜单响应值
#define MENU_CLEAR  100
#define MENU_OPEN    101
#define MENU_SAVE    102
#define MENU_SAVEAS  103
#define MENU_COMPILE 104
#define MENU_RUN     105
#define MENU_HELP    106

//菜单名字
#define MENU_CLEAR_NAME L"清除"
#define MENU_OPEN_NAME L"打开"
#define MENU_SAVE_NAME L"保存"
#define MENU_SAVEAS_NAME L"另存为"
#define MENU_COMPILE_NAME L"编译"
#define MENU_RUN_NAME L"运行"
#define MENU_HELP_NAME L"帮助"



//错误原因及代码
#define ERRORMESSAGETITLE L"错误："

#define REGISTERMAINWINDOWSCLASSERROR L"注册主窗口失败！"
#define REGISTERMAINWINDOWSCLASSERRORCODE -1

#define CREATEMAINWINDOWSERROR  L"创建窗口失败！"
#define CREATEMAINWINDOWSERRORCODE  -2


/*
defining global variable here
*/

HINSTANCE hInstance;

HWND hMainWnd;
HWND hEditWnd;


WCHAR FILE_NAME[FILENAME_MAX];






/*
defining function here
*/
//主窗口响应函数
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
//帮助窗口响应函数
LRESULT CALLBACK HelpWndProc(HWND, UINT, WPARAM, LPARAM);
//添加菜单
void addMenus(HWND);
//添加控件
void addControls(HWND);
//注册帮助窗口类
void registerHelpClass();



bool getSaveFiles(HWND);
bool getOpenFiles(HWND);

void write_data(WCHAR*);
void read_data(WCHAR*);

//显示帮助窗口
void showHelp(HWND);

void clearWndText(HWND);



/*
WinMain
*/
int WINAPI WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPreInst, _In_ LPSTR lpShowCmd, _In_ int nCmdShow)
{
	hInstance = hInst;
	WNDCLASS wcw = { 0 };
	wcw.style = CS_VREDRAW | CS_HREDRAW;
	wcw.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcw.hCursor = LoadCursor(NULL,IDC_ARROW);
	wcw.hInstance = hInst;
	wcw.lpszClassName = MAINWINDOWSCLASSNAME;
	wcw.lpfnWndProc = MainWndProc;
	
	if (!RegisterClass(&wcw))
	{
		MessageBox(0, REGISTERMAINWINDOWSCLASSERROR, ERRORMESSAGETITLE, MB_OK);
		exit(REGISTERMAINWINDOWSCLASSERRORCODE);
	}

	
	

	

	hMainWnd = CreateWindow(MAINWINDOWSCLASSNAME,WINDOWSTITLE,WS_OVERLAPPEDWINDOW|WS_VISIBLE,WINDOWSPOSITIONX,WINDOWSPOSITIONY,WINDOWSWIDTH,WINDOWSHEIGHT,0,0,hInst,0);
	if (!hMainWnd)
	{
		MessageBox(0,CREATEMAINWINDOWSERROR, ERRORMESSAGETITLE, MB_OK);
		exit(CREATEMAINWINDOWSERRORCODE);
	}
	ShowWindow(hMainWnd, nCmdShow);
	UpdateWindow(hMainWnd);

	MSG msg = { 0 };
	while (GetMessageW(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}




LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	/*
	
	*/
	case WM_CREATE:
	{
		FILE_NAME[0] = L'\0';
		

		addMenus(hWnd);
		addControls(hWnd);
		registerHelpClass();


	}
		break;


	/*
	
	*/
	case WM_COMMAND:

	{
		switch (wp)
		{
		case MENU_CLEAR:
		{
			clearWndText(hEditWnd);
			break;

		}
		case MENU_OPEN:
		{
			if (getOpenFiles(hWnd))
			{
				read_data(FILE_NAME);
			}
			

		}
		break;
		case MENU_SAVE:
		{
			if (FILE_NAME[0]==L'\0')
			{
				if (getSaveFiles(hWnd))
				{
					write_data(FILE_NAME);
				}
				GetFileAttributes();
				
			}
			else
			{
				write_data(FILE_NAME);
			}


		}
		break;
		case MENU_SAVEAS:
		{
			if (getSaveFiles(hWnd))
			{
				write_data(FILE_NAME);
			}

		}
		break;
		case MENU_COMPILE:
		{


		}
		break;
		case MENU_RUN:
		{


		}
		break;
		case MENU_HELP:
		{
			showHelp(hWnd);


		}

		break;
		}

	}

		break;

	/*
		
		
	*/
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		MoveWindow(hEditWnd, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, true);
		EndPaint(hWnd, &ps);
	}
		break;



	/*
	
	
	*/
	case WM_DESTROY:
	{
		
		
		PostQuitMessage(0);
	}
		
		break;

	/*
	
	*/
	default:
	{
		return DefWindowProc(hWnd, msg, wp, lp);
	}
		break;
	}
	return 0;
}

void addMenus(HWND hWnd)
{
	HMENU hMenu = CreateMenu();

	AppendMenu(hMenu, MF_STRING, MENU_CLEAR, MENU_CLEAR_NAME);
	AppendMenu(hMenu, MF_STRING, MENU_OPEN, MENU_OPEN_NAME);
	AppendMenu(hMenu, MF_STRING, MENU_SAVE, MENU_SAVE_NAME);
	AppendMenu(hMenu, MF_STRING, MENU_SAVEAS, MENU_SAVEAS_NAME);
	AppendMenu(hMenu, MF_STRING, MENU_COMPILE, MENU_COMPILE_NAME);
	AppendMenu(hMenu, MF_STRING, MENU_RUN, MENU_RUN_NAME);
	AppendMenu(hMenu, MF_STRING, MENU_HELP, MENU_HELP_NAME);

	SetMenu(hWnd, hMenu);
}



void addControls(HWND hWnd)
{

	hEditWnd = CreateWindow(L"edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE, 0, 0, 800, 600, hWnd, 0, 0, 0);


}




bool getSaveFiles(HWND hWnd)
{
	WCHAR file_name[FILENAME_MAX];

	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFile = file_name;
	ofn.lpstrFile[0] = L'\0';
	ofn.lpstrFilter = L"All Files\0*.*\0";
	ofn.nMaxFile = FILENAME_MAX;
	ofn.nFilterIndex = 1;
	ofn.hwndOwner = hWnd;

	if (!GetOpenFileName(&ofn))
	{
		return false;
	}


	lstrcpy(FILE_NAME, ofn.lpstrFile);

	return true;
}

bool getOpenFiles(HWND hWnd)
{
	WCHAR file_name[FILENAME_MAX];

	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFile = file_name;
	ofn.lpstrFile[0] = L'\0';
	ofn.lpstrFilter = L"All Files\0*.*\0";
	ofn.nMaxFile = FILENAME_MAX;
	ofn.nFilterIndex = 1;
	ofn.hwndOwner = hWnd;

	if (!GetOpenFileName(&ofn))
	{
		return false;
	}
	


	lstrcpy(FILE_NAME, ofn.lpstrFile);
	return true;
}



void read_data(WCHAR* strfilename)
{
	FILE* file = NULL;
	if (strfilename[0] == '\0')
	{
		MessageBox(0, L"error file read data FILE_NAME is empty", L"read_data error", MB_OK);
		return;
	}
	_wfopen_s(&file, strfilename, L"r+");
	
	if (!file)
	{
		MessageBox(0, L"error file open ", L"read_data error", MB_OK);
		return;
	}
	
	fseek(file, 0, SEEK_END);
	int _csize = ftell(file);
	char* cdata = new char[_csize+1];
	
	
	
	rewind(file);
	if (_csize <= 0)
	{
		fclose(file);
		return;
	}
	fread(cdata, _csize, 1, file);
	fclose(file);

	
	cdata[_csize] = '\0';
	
	
	int _wsize = MultiByteToWideChar(CP_OEMCP, 0, cdata, _csize, 0, 0);
	WCHAR* wdata = new WCHAR[_wsize+1];
	_wsize = MultiByteToWideChar(CP_OEMCP, 0, cdata, _csize, wdata, _wsize);
	if (wdata == NULL)
	{
		return;
	}
	
	
	wdata[_wsize] = L'\0';
	if (!SetWindowTextW(hEditWnd, wdata))
	{
		MessageBox(0, L"set text fail", L"error", MB_OK);
	}
	
	
	
	
}


void write_data(WCHAR  *strfilename)
{
	
	if (strfilename==NULL)
	{
		return;
	}
	
	FILE* file = NULL;
	_wfopen_s(&file, strfilename, L"w+");
	if (!file)
	{
		MessageBox(0, L"error file open", L"write_data error", MB_OK);
		return;
	}
	int _wsize = GetWindowTextLength(hEditWnd);
	WCHAR* wdata = new WCHAR[_wsize + 1];
	GetWindowText(hEditWnd, wdata, _wsize + 1);
	
	int _csize = WideCharToMultiByte(CP_OEMCP, 0, wdata, wcslen(wdata), 0, 0, 0, 0);
	char* cdata = new char[_csize+1];
	_csize = WideCharToMultiByte(CP_OEMCP, 0, wdata, wcslen(wdata), cdata, _csize, 0, 0);
	if (!_csize)
	{
		MessageBox(0, L"error file write", L"error", MB_OK);
		return;
	}
	fwrite(cdata, _csize, 1, file);
	fclose(file);


	
}

void registerHelpClass()
{
	WNDCLASS helpWc = { 0 };
	helpWc.lpszClassName = HELPWINDOWSCLASSNAME;
	helpWc.style = CS_VREDRAW | CS_HREDRAW;
	helpWc.lpfnWndProc = HelpWndProc;

	helpWc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	helpWc.hInstance = hInstance;
	if (!RegisterClass(&helpWc))
	{
		MessageBox(0, L"HELP WINDOWS REGISTER FAILURE", ERRORMESSAGETITLE, MB_OK);
		exit(-3);
	}

}


LRESULT CALLBACK HelpWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		EnableWindow(hMainWnd, true);
		DestroyWindow(hWnd);
		break;

	default:
		return DefWindowProc(hWnd, msg, wp, lp);
		break;
	}
	return 0;
}





void showHelp(HWND hWnd)
{
	

	
	HWND hHelpWnd = CreateWindow(HELPWINDOWSCLASSNAME, HELPWINDOWSTITLE, WS_VISIBLE|WS_OVERLAPPEDWINDOW, 100, 100, 200, 200, hWnd, 0, 0, 0);
	if (!hHelpWnd)
	{
		MessageBox(0, L"error for create helpwindows", L"error", MB_OK);
		return;
	}
	
	ShowWindow(hHelpWnd, CS_ENABLE);
	UpdateWindow(hHelpWnd);

	EnableWindow(hWnd, false);
	MSG msg = { 0 };
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}

void clearWndText(HWND hWnd)
{
	SetWindowText(hWnd, L"");
}






