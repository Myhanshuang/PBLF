# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\MainwindowAndSettingsWindow_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\MainwindowAndSettingsWindow_autogen.dir\\ParseCache.txt"
  "MainwindowAndSettingsWindow_autogen"
  )
endif()
