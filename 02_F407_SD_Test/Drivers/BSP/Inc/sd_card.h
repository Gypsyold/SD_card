#ifndef _sd_card_h_
#define _sd_card_h_

#include "main.h"


void SDCard_ShowInfo(void);         //显示SD卡的详细信息
void SDCard_EraseBlocks(void);      //擦除SD卡上指定范围的块
void SDCard_TestWrite(void);        //测试向SD卡指定块写入数据
void SDCard_TestRead(void);         //测试从SD卡指定块读取数据

#endif
