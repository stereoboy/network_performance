#ifndef LOGGING_HPP
#define LOGGING_HPP

#ifdef __SPDLOG__

#include <spdlog/spdlog.h>

#ifdef SPDLOG_VER_MAJOR
#if SPDLOG_VERSION > 10000
#include <spdlog/sinks/stdout_color_sinks.h>
#endif //SPDLOG_VERSION > 10000
#endif //SPDLOG_VER_MAJOR

#include <spdlog/fmt/bundled/printf.h>

//
// reference: https://github.com/artemyv/cmake_spdlog_example/blob/printf_like/src/logger.h
//

#define LOG_INFO(format, ...)  \
    console->info(fmt::sprintf(format, ##__VA_ARGS__));

#define LOG_DEBUG(format, ...)  \
    console->debug(fmt::sprintf(format, ##__VA_ARGS__));

#define LOG_WARN(format, ...)  \
    console->warn(fmt::sprintf(format, ##__VA_ARGS__));

#define LOG_ERR(format, ...)  \
    console->error(fmt::sprintf(format, ##__VA_ARGS__));

#else // printf

#include <cstdio>

#define LOG_INFO(fmt, ...)  \
    std::printf("[info] " fmt, ##__VA_ARGS__);

#define LOG_DEBUG(fmt, ...)  \
    std::printf("[debug] " fmt, ##__VA_ARGS__);

#define LOG_WARN(fmt, ...)  \
    std::printf("[warn] " fmt, ##__VA_ARGS__);

#define LOG_ERR(fmt, ...)  \
    std::fprintf(stderr, "[error] " fmt, ##__VA_ARGS__);
#endif

#endif //LOGGING_HPP