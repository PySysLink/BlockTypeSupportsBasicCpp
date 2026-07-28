# Define the shared library
add_library(BlockTypeSupportsBasicCppSupport SHARED RegisterBlockFactories.cpp
            BlockFactoryCpp.cpp
            LoggerInstance.cpp
            )

include(FetchContent)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)

add_dependencies(BlockTypeSupportsBasicCppSupport spdlog::spdlog)
target_link_libraries(BlockTypeSupportsBasicCppSupport PRIVATE spdlog::spdlog)

set(YAML_CPP_STATIC_LIBS TRUE)
find_package(yaml-cpp REQUIRED)

target_link_libraries(BlockTypeSupportsBasicCppSupport PRIVATE PySysLinkBase::PySysLinkBase)
target_link_libraries(BlockTypeSupportsBasicCppSupport PRIVATE yaml-cpp)

# -------------------------------------------------------------------
# Eigen3 (system)
# -------------------------------------------------------------------
find_package(Eigen3 REQUIRED NO_MODULE)
target_link_libraries(BlockTypeSupportsBasicCppSupport PRIVATE Eigen3::Eigen)


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
