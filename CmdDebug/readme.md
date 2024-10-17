## ʹ�÷���
����Ŀ������FreeRTOS����ϵͳ��
### 1.��ֲ
1. ��`\UserCode\Cmd`�ļ��С� `\UserCode\global` �ļ��С� `UserCode\sys` �ļ����µ��ļ�ȫ����ֲ���Լ�����Ŀ�С�
2. �� `UserCode\Usart` �ļ����µ� `myusart.c` `myusart.h` �ļ������Լ��Ĺ��̡�
3. ����������Ŀ����޸ĺ�
   1. `\UserCode\Usart\myusart.h` ��
    ```c
        #define UART_RX_LEN_MAX 30//�����ճ���
        #define UART_TX_LEN_MAX 50//����ͳ���
    ```
    2. `\UserCode\Cmd\cmd.h` ��
    ```c
        #define CMD_COUNT_MAX 30 //�����������
    ```
    3. `\UserCode\sys\system.h` ��
    ```c
        #define OS_TASK_NUM_MAX 15//�����������
    ```
4. ����������Ŀʹ�������޸ĵ���ʹ�ô��ڣ�
    `\UserCode\Cmd\cmd_task.c` �� `cmd_task`�����е� `uart_dma_init`�ĵڶ��������͵���������
### 2.APIʹ��
#### ���񴴽�
����̬����ʱʹ��`sys.c`�ļ��µ� `myTaskCreat` API����API������FreeRTOS��`xTaskCreat` API����˸�API��κ� `xTaskCreat` ��ȫһ��
#### ����Լ�������
�û�����Ļص��������û�ʵ�֣�������κͷ��ض��� `void` ��Ȼ����� `\UserCode\Cmd\cmd.c` �� `cmd_install` ����ע������



### 3.������
- [ ] �޸��ļ��ṹ�����ж���������ͬһ��ͷ�ļ���
- [ ] ���ƶ��Ⱪ¶��API��ʵ�����ã����ã��߼�˳��
- [ ] ����sys�ļ��е�OS��װ��ʵ�������ɾ��ʱ�������б����Ƴ�
- [ ] �����б��������񡰾�����������״̬��ͨ����װOS��APIʵ������״̬׷��
- [ ] ���ܻ���ֲEasyLogger���ڴ˻�����ʵ����־���
   