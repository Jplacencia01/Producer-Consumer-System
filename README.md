# Producer-Consumer System

A C++ implementation of the producer-consumer problem using the Windows API. The program uses separate producer and consumer threads, a bounded circular buffer, semaphores, and a mutex to coordinate concurrent access to shared data.

## Features

- Producer and consumer threads created with the Windows API
- Configurable buffer size and processing limit
- Bounded circular buffer for shared data
- Semaphore-based tracking of empty and filled buffer slots
- Mutex synchronization for shared buffer access
- Thread lifecycle management with `CreateThread`, `WaitForSingleObject`, and `CloseHandle`
- Input validation for buffer size and processing limits

## How It Works

The producer generates random values and places them into the shared buffer. The consumer removes values from the buffer and processes them.

Synchronization prevents the producer from writing when the buffer is full and prevents the consumer from reading when the buffer is empty.

The program accepts two command-line arguments:

```text
<bufferSize> <limit>
```

- `bufferSize` — number of available slots in the bounded buffer
- `limit` — number of items to produce and consume

## Build

This project uses the Windows API and is intended to run on Windows.

Using the Microsoft C++ compiler:

```powershell
cl producer_consumer.cpp
```

## Run

```powershell
.\producer_consumer.exe 5 20
```

Example:

```text
Producer produced: 42
Consumer consumed: 42
Producer produced: 17
Consumer consumed: 17
...
Main program terminated.
```

In this example:

- Buffer size: `5`
- Items produced: `20`
- Items consumed: `20`

## Concepts Demonstrated

- Multithreading
- Producer-consumer synchronization
- Semaphores
- Mutexes
- Circular buffers
- Shared-memory coordination
- Windows thread management

## Project Context

This project was developed as part of an Operating Systems course to explore concurrency, thread synchronization, and shared resource management in C++.