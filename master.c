#include <stdio.h>
#include <modbus.h>
#include <errno.h>
#include "config.h"

int main()
{
    modbus_t *ctx;
    uint16_t tab_reg[32];

    // 创建新的 RTU，波特率设置为 115200
    ctx = modbus_new_rtu("/dev/ttyACM0", BAUD_RATE, 'N', 8, 1);

    // 设置 Modbus 从机 ID 为 1
    modbus_set_slave(ctx, 1);

    // 连接到 Modbus 从机
    if (modbus_connect(ctx) == -1)
    {
        fprintf(stderr, "连接失败: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    // 从地址 0 开始读取 10 个寄存器
    if (modbus_read_registers(ctx, 0, 10, tab_reg) == -1)
    {
        fprintf(stderr, "读取失败: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        printf("寄存器 %d: %d\n", i, tab_reg[i]);
    }

    // 关闭连接并释放 Modbus
    modbus_close(ctx);
    modbus_free(ctx);
    return 0;
}
