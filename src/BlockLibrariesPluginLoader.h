#ifndef SRC_BLOCK_LIBRARIES_PLUGING_LOADER
#define SRC_BLOCK_LIBRARIES_PLUGING_LOADER

#include <map>
#include <memory>
#include <string>
#include <dlfcn.h> // For Linux/macOS dynamic linking. Use `windows.h` for Windows.
#include <spdlog/spdlog.h>
#include <filesystem>
#include <type_traits>
#include <stdexcept>
#include <complex>
#include <iostream>

#include <BlockTypes/BasicCpp/IBasicCppBlockFactory.h>

namespace BlockTypeSupports::BasicCppSupport {

template <typename T>
    class BlockLibrariesPluginLoader {
        public:
            std::map<std::string, std::unique_ptr<BlockTypes::BasicCpp::IBasicCppBlockFactory<T>>> LoadPlugins(const std::string& pluginDirectory) 
            {
                std::string typeNameExtender;
                if (std::is_same<T, double>::value) 
                {
                    typeNameExtender = "Double";
                }
                else if (std::is_same<T, std::complex<double>>::value) 
                {
                    typeNameExtender = "Complex";
                }
                else
                {
                    throw std::invalid_argument("Template type used does not seem to be supported, unable to get typeNameExtender");
                }
                
                std::cout << "typeNameExtender: " << typeNameExtender << std::endl;

                std::map<std::string, std::unique_ptr<BlockTypes::BasicCpp::IBasicCppBlockFactory<T>>> factoryRegistry;
                
                for (const auto& pluginPath : this->FindSharedLibraries(pluginDirectory)) {
                    std::cout << "Plugin to load: " << pluginPath << std::endl;
                    LoggerInstance::GetLogger()->debug("Trying to load plugin: {}", pluginPath);

                    void* handle = dlopen(pluginPath.c_str(), RTLD_LAZY);
                    if (!handle) {
                        LoggerInstance::GetLogger()->error("Failed to load block library plugin: {}", pluginPath);
                        LoggerInstance::GetLogger()->error(dlerror());
                        continue;
                    }
                    LoggerInstance::GetLogger()->debug("Handle obtained for plugin: {}", pluginPath);
                    std::cout << "Handle obtained for plugin: " << pluginPath << std::endl;

                    auto registerFunc = reinterpret_cast<void(*)(std::map<std::string, std::unique_ptr<BlockTypes::BasicCpp::IBasicCppBlockFactory<T>>>&)>(dlsym(handle, ("RegisterBasicCppFactories" + typeNameExtender).c_str()));
                    LoggerInstance::GetLogger()->debug("registerFunc get from plugin: {}", pluginPath);
                    std::cout << "registerFunc get from plugin: " << pluginPath << std::endl;

                    if (!registerFunc) {
                        LoggerInstance::GetLogger()->error("Failed to find entry point in: {}", pluginPath);
                        LoggerInstance::GetLogger()->error(dlerror());
                        dlclose(handle);
                        continue;
                    }
                    std::cout << "Block library plugin loaded: " << pluginPath << std::endl;

                    LoggerInstance::GetLogger()->debug("Block library plugin loaded: {} with type: {}", pluginPath, typeNameExtender);
                    registerFunc(factoryRegistry);
                }
                return factoryRegistry;
            }

        private:
            std::vector<std::string> FindSharedLibraries(const std::string& searchDirectory) {
                std::vector<std::string> sharedLibraries;

                try {
                    // Recursively iterate over the contents of the search directory
                    for (const auto& entry : std::filesystem::recursive_directory_iterator(searchDirectory)) {
                        if (entry.is_regular_file()) {
                            const std::string filename = entry.path().filename().string();
                            if (filename.find("libBlockLibraries") == 0 && this->StringEndsWith(filename, ".so") && this->StringContains(filename, "BasicCpp")) {
                                sharedLibraries.push_back(entry.path().string());
                            }
                        }
                    }
                } catch (const std::filesystem::filesystem_error& e) {
                    LoggerInstance::GetLogger()->error("Error accessing directory: {}", e.what());
                }

                return sharedLibraries;
            }

            bool StringEndsWith(const std::string& str, const std::string& suffix) {
                return str.size() >= suffix.size() &&
                    str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
            }

            bool StringContains(const std::string& str, const std::string& substring) {
                return str.find(substring) != std::string::npos;
            }
    };

} // namespace BlockTypeSupports::BasicCppSupport

#endif /* SRC_BLOCK_LIBRARIES_PLUGING_LOADER */
