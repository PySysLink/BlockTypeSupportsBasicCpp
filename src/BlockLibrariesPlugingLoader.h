#ifndef BLOCK_TYPE_SUPPORTS_BASIC_CPP_SRC_BLOCK_LIBRARIES_PLUGING_LOADER
#define BLOCK_TYPE_SUPPORTS_BASIC_CPP_SRC_BLOCK_LIBRARIES_PLUGING_LOADER

#include <map>
#include <memory>
#include <string>
#include <dlfcn.h> // For Linux/macOS dynamic linking. Use `windows.h` for Windows.

#include <BlockTypes/BasicCpp/IBasicCppBlockFactory.h>

namespace BlockTypeSupports::BasicCppSupport {

class BlockLibrariesPlugingLoader {
public:
    std::map<std::string, std::unique_ptr<BlockTypes::BasicCpp::IBasicCppBlockFactory>> LoadPlugins(const std::string& pluginDirectory);

private:
    std::vector<std::string> FindSharedLibraries(const std::string& pluginDirectory);
    bool StringEndsWith(const std::string& str, const std::string& suffix);
};

} // namespace BlockTypeSupports::BasicCppSupport

#endif /* BLOCK_TYPE_SUPPORTS_BASIC_CPP_SRC_BLOCK_LIBRARIES_PLUGING_LOADER */
