#pragma once
#define DBGOUT 1
#define PRINTFPS 1
#include <stdio.h>
#include <chrono>
#include <thread>
#define LOG(...) { /*printf("%s:%d: ", __FILE__, __LINE__);*/ printf(__VA_ARGS__); puts(""); }
#define FAIL(...) { LOG(__VA_ARGS__); exit(1); }
#define sleep(n) std::this_thread::sleep_for(std::chrono::milliseconds(n));
#define align(offset, alignment) (((offset + alignment - 1) / alignment) * alignment)