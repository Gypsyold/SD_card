#include "sd_card.h"


/**
 * @brief  显示SD卡的详细信息
 * @note   该函数通过HAL库获取SD卡信息并通过串口打印出来
 *         适用于STM32F407开发板的SD卡接口
 * @param  无
 * @retval 无
 */
void SDCard_ShowInfo(void)
{
    // 定义SD卡信息结构体，用于存储从SD卡获取的信息
    HAL_SD_CardInfoTypeDef cardINfo;
    
    // 调用HAL库函数获取SD卡信息，hsd是SD卡句柄（在main.c定义）
    HAL_StatusTypeDef res = HAL_SD_GetCardInfo(&hsd, &cardINfo);
    
    // 检查获取SD卡信息是否成功
    if(res != HAL_OK)
    {
        // 获取信息失败，通过串口打印错误信息
        printf("HAL_SD_GetCardInfo() error!\r\n");
    }
    else
    {
        // 获取信息成功，通过串口打印SD卡详细信息
        printf("*** HAL_SD_GetCardInfo() info ***\r\n");
        
        // 打印SD卡类型（如SDV1、SDV2、SDHC等）
        printf("Card Type = %d\r\n", cardINfo.CardType);
        
        // 打印SD卡版本号
        printf("Card Version = %d\r\n", cardINfo.CardVersion);
        
        // 打印SD卡相对地址（RCA，Relative Card Address）
        printf("Relative Card Address = %d\r\n", cardINfo.RelCardAdd);
        
        // 打印SD卡的物理块数量
        printf("Block Count = %d\r\n", cardINfo.BlockNbr);
        
        // 打印每个物理块的大小（单位：字节）
        printf("Block Size(Bytes) = %d\r\n", cardINfo.BlockSize);
        
        // 打印SD卡的逻辑块数量
        printf("Logic Block Count = %d\r\n", cardINfo.LogBlockNbr);
        
        // 打印每个逻辑块的大小（单位：字节）
        printf("Logic Block Size = %d\r\n", cardINfo.LogBlockSize);
        
        // 计算并打印SD卡总容量（单位：MB）
        // 计算公式：总容量 = 物理块数量 × 每个物理块大小 ÷ 1024 ÷ 1024
        printf("SD Card Capacity(MB) = %d\r\n",  cardINfo.BlockNbr / 1024 * cardINfo.BlockSize / 1024);
    }
}


/**
 * @brief  擦除SD卡上指定范围的块
 * @note   该函数用于擦除SD卡上从起始块到结束块的连续区域
 *         适用于STM32F407开发板的SD卡操作，依赖HAL库
 * @param  无
 * @retval 无
 */
void SDCard_EraseBlocks(void)
{
    // 打印提示信息，表明开始执行块擦除操作
    printf("*** Erasing blocks ***\r\n");
    
    // 定义要擦除的起始块地址（此处从块0开始）
    uint32_t BlockStartAdd = 0;
    // 定义要擦除的结束块地址（此处到块10结束，包含块10）
    uint32_t BlockEndAdd = 10;
    
    // 调用HAL库函数擦除指定范围的块
    // 参数：SD卡句柄(hsd)、起始块地址、结束块地址
    HAL_StatusTypeDef res = HAL_SD_Erase(&hsd, BlockStartAdd, BlockEndAdd);
    
    // 检查擦除操作是否成功
    if (res == HAL_OK) {
        // 擦除成功，打印提示信息
        printf( "Erase blocks success\r\n");
    } else {
        // 擦除失败，打印错误信息
        printf("Erase blocks error\r\n");
    }
    
    // 获取当前SD卡的状态
    HAL_SD_CardStateTypeDef cardState = HAL_SD_GetCardState(&hsd);
    
    // 打印当前SD卡状态（状态值对应HAL库定义的枚举，如忙碌、就绪等）
    printf("GetCardState() = %d\r\n", cardState);
    
    // 等待SD卡状态变为"传输就绪"（HAL_SD_CARD_TRANSFER）
    // 擦除操作可能需要一定时间，需等待操作完成
    while (cardState != HAL_SD_CARD_TRANSFER) {
        // 延时1毫秒后再次检查状态
        HAL_Delay(1);
        cardState = HAL_SD_GetCardState(&hsd);
    }
    
    // 打印擦除完成后的SD卡状态
    printf( "GetCardState() = %d\r\n", cardState);
    
    // 提示用户指定范围的块已成功擦除
    printf( "Blocks 0-10 is erased.\r\n");
}


