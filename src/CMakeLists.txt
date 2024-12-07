# Define the shared library
add_library(BlockTypeSupportsBasicCppSupport SHARED RegisterBlockFactories.cpp
            SampleTimeConversion.cpp
            SimulationBlockCpp.cpp
            BlockFactoryCpp.cpp
            BlockLibrariesPlugingLoader.cpp
            RegisterBlockFactories.cpp)

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
