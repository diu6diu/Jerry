cmake_minimum_required( VERSION 3.22)

project(LED C ASM)
set(MY_TARGET LED)
set(DEBUG "1")
set(OPT "-Og")
set(CMAKE_VERBOSE_MAKEfILE ON)




#设置执行文件输出目录
#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

message("++++++++++++++Start Build+++++++++++++++++")
#添加头文件目录
include_directories(${PROJECT_SOURCE_DIR}/Core/Inc)
include_directories(${PROJECT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F1xx/Include)
include_directories(${PROJECT_SOURCE_DIR}/Drivers/CMSIS/Include)
include_directories(${PROJECT_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Inc)
include_directories(${PROJECT_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy)

#添加源文件目录
aux_source_directory(${PROJECT_SOURCE_DIR}/Core/Src SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src HALSRC)


# cpu
set(CPU  "-mcpu=cortex-m3")
# fpu
set(FPU  "")
# float-abi
set(FLOAT-ABI  "")
# mcu
set(MCU  "${CPU} -mthumb  ${FPU} ${FLOAT-ABI}")

add_definitions(-DUSE_HAL_DRIVER -DSTM32F103xE)
set(CMAKE_C_FLAGS "${MCU} -fno-builtin ${OPT}  -Wall -std=gnu99 -fdata-sections -ffunction-sections")
set(CMAKE_CXX_FLAGS "${MCU} -fno-builtin ${OPT} -Wall -std=c++11 -fdata-sections -ffunction-sections")
set(CMAKE_ASM_FLAGS "${MCU} -x assembler-with-cpp")

# 取消-rdynamic的错误
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

# 添加动态库
#link_directories("/opt/gccStm32/")
set(LIBS "-lc -lm -lnosys" )

# 添加link script
set(LINKER_SCRIPT "${PROJECT_SOURCE_DIR}/STM32F103ZETx_FLASH.ld")
# 这是LDFLAGS
#set(CMAKE_EXE_LINKER_FLAGS "${MCU} -specs=nano.specs -T${LDSCRIPT} -Wl,--gc-sections")
set(EXTRA_LINK_FLAGS "-Wl,-Map=${PROJECT_NAME}.map,--cref,--no-warn-mismatch -specs=nano.specs -specs=nosys.specs")

# 添加可执行文件（可执行文件名 [配置] 源文件）
add_executable(${MY_TARGET}.elf ${SRC} ${HALSRC} ${PROJECT_SOURCE_DIR}/startup_stm32f103xe.s )
message(STATUS "${CMAKE_OBJCOPY}")

# 执行文件链接属性
target_link_libraries(${MY_TARGET}.elf PUBLIC  -lc -lm -lnosys)
target_link_libraries(${MY_TARGET}.elf PRIVATE -T${LINKER_SCRIPT} ${EXTRA_LINK_FLAGS})
#target_link_libraries(${MY_TARGET} ${LIBS})

# 将elf文件转hex和bin
add_custom_command(
    TARGET  ${MY_TARGET}.elf  #当 ${MY_TARGET}.elf被重新生成是执行以下命令
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${MY_TARGET}.elf ${MY_TARGET}.hex)
add_custom_command(
    TARGET  ${MY_TARGET}.elf
    POST_BUILD COMMAND ${CMAKE_OBJCOPY} -O binary ${MY_TARGET}.elf ${MY_TARGET}.bin)

# 显示代码段大小
add_custom_command(
    TARGET  ${MY_TARGET}.elf  #当 ${MY_TARGET}.elf被重新生成是执行以下命令
    POST_BUILD COMMAND ${CMAKE_SIZE}  ${MY_TARGET}.elf )