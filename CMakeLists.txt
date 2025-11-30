cmake_minimum_required(VERSION 3.20.0)
set(BOARD native_posix CACHE STRING "Board to build for")
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(vitalink)
target_sources(app PRIVATE
  src/main.c
  src/sensors/sensor_driver.c
  src/ai/edge_ai.c
  src/comm/secure_comm.c
  src/power/power_mgmt.c
  src/storage/storage.c
  src/digital_twin/dt_sync.c
  src/alert/alert_manager.c
)
target_include_directories(app PRIVATE include)
