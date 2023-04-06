# simplekvdb
simplekvdb is a Redis-like key-value store written in C++. Unlike Redis, it is multithreaded and supports true concurrency.  
This project uses C++20 and Cmake. It is written and tested on Ubuntu 20.04 with GCC-11.
  
#### Features:
* Supports `GET`,`SET`, `DEL`, `HSET`, `HGET`, `HDEL`Redis operations
* Optional append-only file (AOF) logging of write commands for persistence
* Loading of AOF commands on key-value store creation

#### Implementation
simplekvdb consists of 3 components: client, server and the simplekvdb static library.  
* The server serves the key-value store via RPC.
* The client is a CLI tool, similar to redis-cli and communicates with the server.
* The simplekvdb static library can be used independently of the client and server in other projects.

#### Dependencies
These external dependencies should be automatically satisfied by CMake.
* [rpclib](https://github.com/rpclib/rpclib)
* [catch2](https://github.com/catchorg/Catch2)
* [fmtlib](https://github.com/fmtlib/fmt)

#### Getting Started
Ensure you have Cmake version of at least 3.14 and a C++20 compiler.
##### Build
Clone the repository and run:  
```mkdir build && cd build```  
```cmake .. && cmake --build .```

##### Run
The kvserver and kvclient binaries can be found in build/ under their respective folders.

Start the kvserver:  
format: `./kvserver <port> <enable logging>`  
`./kvserver 8080 true`  

Start the kvclient:  
format: `./kvclient <server ip> <server port>`  
`./kvclient localhost 8080`

You can now specify redis-cli commands with similar syntax. For eg. for [GET](https://redis.io/commands/get/).

#### Notes
This project was mainly for me to experiment with some modern C++ features and to gain experience writing a moderately-sized C++ project in a linux environment. The following are somethings I have accomplished:
* Use `std::variant`, `std::optional` (C++17)
* Use `fmtlib`, precursor to `std::format` (C++20)
* Concurrency primitives for multithreading: `std::unique_lock`, `std::shared_lock`, `std::condition_variable`, `std::atomic`, and reasoning about synchronization
* Templating in the `Result` class to standardize the Result type
* Unit testing with `catch2`
* Configuring CMake properly
* Timing code with `std::chrono`

#### Design Notes
The main hash table is found in `KvStore.hpp / KvStore.cpp`.
* fixed size (to reduce complexity in terms of resizing). kvserver initializes a KvStore of size 10,000,000.
* Uses chaining to resolve collisions
* Fine-grained locking of buckets (each bucket has a mutex) for concurrency
* atomic operations on insertion (to prevent race)
* Will load from an AOF file for its identifier (if it can be found) on initialization. Each KvStore has an identifier. kvserver always sets it to 1.
* Can specify to either log or not log write operations to construct the AOF file.

The logging is done by `LogWriter.hpp / LogWriter.cpp`.
* Spawns an async thread for disk I/O operations
* Maintains a `vector<char>` as a buffer
* On call to `log`, async thread is notified by condition_variable to flush the buffer and write to file

Parsing and loading of log file by `AofParser, AofLoader`
* Logs are stored in string format for ease of readability. It uses fixed size formatting. For example for the `HSET` command:  
`HSET|XX|KEY|XX|FIELD1|XX|VALUE1|XX|FIELD2|XX|VALUE2`  
where `XX` represents the length of the next element.
