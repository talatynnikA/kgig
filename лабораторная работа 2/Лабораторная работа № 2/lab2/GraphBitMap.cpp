#pragma once
#include "stdafx.h"
#include "GraphBitMap.h"
#include <fstream>

int counter = 0;

int ShowBitMap(HWND hWnd, HANDLE hBit, int x, int y)
//Функция отображает рисунок в заданной позиции окна
//hWnd - дискриптор окна, куда выводится изображение
//r – область в окне, куда выводится изображение
//hBit - дискриптор рисунка
//(x,y) - координаты левого верхнего угла изображения в окне вывода
{
	BITMAP BitMap; 
	GetObjectW(hBit, sizeof(BITMAP), &BitMap);
	int Height = BitMap.bmHeight;
	int Width = BitMap.bmWidth;
	HDC hdc = GetDC(hWnd);
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP OldBitmap = (HBITMAP)SelectObject(hdcMem, hBit);
	BitBlt(hdc, x, y, Width, Height, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, OldBitmap);
	ReleaseDC(hWnd, hdc);
	return 0;
}
int ClientToBmp(HWND hWnd, char *Name)
//Сохранение рабочей области окна в файле Name.bmp
//hWnd - дискриптор окна, рабочая область которого сохраняется
//r – область в  окне, которая сохраняется в файле
//Name - имя файла для сохранения
{
	RECT r;
	GetClientRect (hWnd, &r);
	return ClientRectToBmp (hWnd, Name, r);
}
int ClientRectToBmp(HWND hWnd, char* name, RECT r)
	{
	HDC dcDisplay = GetDC(hWnd);
	HDC dcMemory = CreateCompatibleDC(dcDisplay);
	//битовая карта создается на основе контекста
//устройства dcDisplay, поэтому она хранит изображение
	HBITMAP hBmp = CreateCompatibleBitmap(dcDisplay, abs(r.right - r.left), abs(r.bottom - r.top)); //созд. битовую карту в соответствии с выбр. областью
	//Функция abs вычисляет абсолютное значение и возвращает модуль значения
	HBITMAP hOldBmp = (HBITMAP)SelectObject(dcMemory, hBmp);
	BitBlt(dcMemory, 0, 0, abs(r.right - r.left), abs(r.bottom - r.top), dcDisplay, r.left, r.top, SRCCOPY);
	hBmp = (HBITMAP)SelectObject(dcMemory, hOldBmp);

	BITMAP bmp;
	BITMAPINFOHEADER bmpInfoHeader;//передаст структуру с инфой о изображении
	BITMAPFILEHEADER bmpFileHeader; //сюда запишем сам файл
	int colorDepth = 32;
	GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp);

	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoHeader.biWidth = bmp.bmWidth;
	bmpInfoHeader.biHeight = bmp.bmHeight;
	bmpInfoHeader.biPlanes = 1; // Количество слоев для целевого устройства
	bmpInfoHeader.biBitCount = colorDepth;
	bmpInfoHeader.biCompression = BI_RGB;
	bmpInfoHeader.biSizeImage = 0; //размер изображения, для BI_RGB 0, тк без сжатия, если другой формат, то оно сожмет до заданного значения
	bmpInfoHeader.biXPelsPerMeter = 0; //разрешение в пикселях на метр целевого устройства для растрового изображения
	bmpInfoHeader.biYPelsPerMeter = 0;
	bmpInfoHeader.biClrUsed = 0; //Количество цветовых индексов в таблице цветов используемое. Если равно нулю, то юзает макс колво
	bmpInfoHeader.biClrImportant = 0; //Количество цветовых индексов, необходимых для отображения растрового изображения. Если  нуль, все цвета.

	DWORD dwBitSize = ((bmp.bmWidth * colorDepth + 31) / 32) * 4 * bmp.bmHeight;
	DWORD dwDibSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwBitSize;

	bmpFileHeader.bfType = ('M' << 8) | 'B'; //тип файла
	bmpFileHeader.bfSize = dwDibSize;
	bmpFileHeader.bfReserved1 = 0;
	bmpFileHeader.bfReserved2 = 0;
	bmpFileHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

	HANDLE hDib = GlobalAlloc(GHND, dwBitSize + sizeof(BITMAPINFOHEADER));	// Выделяет глобальный блок памяти, во все байты блока записываются нулевые значения и заказывается перемещаемый блок памяти, т.е. логический адрес памяти может изменяться
	LPBITMAPINFOHEADER lpBmpInfoHeader = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpBmpInfoHeader = bmpInfoHeader;

	GetDIBits(dcMemory, hBmp, 0, (UINT)bmp.bmHeight, (LPSTR)lpBmpInfoHeader + sizeof(BITMAPINFOHEADER), (BITMAPINFO*)lpBmpInfoHeader, DIB_RGB_COLORS);

	HANDLE hFile = CreateFile((LPCWSTR)name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, L"Не удалось создать файл", L"Ошибка", MB_ICONSTOP);
		return 0;
	}

	DWORD dwWritten;

	WriteFile(hFile, (LPSTR)&bmpFileHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	WriteFile(hFile, (LPSTR)lpBmpInfoHeader, dwDibSize, &dwWritten, NULL);

	GlobalUnlock(hDib);	// Далее освобождение памяти
	GlobalFree(hDib);
	CloseHandle(hFile);

	ReleaseDC(hWnd, dcDisplay);
	DeleteDC(dcMemory);

	if (dwWritten == 0)
	{
		MessageBox(NULL, L"Не удалось записать файл", L"Ошибка", MB_ICONSTOP);
		return 0;
	}
	//	HANDLE fh = CreateFile ((LPCWSTR)name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	//if (fh == INVALID_HANDLE_VALUE) //не создался
	//	return 2;
	//BITMAPINFOHEADER bi;//структура содержит описание изображения
	//	ZeroMemory (&bi, sizeof (BITMAPINFOHEADER));//все заполняет нулями
	//bi.biSize = sizeof (BITMAPINFOHEADER);
	//bi.biWidth = r.right - r.left;
	//bi.biHeight = r.bottom - r.top;
	//bi.biPlanes = 1; 
	//bi.biBitCount = 32; //16 глубина цветов
	//bi.biSizeImage = (bi.biWidth * bi.biBitCount + 31)/32*4*bi.biHeight;

	//BITMAPFILEHEADER bmfHdr; //описывает тип файла, размер, смещение области битов
	//	ZeroMemory (&bmfHdr, sizeof (BITMAPFILEHEADER));
	//bmfHdr.bfType = 0x4D42; //BM  ('M'<<8)|'B';
	//bmfHdr.bfSize = bi.biSizeImage + sizeof (BITMAPFILEHEADER) + bi.biSize;
	//bmfHdr.bfReserved1 = bmfHdr.bfReserved2 = 0;
	//bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)bi.biSize;

	//HDC hDC = GetDC (hWnd); //контекст изображения
	//HDC hDCMem = CreateCompatibleDC (hDC); //получаем дискриптор памяти
	//HBITMAP hBitmap = CreateCompatibleBitmap (hDC, bi.biWidth, bi.biHeight);//создаем битовую карту
	//HBITMAP oldBitmap = (HBITMAP)SelectObject (hDCMem, hBitmap); // в созданый контекст памяти вносит дискриптор битовой карты
	//BitBlt (hDCMem, 0, 0, bi.biWidth, bi.biHeight, hDC, r.left, r.top, SRCCOPY); //копирует из памяти картинку, которая в ней находится
	//hBitmap = (HBITMAP)SelectObject (hDCMem, oldBitmap); //перезаписываем картинку

	//HANDLE hDIB = GlobalAlloc (GHND /*GMEM_FIXED*/, bi.biSizeImage); //коды цвета в бит формате
	//char* lp = (char*)GlobalLock (hDIB); 
	//GetDIBits (hDC, hBitmap, 0, bi.biHeight, lp, (LPBITMAPINFO)&bi, DIB_RGB_COLORS);	// не используется?
	//DWORD dwWritten = sizeof (BITMAPFILEHEADER);//запись файла( заголовочный файл,картинка, карта цветов)
	//	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	//dwWritten = sizeof (BITMAPINFOHEADER); 
	//	WriteFile (fh, (LPSTR)&bi, sizeof (BITMAPINFOHEADER), &dwWritten, NULL);
	//dwWritten = bi.biSizeImage;
	//	WriteFile (fh, lp, bi.biSizeImage, &dwWritten, NULL);

	//GlobalUnlock(hDIB);    
	//GlobalFree(hDIB);

	//DeleteObject(hBitmap);
	////GlobalFree (GlobalHandle (lp));
	//lp = NULL;
	//CloseHandle (fh);
	//ReleaseDC (hWnd, hDCMem);
	//ReleaseDC (hWnd, hDC);
	//DeleteDC (hDCMem);
	//DeleteDC (hDC);
	//if (dwWritten == 2) return 2;
	//counter++;
	//return 0;

	}