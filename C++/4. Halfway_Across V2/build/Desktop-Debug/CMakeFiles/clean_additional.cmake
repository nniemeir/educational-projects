# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/Halfway_Across_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Halfway_Across_autogen.dir/ParseCache.txt"
  "Halfway_Across_autogen"
  )
endif()
