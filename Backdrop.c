#include <windows.h>
#include "Backdrop.h"
#include "Globals.h"
#include "Utils.h"

static LPCSTR       backdropClassName = "Kaia.BackdropWindow";
static WNDCLASSEX   backdropClass;
static BOOL         isBackdropClassRegistered = FALSE;
static HBRUSH       hbrBackground = NULL;

static LRESULT CALLBACK BackdropWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static void RegisterBackdropClass(void);

/** Backdrop window procedure */
static LRESULT CALLBACK BackdropWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nVirtKey;
	
	switch(uMsg){
		case WM_LBUTTONUP:
			return SendMessage(hwnd, WM_CLOSE, 0L, 0L);
		case WM_CLOSE:
			DestroyWindow(hwnd);
			PostQuitMessage(0);
			return 0;
		case WM_KEYUP:
			nVirtKey = (int)wParam;
			if(nVirtKey == VK_ESCAPE){
				return SendMessage(hwnd, WM_CLOSE, 0L, 0L);
			}
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}


/** Creates and positions the backdrop window */
HWND CreateBackdropWindow(COLORREF backgroundColor)
{
	LOGBRUSH    lbr;
	HWND        desktopWindow;
	RECT        desktopRect;
	HWND        backdropWindow;
	
	/* Create background brush */
	if (hbrBackground == NULL)
	{
		lbr.lbStyle = BS_SOLID;
		lbr.lbColor = backgroundColor;
		lbr.lbHatch = 0L;
		hbrBackground = CreateBrushIndirect(&lbr);
	}
	
	RegisterBackdropClass();
		
	/* Create window with dimensions of desktop */
	desktopWindow = GetDesktopWindow();
	if (!GetWindowRect(desktopWindow, &desktopRect) == TRUE)
	{
		DisplayErrorMessage("GetDesktopWindow() failed.");
	}

	backdropWindow = CreateWindow(
		backdropClassName, 
		NULL,
		WS_BORDER,
		desktopRect.left,
		desktopRect.top,
		(desktopRect.right - desktopRect.left),
		(desktopRect.bottom - desktopRect.top),
		(HWND) NULL,
		(HMENU) NULL,
		ghInstance,
		(LPVOID) NULL
	);	
	if (backdropWindow == NULL)
	{
		DisplayErrorMessage("CreateWindow() failed.");
	}
	/* Hide all window "furniture" */
	SetWindowLong(backdropWindow, GWL_STYLE, 0);
	SetWindowPos(
		backdropWindow, 
		0, 
		desktopRect.left,
		desktopRect.top,
		(desktopRect.right - desktopRect.left),
		(desktopRect.bottom - desktopRect.top),
		SWP_FRAMECHANGED
	);
	
	return backdropWindow;
}


/** Releases backdrop window resources */
void DestroyBackdropWindow()
{
	/* De-register window class */
	if (isBackdropClassRegistered)
	{
		UnregisterClass(backdropClassName, ghInstance);
	}
	
	/* Free GDI resources */
	if (hbrBackground != NULL)
	{
		DeleteObject(hbrBackground);
	}
}


/** Registers the backdrop window class or dies */
static void RegisterBackdropClass(void)
{
	backdropClass.cbSize         = sizeof(backdropClass);
	backdropClass.style          = CS_HREDRAW | CS_VREDRAW;
	backdropClass.lpfnWndProc    = BackdropWndProc;
	backdropClass.cbClsExtra     = 0;
	backdropClass.cbWndExtra     = 0;
	backdropClass.hInstance      = ghInstance;
	backdropClass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
	backdropClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
	backdropClass.hbrBackground  = hbrBackground;
	backdropClass.lpszMenuName   = NULL;
	backdropClass.lpszClassName  = backdropClassName;
	backdropClass.hIconSm        = LoadImage(
		ghInstance, 
		MAKEINTRESOURCE(5), 
		IMAGE_ICON,
		GetSystemMetrics(SM_CXSMICON), 
		GetSystemMetrics(SM_CYSMICON), 
		LR_DEFAULTCOLOR
	);
	if (RegisterClassEx(&backdropClass) == 0)
	{
		DisplayErrorMessage("RegisterClassEx() failed.");
	}
	isBackdropClassRegistered = TRUE;
}