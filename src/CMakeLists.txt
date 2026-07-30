# Define the shared library
add_library(BlockTypeSupportsBasicCppSupport SHARED RegisterBlockFactories.cpp
            BlockFactoryCpp.cpp
            LoggerInstance.cpp
            )

include(FetchContent)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(spdlog REQUIRED)
find_package(yaml-cpp REQUIRED)

add_library(yaml-cpp-static STATIC IMPORTED)

set_target_properties(yaml-cpp-static PROPERTIES
    IMPORTED_LOCATION "/usr/lib64/libyaml-cpp.a"
    INTERFACE_INCLUDE_DIRECTORIES "/usr/include"
)

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)


# -------------------------------------------------------------------
# Eigen3 (system)
# -------------------------------------------------------------------
find_package(Eigen3 REQUIRED NO_MODULE)


target_link_libraries(BlockTypeSupportsBasicCppSupport
    PRIVATE
        PySysLinkBase::PySysLinkBase
        yaml-cpp-static
        spdlog::spdlog_header_only
        fmt::fmt
        Eigen3::Eigen
)

# Include directories for header files
target_include_directories(BlockTypeSupportsBasicCppSupport PUBLIC ${PySysLinkBase_INCLUDE_DIRS})


set_target_properties(BlockTypeSupportsBasicCppSupport PROPERTIES 
    POSITION_INDEPENDENT_CODE ON
    VERSION ${PROJECT_VERSION}
    IMPORTED_LOCATION "${CMAKE_INSTALL_PREFIX}${INSTALLED_DYNAMIC_LIB_NAME}.so"
    OUTPUT_NAME "${INSTALLED_DYNAMIC_LIB_NAME}"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

target_include_directories(BlockTypeSupportsBasicCppSupport
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>  # Include path for the build tree
        $<INSTALL_INTERFACE:include>                   # Include path for the installed tree
)

target_link_options(BlockTypeSupportsBasicCppSupport PRIVATE
    -static-libstdc++
    -static-libgcc
)