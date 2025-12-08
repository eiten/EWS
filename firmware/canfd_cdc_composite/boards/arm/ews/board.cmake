# SPDX-License-Identifier: Apache-2.0

if(CONFIG_BOARD_EWS)
  board_runner_args(jlink "--device=STM32G0B1KB" "--speed=4000")
  board_runner_args(pyocd "--target=stm32g0b1kbux")
  
  include(${ZEPHYR_BASE}/boards/common/pyocd.board.cmake)
  include(${ZEPHYR_BASE}/boards/common/jlink.board.cmake)
  include(${ZEPHYR_BASE}/boards/common/openocd.board.cmake)
endif()