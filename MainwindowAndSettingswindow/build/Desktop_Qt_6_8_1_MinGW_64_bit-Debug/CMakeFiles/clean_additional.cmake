# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\MainwindowAndSettingswindow_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\MainwindowAndSettingswindow_autogen.dir\\ParseCache.txt"
  "MainwindowAndSettingswindow_autogen"
  )
endif()
