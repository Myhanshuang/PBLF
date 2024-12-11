# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\keyrythme_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\keyrythme_autogen.dir\\ParseCache.txt"
  "keyrythme_autogen"
  )
endif()
