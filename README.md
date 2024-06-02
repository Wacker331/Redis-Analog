# Redis-Analog
Test case for Protei internship. It's a simple key-value storage with client-server architecture.

## Requirements
- Linux
- CMake
- C++ 20

## Install
```
git clone https://github.com/Wacker331/Redis-Analog.git
cd Redis-Analog
cmake .
```

## Quick start
```
./Redis-Analog -h
```

## Options
Usage: ./Redis-Analog [OPTIONS]
```
-h, --help          help message

-f, --file          set dump filename to load in storage

-p, --port          set listening port of server

-m, --max_clients   set maximum of clients in server
```

## Features
Program written with C++ using OOP principles. Storage entity is independent, so it can be used in another project. Also, internal data structure of storage can be easily replaced by anything, that support "StorageInterface". By default, storage uses binary tree structure.

Server works in multithreading, including separated thread for logger (other threads put messages in log queue). New connections initialises additional thread to service client's requests. And main thread of program is is server's main thread that listens on port and accepts new connections.