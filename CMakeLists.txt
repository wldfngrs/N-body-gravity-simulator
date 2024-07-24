# CMakeList.txt : CMake project for N-BodySim, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("N-BodySim")

# Add source to this project's executable.
add_executable (N-BodySim "main.cpp" "Simulation.h" "Body.h" "Vector2D.h" "Body.cpp" "Simulation.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET N-BodySim PROPERTY CXX_STANDARD 20)
endif()

set(SDL2_DIR SDL2/cmake)
set(SDL2_IMAGE_DIR SDL2_image/cmake)

find_package(SDL2 REQUIRED CONFIG PATHS ${SDL2_DIR})
find_package(SDL2_image REQUIRED CONFIG PATHS ${SDL2_IMAGE_DIR})

include_directories(SDL2/include)
include_directories(SDL2_image/include)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_link_libraries(${PROJECT_NAME} SDL2_image::SDL2_image)