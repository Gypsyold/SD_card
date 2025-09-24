#include "file_opera.h"


/**
 * @brief 获取并显示FAT文件系统的磁盘信息
 * @details 该函数获取指定驱动器的FAT文件系统信息，包括：
 *          - FAT文件系统类型
 *          - 扇区大小
 *          - 簇大小
 *          - 总簇数和空闲簇数
 *          - 总扇区数和空闲扇区数
 *          - 总空间和空闲空间
 *          所有信息通过串口打印输出
 */
void fatTest_GetDiskInfo() 
{
    FATFS *fs;                    // FATFS工作区对象指针
    DWORD free_clust;             // 存储空闲簇的数量
    // 获取驱动器0的空闲簇数和文件系统信息
    FRESULT res = f_getfree("0:", &free_clust, &fs);
    
    // 检查获取磁盘信息是否成功
    if (res != FR_OK) 
    {
        printf("f_getfree() error\r\n");
        return;
    }
    
    printf("*** FAT disk info ***\r\n");
    
    // 计算总扇区数：(总簇数-2) * 每簇扇区数
    // 减2是因为前两个簇被保留（FAT表使用）
    DWORD total_sector = (fs->n_fatent - 2) * fs->csize;
    // 计算空闲扇区数：空闲簇数 * 每簇扇区数
    DWORD free_sector = free_clust * fs->csize;

    // 根据扇区大小是否固定来计算空间大小
#if _MAX_SS == _MIN_SS
    // 扇区大小固定时：将扇区数转换为MB（右移11位相当于除以2048）
    DWORD free_space = free_sector >> 11;
    DWORD total_space = total_sector >> 11;
#else
    // 扇区大小可变时：将扇区数转换为KB（乘以扇区大小后右移10位相当于除以1024）
    DWORD free_space = (free_sector * fs->ssize) >> 10;
    DWORD total_space = (total_sector * fs->ssize) >> 10;
#endif

    // 显示FAT文件系统类型
    printf("FAT type = %d\r\n", (int)fs->fs_type);
    // 显示FAT类型说明
    printf("[1=FAT12,2=FAT16,3=FAT32,4=EXFAT]\r\n");

    // 根据扇区大小是否固定显示扇区大小
#if _MAX_SS == _MIN_SS
    printf("Sector size(bytes) = %d\r\n", _MIN_SS);
#else
    printf("Sector size(bytes) = %d\r\n", fs->ssize);
#endif
    
    // 显示簇大小（以扇区为单位）
    printf("Cluster size(sectors) = %d\r\n", fs->csize);
    // 显示总簇数
    printf("Total cluster count = %d\r\n", fs->n_fatent - 2);
    // 显示总扇区数
    printf("Total sector count = %d\r\n", total_sector);

    // 根据扇区大小是否固定显示总空间
#if _MAX_SS == _MIN_SS
    printf("Total space(MB) = %d\r\n", total_space);
#else
    printf("Total space(KB) = %d\r\n", total_space);
#endif

    // 显示空闲簇数
    printf("Free cluster count = %d\r\n", free_clust);
    // 显示空闲扇区数
    printf("Free sector count = %d\r\n", free_sector);

    // 根据扇区大小是否固定显示空闲空间
#if _MAX_SS == _MIN_SS
    printf("Free space(MB) = %d\r\n", free_space);
#else
    printf("Free space(KB) = %d\r\n", free_space);
#endif

    // 显示操作完成信息
    printf("Get FAT disk info OK\r\n");
}


/**
 * @brief 扫描并显示指定目录下的所有文件和子目录
 * @param PathName 要扫描的目录路径
 * @details 该函数会：
 *          1. 打开指定目录
 *          2. 遍历目录中的所有条目
 *          3. 区分显示文件和子目录
 *          4. 通过串口输出所有条目信息
 */
void fatTest_ScanDir(const TCHAR* PathName) {
    DIR dir;                    // 目录对象
    FILINFO dir_info;           // 文件/目录信息结构体
    FRESULT res;                // FatFs函数返回结果

    // 打开指定目录
    res = f_opendir(&dir, PathName);
    if (res != FR_OK) {
        printf("Failed to open directory: %s\r\n", PathName);
        f_closedir(&dir);
        return;
    }

    // 显示目录扫描开始信息
    printf("All entries in dir %s\r\n", PathName);
    printf("--------------------------------\r\n");

    // 循环读取目录中的每个条目
    while (1) {
        // 读取目录中的下一个条目
        res = f_readdir(&dir, &dir_info);
        
        // 如果读取错误或到达目录末尾，退出循环
        if (res != FR_OK || dir_info.fname[0] == 0) {
            break;
        }

        // 判断当前条目是目录还是文件
        if (dir_info.fattrib & AM_DIR) {
            // 如果是目录，显示DIR标记
            printf("DIR   %s\r\n", dir_info.fname);
        } else {
            // 如果是文件，显示FILE标记
            printf("FILE  %s\r\n", dir_info.fname);
        }
    }

    // 显示目录扫描完成信息
    printf("--------------------------------\r\n");
    printf("Scan dir OK\r\n");
    
    // 关闭目录
    f_closedir(&dir);
}