/**
 * @brief  测试向SD卡指定块写入数据
 * @note   该函数用于向SD卡的指定块写入测试数据，并验证写入操作结果
 *         适用于STM32F407开发板的SD卡数据写入测试
 * @param  无
 * @retval 无
 */
void SDCard_TestWrite(void)
{
    // 打印提示信息，表明开始执行块写入操作
    printf("*** Writing blocks ***\r\n");
    
    // 定义数据缓冲区并初始化测试字符串
    // BLOCKSIZE为宏定义，代表SD卡每个块的大小（单位：字节）
    uint8_t pData[BLOCKSIZE] = "Hello, welcome to UPC\0";
    
    // 定义要写入的目标块地址（此处为块5）
    uint32_t BlockAdd = 5;
    
    // 定义要写入的块数量（此处只写入1个块）
    uint32_t NumberOfBlocks = 1;
    
    // 定义操作超时时间（单位：毫秒）
    uint32_t Timeout = 1000;
    
    // 调用HAL库函数向SD卡写入数据
    // 参数：SD卡句柄(hsd)、数据缓冲区、目标块地址、块数量、超时时间
    HAL_StatusTypeDef res = HAL_SD_WriteBlocks(&hsd, pData, BlockAdd, NumberOfBlocks, Timeout);
    
    // 检查写入操作是否成功
    if (res == HAL_OK) {
        // 写入成功，打印提示信息
        printf("Write to Block 5 is OK\r\n");
        // 打印写入的字符串内容
        printf("The string is : %s\r\n", pData);
    } else {
        // 写入失败，打印错误信息
        printf("Write to Block 5 is error\r\n");
    }
}




/**
 * @brief  测试从SD卡指定块读取数据
 * @note   该函数用于从SD卡的指定块读取数据，并验证读取操作结果
 *         通常与SDCard_TestWrite()配合使用，读取之前写入的数据
 *         适用于STM32F407开发板的SD卡数据读取测试
 * @param  无
 * @retval 无
 */
void SDCard_TestRead(void)
{
    // 打印提示信息，表明开始执行块读取操作
    printf( "*** Reading blocks ***\r\n");
    
    // 定义数据缓冲区，用于存储从SD卡读取的数据
    // BLOCKSIZE为宏定义，需与SD卡块大小一致（同写入时使用的块大小）
    uint8_t pData[BLOCKSIZE];
    
    // 定义要读取的目标块地址（此处为块5，与写入测试的块地址对应）
    uint32_t BlockAdd = 5;
    
    // 定义要读取的块数量（此处读取1个块，与写入时的块数量对应）
    uint32_t NumberOfBlocks = 1;
    
    // 定义操作超时时间（单位：毫秒）
    uint32_t Timeout = 1000;
    
    // 调用HAL库函数从SD卡读取数据
    // 参数：SD卡句柄(hsd)、接收数据缓冲区、目标块地址、块数量、超时时间
    HAL_StatusTypeDef res = HAL_SD_ReadBlocks(&hsd, pData, BlockAdd, NumberOfBlocks, Timeout);
    
    // 检查读取操作是否成功
    if (res == HAL_OK) {
        // 读取成功，打印提示信息
        printf( "Read Block 5 is OK\r\n");
        // 打印读取到的字符串内容（若为字符串数据）
        printf( "The string is : %s\r\n", pData);
    } else {
        // 读取失败，打印错误信息
        printf("Read Block 5 is error\r\n");
    }
}

