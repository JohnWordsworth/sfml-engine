###########################
## Simple SFML Engine
###########################

set(LIBRARY "sfml-engine")

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "source/*.cpp")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "include/*.h")

file(GLOB_RECURSE NLOHMANN_JSON_FILES CONFIGURE_DEPENDS "libs/nlohmann/*.hpp")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/libs/" PREFIX "Libraries" FILES ${NLOHMANN_JSON_FILES})

add_library(${LIBRARY} STATIC ${SOURCE_FILES} ${HEADER_FILES} ${NLOHMANN_JSON_FILES})
target_include_directories(${LIBRARY} PUBLIC "include" "libs/")
target_include_directories(${LIBRARY} PRIVATE source)

## SFML

find_package(SFML 2 REQUIRED network audio graphics window system)

if (NOT SFML_FOUND)
	message(ERROR "SFML not found. Did you install the vcpkg and set -DCMAKE_TOOLCHAIN_FILE to the correct path?")
else()
	target_link_libraries(${LIBRARY} PUBLIC sfml-network sfml-audio sfml-graphics sfml-window sfml-system)
endif()


## Box2D

find_package(unofficial-box2d CONFIG REQUIRED)
target_link_libraries(${LIBRARY} PUBLIC unofficial::box2d::box2d)

