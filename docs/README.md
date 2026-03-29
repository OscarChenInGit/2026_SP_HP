# Freenove 4WD Raspberry Pi Car - C Refactor Starter

这是一个**从原始 Python Server 逻辑抽出来的 C 版重构骨架**，目标是先把最核心的部分落到 C：

- 命令协议解析（`CMD_MOTOR`, `CMD_SERVO`, `CMD_SONIC`...）
- 电机控制抽象
- 舵机控制抽象
- 小车自动模式核心逻辑（超声/红外/循光）
- 硬件访问 HAL（目前先给 stub）

## 没有直接搬过去的部分

这些模块原仓库里和 Python 生态绑定很深，建议第二阶段再做：

- PyQt Server UI
- Camera / OpenCV 视频流
- Python 多线程 + multiprocessing 结构
- 网络服务器完整移植（目前示例只保留 stdin 命令入口）
- WS2812 / LED 复杂特效

## 目录说明

- `include/`：头文件
- `src/message.c`：命令解析，替代原 `message.py`
- `src/command.c`：命令枚举映射，替代原 `command.py`
- `src/motor.c`：电机控制，替代原 `motor.py`
- `src/servo.c`：舵机控制，替代原 `servo.py`
- `src/car.c`：自动模式逻辑，替代原 `car.py`
- `src/hw_stub.c`：硬件抽象层 stub，后面你可以替换成真正的 Pi I2C/GPIO 实现
- `src/main.c`：演示入口

## 编译

```bash
make
```

## 运行

```bash
./freenove_car
```

输入示例：

```text
CMD_MOTOR#1000#1000#1000#1000
CMD_SERVO#0#90
CMD_SONIC
CMD_LIGHT
CMD_POWER
```

## 下一步建议

### 第一步：先把硬件打通
把 `src/hw_stub.c` 换成 Raspberry Pi 真正实现：

- PCA9685: I2C `write/read`
- ADC: 对接你板子的 ADC 芯片
- Ultrasonic: trig/echo GPIO
- Infrared: GPIO 输入
- Buzzer: GPIO 输出

### 第二步：把 TCP server 补上
把原来的：

- `tcp_server.py`
- `server.py`
- `main.py` 里的命令接收线程

改成：

- `server_socket.c`
- `client_session.c`
- `event_loop.c`

建议用：

- `select()` 或 `poll()`
- 一个线程收命令，一个线程发视频（如果你后面还保留视频）

### 第三步：视频和 UI 分离
如果你真的想“完全 C 化”，我建议：

- 小车控制端：纯 C，跑在 Pi 上
- PC 控制端 UI：保留 Python / Qt 或改成 C++ Qt

因为：

- UI 用 C 写成本很高
- 摄像头/OpenCV/GUI 在 Python/C++ 更顺手
- 最适合 C 重构的是**底层控制和协议层**

## 我帮你做了哪些映射

| Python 原文件 | C 重构文件 |
|---|---|
| `message.py` | `src/message.c` |
| `command.py` | `src/command.c` |
| `motor.py` | `src/motor.c` |
| `servo.py` | `src/servo.c` |
| `car.py` | `src/car.c` |

## 你现在最适合的路线

不要一口气把整个项目“全改成 C”。
最稳的做法是：

1. 先把 **Server/控制逻辑** 改成 C
2. 再把 **TCP 通信** 改成 C
3. 最后决定 UI/视频还要不要保留 Python

这样不会一下子把工程量炸掉。
