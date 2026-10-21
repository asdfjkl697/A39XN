# ============================================================================
#  A39X — GD32F303VG 按摩椅固件  (VS Code + CMake + arm-none-eabi-gcc)
#
#  快速上手:
#     cmake -B build -G "MinGW Makefiles" -DTOOLCHAIN_PREFIX=arm-none-eabi-
#     cmake --build build -j
#  生成物: build/A39X.elf / A39X.hex / A39X.bin
#
#  若 arm-none-eabi-gcc 不在 PATH, 用绝对前缀覆盖:
#     -DTOOLCHAIN_PREFIX=D:/gcc-arm-none-eabi/bin/arm-none-eabi-
# ============================================================================

cmake_minimum_required(VERSION 3.20)

# ---------------------------------------------------------------------------
# 交叉编译工具链 (必须在 project() 之前设置)
# ---------------------------------------------------------------------------
if(NOT TOOLCHAIN_PREFIX)
    set(TOOLCHAIN_PREFIX "arm-none-eabi-")
endif()

set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER       ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER     ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_OBJCOPY          ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_OBJDUMP          ${TOOLCHAIN_PREFIX}objdump)
set(CMAKE_SIZE             ${TOOLCHAIN_PREFIX}size)
# 裸机交叉编译, 禁止 CMake 在本机运行测试程序
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
# 固定输出扩展名, 保证生成 A39X.elf (而非 Windows 下的 A39X.exe)
set(CMAKE_EXECUTABLE_SUFFIX ".elf")

project(A39X LANGUAGES C ASM)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

# ---------------------------------------------------------------------------
# CPU 编译选项
#   -mfloat-abi=soft: 与 Keil 一致 (未定义 __FPU_USED, 不使用硬件浮点)
#   如需硬件浮点: 改为 -mfloat-abi=hard -mfpu=fpv4-sp-d16 并加宏 __FPU_USED=1
# ---------------------------------------------------------------------------
set(CPU_FLAGS -mcpu=cortex-m4 -mthumb -mfloat-abi=soft)

# 2 个宏 (与 Keil 工程 <Define> 完全一致)
add_compile_definitions(
    USE_STDPERIPH_DRIVER
    GD32F30X_XD
)

# ---------------------------------------------------------------------------
# include 路径 (与 Keil 工程 <IncludePath> 一一对应, 并补上 GCC 必需的一项)
# ---------------------------------------------------------------------------
include_directories(
    ${CMAKE_SOURCE_DIR}/App
    ${CMAKE_SOURCE_DIR}/USER
    ${CMAKE_SOURCE_DIR}/Firmware/CMSIS/GD/GD32F30x/Include
    ${CMAKE_SOURCE_DIR}/Firmware/GD32F30x_standard_peripheral/Include
    # 必需: core_cm4.h / core_cmFunc.h 等位于 Firmware/CMSIS/ 下,
    # gd32f30x.h 通过 #include "core_cm4.h" 引用。Keil 靠 RTE 包自动补上,
    # GCC 必须显式添加。
    ${CMAKE_SOURCE_DIR}/Firmware/CMSIS
    # 以下为 Keil 工程中的 USER 子目录路径; 当前源码为扁平结构 (子目录不存在),
    # 保留以与 Keil 工程完全对应, 日后恢复子目录结构也无需改动。
    ${CMAKE_SOURCE_DIR}/USER/Air
    ${CMAKE_SOURCE_DIR}/USER/Heat
    ${CMAKE_SOURCE_DIR}/USER/Motor
    ${CMAKE_SOURCE_DIR}/USER/LED
    ${CMAKE_SOURCE_DIR}/USER/NOI
    ${CMAKE_SOURCE_DIR}/USER/Voice
    ${CMAKE_SOURCE_DIR}/USER/WorkState
    ${CMAKE_SOURCE_DIR}/USER/Bluetooth
    ${CMAKE_SOURCE_DIR}/USER/ArmKey
    ${CMAKE_SOURCE_DIR}/USER/Power
    ${CMAKE_SOURCE_DIR}/USER/Leg
    ${CMAKE_SOURCE_DIR}/USER/485COM
    ${CMAKE_SOURCE_DIR}/USER/FootRoll&CalfKnead
    ${CMAKE_SOURCE_DIR}/USER/Handler
    ${CMAKE_SOURCE_DIR}/USER/Movement
    ${CMAKE_SOURCE_DIR}/USER/Action
    ${CMAKE_SOURCE_DIR}/USER/Pump
    ${CMAKE_SOURCE_DIR}/USER/Memory
    ${CMAKE_SOURCE_DIR}/USER/Health
)

# ---------------------------------------------------------------------------
# 自动收集 App/ USER/ Firmware 下所有 .c 源文件
# (注意: 不 glob *.s —— 仓库里的 ARM/IAR 启动文件是 Keil/IAR 语法, GCC 用不了;
#  下面的 GNU 启动文件单独加入)
# ---------------------------------------------------------------------------
file(GLOB_RECURSE APP_SOURCES
    ${CMAKE_SOURCE_DIR}/App/*.c
    ${CMAKE_SOURCE_DIR}/USER/*.c
    ${CMAKE_SOURCE_DIR}/Firmware/*.c
)

# GNU 汇编语法启动文件 (由 Keil 版转写, 见同目录注释)
set(STARTUP_FILE
    ${CMAKE_SOURCE_DIR}/Firmware/CMSIS/GD/GD32F30x/Source/GCC/startup_gd32f30x_hd.S)

# ---------------------------------------------------------------------------
# 目标
# ---------------------------------------------------------------------------
add_executable(${PROJECT_NAME} ${APP_SOURCES} ${STARTUP_FILE})

# Re-link if the boot image or merge script changes, so the combined HEX is refreshed.
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY LINK_DEPENDS
    ${CMAKE_SOURCE_DIR}/tools/A390-BOOTLOADER.hex
    ${CMAKE_SOURCE_DIR}/tools/merge_hex.py
)

# 固定输出为 A39X.elf (CMAKE_EXECUTABLE_SUFFIX 会被 project() 按 Generic 平台重置,
# 这里用目标级 SUFFIX 属性保证生成 A39X.elf 而非 A39X)
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".elf")

target_compile_options(${PROJECT_NAME} PRIVATE
    ${CPU_FLAGS}
    -std=gnu11
    -ffunction-sections
    -fdata-sections
    -Wall
    -Wextra
    -gdwarf-2            # 兼容 J-Link GDB
    -Og                  # 便于调试; 量产可改为 -O2 或 -Os
)

target_link_options(${PROJECT_NAME} PRIVATE
    ${CPU_FLAGS}
    --specs=nano.specs   # newlib-nano (更小体积)
    --specs=nosys.specs  # 提供 _write/_sbrk 等系统调用桩
    -Wl,--gc-sections    # 丢弃未引用段 (未用外设驱动会被裁掉)
    -Wl,-Map=${PROJECT_NAME}.map
    -T${CMAKE_SOURCE_DIR}/GD32F303VG.ld
)

# ---------------------------------------------------------------------------
# 生成 .hex / .bin 并打印段体积
# ---------------------------------------------------------------------------
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex   $<TARGET_FILE:${PROJECT_NAME}>
            ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/merge_hex.py
            ${CMAKE_SOURCE_DIR}/tools/A390-BOOTLOADER.hex
            ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}>
            ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE}             $<TARGET_FILE:${PROJECT_NAME}>
)
