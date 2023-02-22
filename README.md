
# Visual Concepts Logger

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/674138339377421ba387cd3945621e2b)](https://app.codacy.com/gh/AnishEllore/vclogger?utm_source=github.com&utm_medium=referral&utm_content=AnishEllore/vclogger&utm_campaign=Badge_Grade_Settings)
[![CodeFactor](https://www.codefactor.io/repository/github/anishellore/vclogger/badge)](https://www.codefactor.io/repository/github/anishellore/vclogger)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/81628150fc744467b8df864879318137)](https://www.codacy.com/gh/AnishEllore/vclogger/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=AnishEllore/vclogger&amp;utm_campaign=Badge_Grade)

## Features

1. Thread safe logger
2. Supports multiple log levels
3. Supports custom sinks
4. Supports appending multiple sinks to the logger
5. Supports custom loggers
6. Supports setting runtime log levels
7. Well documented code
8. Can easily extend to support file rotation

### Motivation

1. Write a simple logger which can be used to log messages in a multi threaded environment and demonstrate my understanding of C++.
2. Write a logger which is extendable and can be used in production code bases.
3. Learn about different design patterns and their use cases.
4. Enjoy the process of writing code and learning new things.

### My Experience

Initially I thought of implementing the logger with singleton design pattern but later I realized that it is not a good idea to use singleton in production code bases. So I used dependency injection to write my logger and corresponding sink. It was very fun to write the logger and sink using dependency injection. I'm very happy with the way this project turned out! Thanks Visual Concepts team for making me write this logger!

### Design Choices

1. Singleton vs Dependency Injection. Singleton seems like a bad idea for production code bases and has some limitations. So I used dependency injection to write my logging module and corresponding user oddeven service
2. Write policies: I have demonstarted two custom sink write policies in my project
    - Non blocking lazy writing using two queues in consoleSink
    - Synchronous Blocking in fileSink. (Writing to file is faster than console so I chose these policies for the corresponding sinks
    - (TODO) Asynchronous writing using a thread pool.
If the target I/O (Disk, Remote Storage, Network) is very fast we can go ahead with blocking but if writing to our target is slow we shouldn't block applications for logging. So in that case lazy writing gives optimal performance as the user applications are no longer blocked by I/O.
3. Need better string handling. I have used std::string for now. But I need to use a better string library with less copying and better performance.

### Build Instructions

Tested on Mac OS 13.2.1

```bash
#!/bin/bash
make clean
make run
```

### Example

```cpp
#include <config.hpp>

int main()
{
 // You can update custom loggers and custom sinks in config store
 std::shared_ptr<IVCLogger> logger = Config().getLogger();
 logger->log(VCLogLevel::VCInfo, "First good log");

 return 0;
}
```

### Directory Structure

```text
 .
 ├── bin     # Compiled files (alternatively `dist`)
 ├── src     # Source files application logic oddeven service
 ├── test    # [TODO] Unit Testing. For now I have written a simple test program
 ├── include    # Header files for logger
 ├── include/interface   # Interface files for logger and sink
 ├── include/sinks   # Custom sink implementations
 ├── include/loggers   # Custom logger implementations
 ├── include/config.hpp   # Helps to modify logger configurations and keep source application code clean
 ├── README.md    # This file
 ├── vcloggerdemo.log   # Demo Log file generated by fileSink
```
