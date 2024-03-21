# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\card-match_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\card-match_autogen.dir\\ParseCache.txt"
  "card-match_autogen"
  )
endif()
