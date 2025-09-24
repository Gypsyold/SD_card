#ifndef _file_opera_h_
#define _file_opera_h_


#include "ff.h"

#include <stdio.h>
#include "main.h"

void fatTest_GetDiskInfo(void);
void fatTest_ScanDir(const TCHAR* PathName);
void fatTest_WriteTXTFile(TCHAR* filename, uint16_t year, uint8_t month, uint8_t day);
void fatTest_WriteBinFile(TCHAR* filename, uint32_t pointCount, uint32_t sampFreq);
void fatTest_ReadTXTFile(TCHAR* filename);
void fatTest_ReadBinFile(TCHAR* filename);
void fatTest_GetFileInfo(TCHAR* filename);

DWORD fat_GetFatTimeFromRTC(void);


#endif  