/**
 * @brief 写入文本文件
 * @param filename 要写入的文件名
 * @param year 年份
 * @param month 月份
 * @param day 日期
 * @details 该函数创建一个新的文本文件并写入三行内容：
 *          1. 固定的问候语
 *          2. 固定的位置信息
 *          3. 格式化的日期信息
 */
void fatTest_WriteTXTFile(TCHAR* filename, uint16_t year, uint8_t month, uint8_t day) {
    FIL file;                   // 文件对象
    FRESULT res;                // FatFs函数返回结果

    // 创建并打开文件，如果文件已存在则覆盖
    res = f_open(&file, filename, FA_CREATE_ALWAYS | FA_WRITE);
    if (res == FR_OK) {
        // 写入第一行：固定问候语
        f_puts("Line1: Hello FatFS\n", &file);
        // 写入第二行：固定位置信息
        f_puts("Line2: UPC, Qingdao\n", &file);
        // 写入第三行：格式化的日期信息
        f_printf(&file, "Line3: Date=%04d-%02d-%02d\n", year, month, day);
        printf("Successfully wrote to TXT file: %s\r\n", filename);
    } else {
        printf("Error: Failed to write TXT file: %s\r\n", filename);
    }
    f_close(&file);
}

/**
 * @brief 写入二进制文件
 * @param filename 要写入的文件名
 * @param pointCount 数据点数量
 * @param sampFreq 采样频率
 * @details 该函数创建一个二进制文件并写入：
 *          1. 字符串标识符
 *          2. 数据点数量（32位）
 *          3. 采样频率（32位）
 *          4. 一系列递增的32位数据
 */
void fatTest_WriteBinFile(TCHAR* filename, uint32_t pointCount, uint32_t sampFreq) {
    FIL file;                   // 文件对象
    FRESULT res;                // FatFs函数返回结果
    UINT bw = 0;               // 实际写入的字节数

    // 创建并打开文件，如果文件已存在则覆盖
    res = f_open(&file, filename, FA_CREATE_ALWAYS | FA_WRITE);
    if (res == FR_OK) {
        // 写入标识字符串
        f_puts("ADC1-IN5\n", &file);
        
        // 写入数据点数量
        f_write(&file, &pointCount, sizeof(uint32_t), &bw);
        // 写入采样频率
        f_write(&file, &sampFreq, sizeof(uint32_t), &bw);

        // 生成并写入递增的数据点
        uint32_t value = 1000;
        for (uint16_t i = 0; i < pointCount; i++, value++) {
            f_write(&file, &value, sizeof(uint32_t), &bw);
        }
        printf("Successfully wrote to BIN file: %s\r\n", filename);
    } else {
        printf("Error: Failed to write BIN file: %s\r\n", filename);
    }
    f_close(&file);
}

/**
 * @brief 读取文本文件内容
 * @param filename 要读取的文件名
 * @details 该函数读取文本文件的所有行并通过串口输出
 */
void fatTest_ReadTXTFile(TCHAR* filename) {
    FIL file;                   // 文件对象
    FRESULT res;                // FatFs函数返回结果
    uint8_t line[40];          // 存储每行内容的缓冲区

    printf("Reading TXT file: %s\r\n", filename);
    printf("--------------------------------\r\n");

    // 以只读方式打开文件
    res = f_open(&file, filename, FA_READ);
    if (res == FR_OK) {
        // 逐行读取文件内容
        while (!f_eof(&file)) {
            f_gets(line, sizeof(line), &file);
            printf("%s", line);
        }
        printf("--------------------------------\r\n");
        printf("TXT file read completed\r\n");
    } else if (res == FR_NO_FILE) {
        printf("Error: File %s does not exist\r\n", filename);
    } else {
        printf("Error: Failed to open file %s\r\n", filename);
    }
    f_close(&file);
}

/**
 * @brief 读取二进制文件内容
 * @param filename 要读取的文件名
 * @details 该函数读取二进制文件的：
 *          1. 标识字符串
 *          2. 数据点数量
 *          3. 采样频率
 *          4. 部分数据点示例
 */
