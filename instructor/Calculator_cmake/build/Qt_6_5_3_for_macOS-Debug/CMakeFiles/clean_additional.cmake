# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/CalculatorGuiTests_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/CalculatorGuiTests_autogen.dir/ParseCache.txt"
  "CMakeFiles/Calculator_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Calculator_autogen.dir/ParseCache.txt"
  "CalculatorGuiTests_autogen"
  "Calculator_autogen"
  )
endif()
