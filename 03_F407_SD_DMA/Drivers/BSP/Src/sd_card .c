#include "sd_card.h"

uint8_t SDBuf_RX[BLOCKSIZE];


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




/**
 * @brief  使用DMA方式向SD卡写入数据
 * @note   该函数通过DMA(直接存储器访问)方式向SD卡指定块写入测试数据，
 *         相比普通写入方式可减少CPU占用，提高数据传输效率
 *         适用于STM32F407开发板的SD卡高速数据写入场景
 * @param  无
 * @retval 无
 */
void SDCard_TestWrite_DMA(void)
{
    // 打印提示信息，表明开始执行DMA块写入操作
    printf( "*** DMA Writing blocks ***\r\n");
    
    // 定义DMA发送缓冲区，大小为BLOCKSIZE（SD卡块大小）
    // 使用uint16_t类型可一次传输16位数据，提高DMA传输效率
    uint8_t SDBuf_TX[BLOCKSIZE];
    
    // 初始化发送缓冲区数据，填充0到BLOCKSIZE-1的递增数值
    for (uint16_t i = 0; i < BLOCKSIZE; ++i) {
        SDBuf_TX[i] = i;
    }
    
    // 提示即将写入块6
    printf("Writing block 6");
    
    
    // 打印缓冲区中索引10到14的数据，用于验证写入内容
    printf("Data in [10:15] is: %d,%d,%d,%d,%d\r\n",
            SDBuf_TX[10], SDBuf_TX[11], SDBuf_TX[12], SDBuf_TX[13], SDBuf_TX[14]);
    
    // 提示即将调用DMA写入函数
    printf("HAL_SD_WriteBlocks_DMA() is called\r\n");
    
    // 定义要写入的目标块地址（此处为块6）
    uint32_t BlockAdd = 6;
    
    // 定义要写入的块数量（此处写入1个块）
    uint32_t NumberOfBlocks = 1;
    
    // 调用HAL库的DMA方式写入函数
    // 参数：SD卡句柄(hsd)、发送缓冲区、目标块地址、块数量
    // 该函数会启动DMA传输，无需等待完成即可返回，传输完成后会触发回调函数
    HAL_SD_WriteBlocks_DMA(&hsd, SDBuf_TX, BlockAdd, NumberOfBlocks);
}

/**
 * @brief  SD卡DMA发送完成回调函数
 * @note   当SD卡通过DMA方式完成数据发送后，HAL库会自动调用此回调函数
 *         用于处理传输完成后的后续操作
 * @param  hsd: SD卡句柄指针
 * @retval 无
 */
void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
{
    // 打印DMA写入完成提示
    printf( "DMA write complete.\r\n");
    
    // 提示当前回调函数被调用
    printf("HAL_SD_TxCpltCallback() is called\r\n");
    
    // 提示用户可重新选择菜单项或复位系统
    printf("Reselect menu item or reset\r\n");
}


/**
 * @brief  使用DMA方式从SD卡读取数据
 * @note   该函数通过DMA(直接存储器访问)方式从SD卡指定块读取数据，
 *         相比普通读取方式可减少CPU占用，提高数据传输效率
 *         通常与SDCard_TestWrite_DMA()配合使用，读取之前通过DMA写入的数据
 * @param  无
 * @retval 无
 */
void SDCard_TestRead_DMA() 
{
    // 打印提示信息，表明开始执行DMA块读取操作
    printf( "*** DMA Reading blocks ***\r\n");
    
    // 提示即将调用DMA读取函数
    printf("HAL_SD_ReadBlocks_DMA() is called\r\n");
    
    // 定义要读取的目标块地址（此处为块6，与DMA写入的块地址对应）
    uint32_t BlockAdd = 6;
    
    // 定义要读取的块数量（此处读取1个块，与DMA写入的块数量对应）
    uint32_t NumberOfBlocks = 1;
    
    // 调用HAL库的DMA方式读取函数
    // 参数：SD卡句柄(hsd)、接收缓冲区(SDBuf_RX，全局变量)、目标块地址、块数量
    // 该函数会启动DMA传输，无需等待完成即可返回，传输完成后会触发回调函数
    HAL_SD_ReadBlocks_DMA(&hsd, SDBuf_RX, BlockAdd, NumberOfBlocks);
}

/**
 * @brief  SD卡DMA接收完成回调函数
 * @note   当SD卡通过DMA方式完成数据接收后，HAL库会自动调用此回调函数
 *         用于处理接收完成后的后续操作，如验证读取的数据
 * @param  hsd: SD卡句柄指针
 * @retval 无
 */
void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd) 
{
    // 打印DMA读取完成提示
    printf("DMA Read complete.\r\n");
    
    // 提示当前回调函数被调用
    printf("HAL_SD_RxCpltCallback() is called\r\n");
    
    // 打印接收缓冲区中索引10到14的数据，用于验证读取内容
    // 与写入时的对应位置数据对比，可确认读写操作是否正确
    printf("Data in [10:15] is: %d,%d,%d,%d,%d\r\n",
            SDBuf_RX[10], SDBuf_RX[11], SDBuf_RX[12], SDBuf_RX[13], SDBuf_RX[14]);
    
    // 提示用户可重新选择菜单项或复位系统
    printf("Reselect menu item or reset\r\n");
}

