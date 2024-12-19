#include "BlockLibrariesPlugingLoader.h"
#include <filesystem>
#include "spdlog/spdlog.h"

namespace BlockTypeSupports::BasicCppSupport
{
    std::map<std::string, std::unique_ptr<BlockTypes::BasicCpp::IBasicCppBlockFactory>> BlockLibrariesPlugingLoader::LoadPlugins(const std::string& pluginDirectory) {
        std::map<std::string, std::unique_ptr<BlockTypes::BasicCpp::IBasicCppBlockFactory>> factoryRegistry;
        
        for (const auto& pluginPath : this->FindSharedLibraries(pluginDirectory)) {
            void* handle = dlopen(pluginPath.c_str(), RTLD_LAZY);
            if (!handle) {
                spdlog::get("default_pysyslink")->error("Failed to load block library plugin: {}", pluginPath);
                spdlog::get("default_pysyslink")->error(dlerror());
                continue;
            }

            auto registerFunc = reinterpret_cast<void(*)(std::map<std::string, std::unique_ptr<BlockTypes::BasicCpp::IBasicCppBlockFactory>>&)>(dlsym(handle, "RegisterBasicCppFactories"));

            if (!registerFunc) {
                spdlog::get("default_pysyslink")->error("Failed to find entry point in: {}", pluginPath);
                spdlog::get("default_pysyslink")->error(dlerror());
                dlclose(handle);
                continue;
            }
            spdlog::get("default_pysyslink")->debug("Block library pluging loaded: {}", pluginPath);
            registerFunc(factoryRegistry);
        }
        return factoryRegistry;
    }

    std::vector<std::string> BlockLibrariesPlugingLoader::FindSharedLibraries(const std::string& searchDirectory) {
        std::vector<std::string> sharedLibraries;

        try {
            // Iterate over the contents of the search directory
            for (const auto& entry : std::filesystem::directory_iterator(searchDirectory)) {
                if (entry.is_regular_file()) {
                    // Get the filename
                    const std::string filename = entry.path().filename().string();

                    // Check if the filename matches the desired pattern
                    if (filename.find("libBlockLibraries") == 0 && this->StringEndsWith(filename, "BasicCpp.so")) {
                        sharedLibraries.push_back(entry.path().string());
                    }
                }
            }
        } catch (const std::filesystem::filesystem_error& e) {
            spdlog::get("default_pysyslink")->error("Error accessing directory: ", e.what());
        }

        return sharedLibraries;
    }

    bool BlockLibrariesPlugingLoader::StringEndsWith(const std::string& str, const std::string& suffix) {
        return str.size() >= suffix.size() &&
            str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    }
} // namespace BlockTypeSupports::BasicCppSupport
