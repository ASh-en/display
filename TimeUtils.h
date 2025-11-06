
/**
 * @file TimeUtils.h
 * @brief 跨平台高精度(ms)时间工具，提供系统时间戳与单调时钟及简易计时器封装
 * 
 * @author Ash (MOT)
 * @date 2025-07-04
 * 
 * 功能：
 *  - 获取当前系统时间（自1970年1月1日UTC以来的毫秒数）
 *  - 获取单调时钟时间（适合时间间隔计算，防止系统时间跳变）
 *  - 简易计时器类，支持计时开始、重置及获取经过时间（毫秒）
 * 
 * 设计原则：
 *  - 仅依赖C++11标准库，跨平台（Linux/Windows均适用）
 *  - 轻量、无外部依赖、易于集成与扩展
 *  - 代码内联，便于头文件使用
 * 
 * 用法示例：
 *  @code
 *  #include "TimeUtils.h"
 *  #include <iostream>
 *  #include <thread>
 * 
 *  int main() {
 *      std::cout << "当前系统时间戳(ms): " << TimeUtils::currentTimeMillis() << std::endl;
 *      std::cout << "当前单调时间戳(ms): " << TimeUtils::steadyTimeMillis() << std::endl;
 * 
 *      TimeUtils::Timer timer;
 *      std::this_thread::sleep_for(std::chrono::milliseconds(500));
 *      std::cout << "延时500ms后，计时器记录: " << timer.elapsedMillis() << " ms" << std::endl;
 * 
 *      return 0;
 *  }
 *  @endcode
 */

#pragma once
#include <chrono>
#include <cstdint>
#include <thread>

namespace TimeUtils {

    // 获取当前系统时间（1970年起的毫秒数）
    inline int64_t currentTimeMillis() {
        auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
        return ms.time_since_epoch().count();
    }

    // 获取单调时间（适合计算间隔，避免系统时间跳变）
    inline int64_t steadyTimeMillis() {
        auto now = std::chrono::steady_clock::now();
        auto ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
        return ms.time_since_epoch().count();
    }

    // 简易计时器类
    class Timer {
    public:
        Timer() { reset(); }

        void reset() {
            startPoint = std::chrono::steady_clock::now();
        }

        int64_t elapsedMillis() const {
            auto now = std::chrono::steady_clock::now();
            return std::chrono::duration_cast<std::chrono::milliseconds>(now - startPoint).count();
        }

    private:
        std::chrono::steady_clock::time_point startPoint;
    };
}








