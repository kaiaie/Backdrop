#include <windows.h>
#include <wincon.h>
#include "Backdrop.h"
#include "Utils.h"

HINSTANCE ghInstance;

COLORREF GetColorFromDialog(void);

/** Application entry point */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow){
	HWND       backdropWindow;
	MSG        msg;
	LPTSTR     cmdLine;
	COLORREF   backgroundColor;
	
	ghInstance = hInstance;
	
	/* Check if we can attach to the console; if we can't we're running in the GUI and show the 
	** colour picker; see http://stackoverflow.com/questions/510805
	** Note: AttachConsole only appears in Win32 v5.1 (XP) or higher
	*/
	if (AttachConsole((DWORD)-1) == 0)
	{
		backgroundColor = GetColorFromDialog();
	}
	else
	{
		/* Parse command line */
		cmdLine = GetCommandLine();
		backgroundColor = RgbTripleToColorref(cmdLine);
	}
		
	backdropWindow = CreateBackdropWindow(backgroundColor);
	ShowWindow(backdropWindow, nCmdShow);
	UpdateWindow(backdropWindow);

	/* Message loop */
	while (GetMessage(&msg, (HWND)NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	DestroyBackdropWindow();
	ExitProcess(msg.wParam);
}


/** Displays the common colour chooser dialog and returns the selected value */
COLORREF GetColorFromDialog(void)
{
	COLORREF acrCustClr[16] = { 
		RGB(0xFF, 0xFF, 0xFF), RGB(0xEF, 0xEF, 0xEF), 
		RGB(0xDF, 0xDF, 0xDF), RGB(0xCF, 0xCF, 0xCF), 
		RGB(0xBF, 0xBF, 0xBF), RGB(0xAF, 0xAF, 0xAF), 
		RGB(0x9F, 0x9F, 0x9F), RGB(0x8F, 0x8F, 0x8F), 
		RGB(0x7F, 0x7F, 0x7F), RGB(0x6F, 0x6F, 0x6F), 
		RGB(0x5F, 0x5F, 0x5F), RGB(0x4F, 0x4F, 0x4F), 
		RGB(0x3F, 0x3F, 0x3F), RGB(0x2F, 0x2F, 0x2F), 
		RGB(0x1F, 0x1F, 0x1F), RGB(0x0F, 0x0F, 0x0F) 
	};
	CHOOSECOLOR colorChooser;
	
	colorChooser.lStructSize = sizeof(colorChooser);
	colorChooser.hwndOwner = NULL;
	colorChooser.hInstance = NULL;
	colorChooser.rgbResult = RGB(0xFF, 0xFF, 0xFF);
	colorChooser.lpCustColors = acrCustClr;
	colorChooser.Flags = CC_RGBINIT;
	colorChooser.lCustData = 0L;
	colorChooser.lpfnHook = NULL;
	colorChooser.lpTemplateName = NULL;
	
	if (ChooseColor(&colorChooser)) 
	{
		return colorChooser.rgbResult;
	}
	else
	{
		ExitProcess(0);
	}
}