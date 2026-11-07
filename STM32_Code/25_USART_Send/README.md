# 25 USART串口发送基础

这是串口通信的第一课，主要搞懂了单片机是怎么把数据“说”给电脑听的。
- **基础发送**：配置了PA9(TX)为复用推挽输出，波特率9600。封装了 `Serial_SendByte`、`Serial_SendArray` 和 `Serial_SendString`。
- **数字转字符（痛点）**：为了把数字 `1562` 显示出来，我学习视频里手写了 `Serial_Pow` 和 `Serial_SendNum`。其实原理就是利用除法和取余，把千位、百位强行剥离出来，然后加上 `0x30` 转换成字符发出去。
- **printf重定向**：这是最爽的一步。重写了底层的 `fputc` 函数，并在Keil里勾选了 `Use MicroLIB`（避坑：不勾选会导致程序卡死）。现在可以直接用 `printf` 打印变量了。
- **sprintf封装**：另外我还尝试了 `sprintf` 和可变参数 `vsprintf`，自己封装了一个 `Serial_Printf`，以后打日志再也不用手动拼接字符串了。