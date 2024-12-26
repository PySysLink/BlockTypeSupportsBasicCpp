# Define the shared library
add_library(BlockTypeSupportsBasicCppSupport SHARED RegisterBlockFactories.cpp
            SampleTimeConversion.cpp
            BlockFactoryCpp.cpp
            RegisterBlockFactories.cpp
            CppEventHandler.cpp)

include(FetchContent)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)


FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.15.0
)

FetchContent_MakeAvailable(spdlog)

add_dependencies(BlockTypeSupportsBasicCppSupport spdlog::spdlog)
target_link_libraries(BlockTypeSupportsBasicCppSupport PRIVATE spdlog::spdlog)



target_link_libraries(BlockTypeSupportsBasicCppSupport PRIVATE PySysLinkBase)
target_link_libraries(BlockTypeSupportsBasicCppSupport PRIVATE BlockTypesBasicCpp)
# target_link_libraries(BasicCppSupport PRIVATE BasicBlocks)

# Include directories for header files
target_include_directories(BlockTypeSupportsBasicCppSupport PUBLIC ${PySysLinkBase_INCLUDE_DIRS})
target_include_directories(BlockTypeSupportsBasicCppSupport PUBLIC ${BlockTypesBasicCpp_INCLUDE_DIRS})

# target_include_directories(BlockTypesBasicCpp
#     PUBLIC
#         $<BUILD_INTERFACE:${PySysLinkBase_INCLUDE_DIRS}>  # Include path for the build tree
#         $<INSTALL_INTERFACE:include>                   # Include path for the installed tree
# )
# target_include_directories(BlockTypesBasicCpp
#     PUBLIC
#         $<BUILD_INTERFACE:${BlockTypesBasicCpp_INCLUDE_DIRS}>  # Include path for the build tree
#         $<INSTALL_INTERFACE:include>                   # Include path for the installed tree
# )
target_include_directories(BlockTypeSupportsBasicCppSupport
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>  # Include path for the build tree
        $<INSTALL_INTERFACE:include>                   # Include path for the installed tree
)
