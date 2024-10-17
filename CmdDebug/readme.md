## 使用方法
本项目运行在FreeRTOS操作系统下
### 1.移植
1. 将`\UserCode\Cmd`文件夹、 `\UserCode\global` 文件夹、 `UserCode\sys` 文件夹下的文件全部移植到自己的项目中。
2. 将 `UserCode\Usart` 文件夹下的 `myusart.c` `myusart.h` 文件加入自己的工程。
3. 根据自身项目情况修改宏
   1. `\UserCode\Usart\myusart.h` 的
    ```c
        #define UART_RX_LEN_MAX 30//最大接收长度
        #define UART_TX_LEN_MAX 50//最大发送长度
    ```
    2. `\UserCode\Cmd\cmd.h` 的
    ```c
        #define CMD_COUNT_MAX 30 //最大命令数量
    ```
    3. `\UserCode\sys\system.h` 的
    ```c
        #define OS_TASK_NUM_MAX 15//最大任务数量
    ```
4. 根据自身项目使用外设修改调试使用串口：
    `\UserCode\Cmd\cmd_task.c` 的 `cmd_task`函数中的 `uart_dma_init`的第二个参数和第三个参数
### 2.API使用
#### 任务创建
任务动态创建时使用`sys.c`文件下的 `myTaskCreat` API，该API调用了FreeRTOS的`xTaskCreat` API，因此该API入参和 `xTaskCreat` 完全一致
#### 添加自己的命令
用户命令的回调函数需用户实现，函数入参和返回都是 `void` ，然后调用 `\UserCode\Cmd\cmd.c` 的 `cmd_install` 函数注册命令



### 3.待完善
- [ ] 修改文件结构，所有对外配置在同一个头文件中
- [ ] 完善对外暴露的API，实现易用，好用，逻辑顺畅
- [ ] 完善sys文件中的OS封装，实现任务的删除时从任务列表中移除
- [ ] 任务列表增加任务“就绪”“挂起”状态，通过封装OS的API实现任务状态追踪
- [ ] 可能会移植EasyLogger，在此基础上实现日志输出
   