#include "Utils.h"
#include "Globals.h"

static BYTE HexToByte(LPCSTR ch);

void DisplayErrorMessage(LPCSTR msg)
{
	CHAR  errorBuffer[1024];
	DWORD errorCode = GetLastError();
	
	wsprintf(errorBuffer, "%s\nError code: %lu", msg, errorCode);
	MessageBox(NULL, errorBuffer, gAppTitle, MB_OK | MB_ICONSTOP);
	ExitProcess(1);
}


/** Given a string, find the first occurrence an RGB hex triplet (prefaced with #) and convert it to a COLORREF */
COLORREF RgbTripleToColorref(LPCSTR rgb)
{
	BYTE       red    = 0xFF;
	BYTE       green  = 0xFF;
	BYTE       blue   = 0xFF;
	LPTSTR     buffer;
	LPTSTR     currPos;
	LPTSTR     nextPos;
	CHAR       currChar;
	UINT       charCount;
	BOOL       gotStart;
	
	buffer = (LPTSTR)LocalAlloc(LMEM_FIXED, (lstrlen(rgb) + 1) * sizeof(CHAR));
	lstrcpy(buffer, rgb);
	CharUpper(buffer);
	currPos   = buffer;
	charCount = 0;
	gotStart = FALSE;
	do
	{
		currChar = *currPos;
		if (!gotStart)
		{
			if (currChar == '#')
			{
				gotStart = TRUE;
				charCount = 0;
			}
		}
		else
		{
			if(charCount == 1)
			{
				red   = HexToByte(currPos);
			}
			else if(charCount == 3)
			{
					green = HexToByte(currPos);
			}
			else if(charCount == 5)
			{
				blue  = HexToByte(currPos);
			}
		}
		nextPos = CharNext(currPos);
		if (nextPos == currPos) break;
		currPos = nextPos;
		charCount++;
	} while(TRUE);
	LocalFree((HLOCAL)buffer);	
	return RGB(red, green, blue);
}


static BYTE HexToByte(LPCSTR ch)
{
	CHAR digits[] = "0123456789ABCDEF";
	BYTE retVal = 0;
	int ii, jj;
	
	for (ii = 0; ii < 2; ++ii)
	{
		for (jj = 0; jj < lstrlen(digits); ++jj)
		{
			if (ch[ii] == digits[jj])
			{
				if (ii == 0)
				{
					retVal += jj * 16;
				}
				else
				{
					retVal += jj;
				}
				break;
			}
		}
	}
	return retVal;
}