void fatTest_ReadBinFile(TCHAR* filename) {
    FIL file;                   // 文件对象
    FRESULT res;                // FatFs函数返回结果
    UINT br = 0;               // 实际读取的字节数
    uint8_t header[40];        // 存储文件头的缓冲区
    uint32_t pointCount;       // 数据点数量
    uint32_t sampFreq;         // 采样频率
    uint32_t *values;          // 存储数据点的数组

    printf("Reading BIN file: %s\r\n", filename);
    printf("--------------------------------\r\n");

    // 以只读方式打开文件
    res = f_open(&file, filename, FA_READ);
    if (res == FR_OK) {
        // 读取标识字符串
        f_gets(header, sizeof(header), &file);
        printf("Header: %s", header);

        // 读取数据点数量
        f_read(&file, &pointCount, sizeof(uint32_t), &br);
        printf("Point count: %d\r\n", pointCount);

        // 读取采样频率
        f_read(&file, &sampFreq, sizeof(uint32_t), &br);
        printf("Sampling frequency: %d\r\n", sampFreq);

        // 分配内存并读取所有数据点
        values = (uint32_t*)malloc(pointCount * sizeof(uint32_t));
        if (values != NULL) {
            for (uint16_t i = 0; i < pointCount; i++) {
                f_read(&file, &values[i], sizeof(uint32_t), &br);
            }

            // 显示部分数据点作为示例
            printf("Sample values:\r\n");
            printf("  value[5] = %d\r\n", values[5]);
            printf("  value[16] = %d\r\n", values[16]);
            
            free(values);
        }
        printf("--------------------------------\r\n");
        printf("BIN file read completed\r\n");
    } else if (res == FR_NO_FILE) {
        printf("Error: File %s does not exist\r\n", filename);
    } else {
        printf("Error: Failed to open file %s\r\n", filename);
    }
    f_close(&file);
}

/**
 * @brief 获取并显示文件信息
 * @param filename 要查询的文件名
 * @details 该函数获取并显示文件的：
 *          1. 文件大小
 *          2. 文件属性
 *          3. 文件名
 *          4. 修改日期和时间
 */
void fatTest_GetFileInfo(TCHAR* filename) {
    FILINFO file_info;          // 文件信息结构体
    FRESULT res;                // FatFs函数返回结果

    printf("File info of: %s\r\n", filename);
    printf("--------------------------------\r\n");

    // 获取文件信息
    res = f_stat(filename, &file_info);
    if (res == FR_OK) {
        // 显示文件大小（字节）
        printf("File size(bytes) = %d\r\n", file_info.fsize);
        
        // 显示文件属性（十六进制）
        printf("File attribute = 0x%X\r\n", file_info.fattrib);
        
        // 显示文件名
        printf("File name = %s\r\n", file_info.fname);

        // 解析并显示修改日期
        uint16_t day = file_info.fdate & 0x001F;
        uint16_t month = (file_info.fdate & 0x01E0) >> 5;
        uint16_t year = 1980 + ((file_info.fdate & 0xFE00) >> 9);
        printf("File Date = %04d-%02d-%02d\r\n", year, month, day);

        // 解析并显示修改时间
        uint16_t hours = (file_info.ftime & 0xF800) >> 11;
        uint16_t minutes = (file_info.ftime & 0x07E0) >> 5;
        uint16_t seconds = (file_info.ftime & 0x001F) << 1;
        printf("File Time = %02d:%02d:%02d\r\n", hours, minutes, seconds);
        
        printf("--------------------------------\r\n");
        printf("File info retrieved successfully\r\n");
    } else if (res == FR_NO_FILE) {
        printf("Error: File %s does not exist\r\n", filename);
    } else {
        printf("Error: Failed to get file info for %s\r\n", filename);
    }
}



/**
  * @brief  从RTC获取时间并转换为FAT文件系统时间格式
  * @param  无
  * @retval DWORD: 返回FAT格式的时间戳，如果获取失败返回0
  * @note   FAT文件系统时间格式说明：
  *         - 日期部分(16位): 
  *           bits 15-9: 年(相对于1980年的偏移)
  *           bits 8-5: 月(1-12)
  *           bits 4-0: 日(1-31)
  *         - 时间部分(16位):
  *           bits 15-11: 时(0-23)
  *           bits 10-5: 分(0-59)
  *           bits 4-0: 秒(0-29, 实际秒数需要乘以2)
  */
DWORD fat_GetFatTimeFromRTC() 
{
    /* 定义时间和日期结构体变量 */
    RTC_TimeTypeDef sTime;    // 用于存储时间信息
    RTC_DateTypeDef sDate;    // 用于存储日期信息
    
    /* 从RTC获取时间，如果成功则继续获取日期 */
    if (HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN) == HAL_OK) {
        /* 获取RTC日期 */
        HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
        
        /* 构造FAT格式的日期部分(16位) */
        WORD date = ((2000 + sDate.Year - 1980) << 9)    // 年: 相对于1980的偏移，左移9位
                  | (sDate.Month << 5)                  // 月: 左移5位
                  | sDate.Date;                          // 日: 直接存放
        
        /* 构造FAT格式的时间部分(16位) */
        WORD time = (sTime.Hours << 11)                  // 时: 左移11位
                  | (sTime.Minutes << 5)                 // 分: 左移5位
                  | (sTime.Seconds >> 1);                // 秒: 除以2(右移1位)
        
        /* 组合日期和时间为32位的FAT时间戳 */
        DWORD fatTime = ((DWORD)date << 16) | time;      // 日期放在高16位，时间放在低16位
        
        return fatTime;                                  // 返回构造好的FAT时间戳
    } else {
        return 0;                                         // 获取RTC时间失败，返回0
    }
}

