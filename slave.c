#include <stdio.h>
#include <modbus.h>
#include <errno.h>
#include "config.h"

int main()
{
    modbus_t *ctx;
    modbus_mapping_t *mb_mapping;

    // 创建新的 RTU，波特率设置为 115200
    ctx = modbus_new_rtu("/dev/ttyACM0", BAUD_RATE, 'N', 8, 1);

    // 设置 Modbus 从机 ID 为 1
    modbus_set_slave(ctx, 1);

    // 连接到 Modbus 主机
    if (modbus_connect(ctx) == -1)
    {
        fprintf(stderr, "连接失败: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    // 分配 Modbus 映射（100 个保持寄存器）
    mb_mapping = modbus_mapping_new(0, 0, 100, 0);
    if (mb_mapping == NULL)
    {
        fprintf(stderr, "映射失败: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    while (1)
    {
        uint8_t query[MODBUS_RTU_MAX_ADU_LENGTH];
        // 从 Modbus 主机接收查询
        int rc = modbus_receive(ctx, query);

        if (rc > 0)
        {
            // 回复查询
            modbus_reply(ctx, query, rc, mb_mapping);
        }
        else if (rc == -1)
        {
            fprintf(stderr, "接收失败: %s\n", modbus_strerror(errno));
            break;
        }
    }

    // 释放 Modbus
    modbus_mapping_free(mb_mapping);
    modbus_close(ctx);
    modbus_free(ctx);
    return 0;
}
