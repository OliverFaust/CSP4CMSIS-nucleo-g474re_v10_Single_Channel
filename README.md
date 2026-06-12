# CSP4CMSIS Simple Sender‑Receiver Demo for NUCLEO-G474RE

A minimal demonstration of the **CSP (Communicating Sequential Processes)** library using CMSIS‑RTOS v2 on an STM32G474RE microcontroller. This project implements a classic rendezvous channel between two processes: one sender that transmits an ever‑incrementing unsigned integer, and one receiver that prints each value. The formal CSP model can be found [here](https://github.com/OliverFaust/CSP4CMSIS-nucleo-g474re_v10_Single_Channel/tree/main/Formal%20model).

## Features

- **FreeRTOS** with CMSIS‑RTOS v2 API  
- **CSP4CMSIS** library for channel‑based, deterministic concurrency  
- **Rendezvous synchronisation** – sender and receiver meet exactly at each message exchange  
- **Zero‑heap** static memory allocation – all channels and processes reside in `.data`/`.bss`  
- **Roll‑over counter** – `unsigned int` wraps from `UINT_MAX` to `0` automatically  
- **Serial console output** via USART1 (115200 baud) – only the receiver prints, so no message interleaving

## Hardware Requirements

- STM32 Nucleo‑G474RE board  
- USB cable for power, programming, and serial communication  
- No external components required

## Software Requirements

- STM32CubeIDE (or any ARM GCC toolchain)  
- CSP4CMSIS library (included as a git submodule or directly in `lib/`)

## Serial Configuration

| Parameter   | Value          |
|-------------|----------------|
| Baud Rate   | 115200         |
| Data Bits   | 8              |
| Stop Bits   | 1              |
| Parity      | None           |
| Flow Control| None           |

## Building with STM32CubeIDE

1. **Clone this repository** (do not place it inside your STM32CubeIDE workspace directory).  
2. Open STM32CubeIDE.  
3. Go to `File → Import → Existing Projects into Workspace`.  
4. Select the cloned directory.  
5. Build the project (default configuration `Debug` or `Release`).  
6. Flash the binary to your Nucleo board.

## Project Structure
```text
├── Core/ # Main application code (main.c, application.cpp)
├── Drivers/ # STM32 HAL drivers
├── lib/CSP4CMSIS/ # CSP library (static, zero‑heap)
├── Middlewares/ # FreeRTOS + CMSIS‑RTOS v2
├── .gitignore # Excludes build artefacts
└── README.md
```

## How It Works

1. **Channel**: `static Channel<MessageType> chan;` – a blocking rendezvous channel with zero capacity.  
2. **Sender process**: runs an infinite loop, sending the current value of `counter` through `out << counter`. The send operation blocks until the receiver is ready. After sending, `counter` increments (rolls over automatically).  
3. **Receiver process**: runs an infinite loop, waiting for a message with `in >> received`. The receive operation blocks until the sender has sent a value. Once received, it prints `Send: X Received: X`.  
4. **Parallel composition**: `InParallel(sender, receiver)` starts both processes simultaneously. The `ExecutionMode::StaticNetwork` ensures no dynamic memory allocation after startup.

Because the channel is a rendezvous (capacity 0), the two processes are perfectly synchronised – every value sent is immediately received and printed.

## Example Console Output

```text
Welcome to STM32 world !
=== STM32 FreeRTOS + CSP4CMSIS bootstrap ===

--- Single Sender & Receiver with Infinite Loop ---
Send: 0 Received: 0
Send: 1 Received: 1
Send: 2 Received: 2
Send: 3 Received: 3
...
Send: 4294967295 Received: 4294967295
Send: 0 Received: 0
...
```

## Key CSP4CMSIS Concepts Demonstrated

- **Process** – process initialisation and run function.
- **Chanout / Chanin** – typed ports for sending and receiving.
- **Channel** – synchronous rendezvous between processes.
- **Static network** – no dynamic memory allocation after startup.
- **Process composition** – `InParallel` combines independent processes.
- **External choice** (not used here, but supported via `Alternative`).

## Troubleshooting

- **No output on serial**: Verify the baud rate and that the correct COM port is used.
- **Program hangs**: The rendezvous channel blocks forever if only one process runs. Ensure both `Sender` and `Receiver` are included in `InParallel`.
- **Heap usage warning**: CSP4CMSIS uses zero heap – all memory is static. If you see heap allocations, check that you are not using `new`/`malloc` elsewhere.

## License

MIT License – see `LICENSE` file (if included) or refer to the CSP4CMSIS library license.

## Acknowledgments

- STMicroelectronics for the STM32 HAL and CMSIS‑RTOS v2
- The FreeRTOS team
- [CSP4CMSIS](https://oliverfaust.github.io/CSP4CMSIS/) library by Oliver Faust

