#include "BlockFactoryCpp.h"
#include <stdexcept>
#include "SimulationBlockCpp.h"
#include "spdlog/spdlog.h"
#include "BlockLibrariesPluginLoader.h"
#include <algorithm>
#include "CppEventHandler.h"
#include "LoggerInstance.h"
#include <PySysLinkBase/PortsAndSignalValues/PortTypeMetadata.h>

namespace BlockTypeSupports::BasicCppSupport
{
    BlockFactoryCpp::BlockFactoryCpp(std::map<std::string, PySysLinkBase::ConfigurationValue> pluginConfiguration)
    {
        std::string libraryPluginPath = PySysLinkBase::ConfigurationValueManager::TryGetConfigurationValue<std::string>("BasicCppSupport/libraryPluginPath", pluginConfiguration);

        std::unique_ptr<BlockLibrariesPluginLoader<double>> blockLibrariesPluginLoaderDouble = std::make_unique<BlockLibrariesPluginLoader<double>>();
        this->factoryRegistryDouble = blockLibrariesPluginLoaderDouble->LoadPlugins(libraryPluginPath);

        std::unique_ptr<BlockLibrariesPluginLoader<std::complex<double>>> blockLibrariesPluginLoaderComplex = std::make_unique<BlockLibrariesPluginLoader<std::complex<double>>>();
        this->factoryRegistryComplex = blockLibrariesPluginLoaderComplex->LoadPlugins(libraryPluginPath);

        std::unique_ptr<BlockLibrariesPluginLoader<int>> blockLibrariesPluginLoaderInt = std::make_unique<BlockLibrariesPluginLoader<int>>();
        this->factoryRegistryInt = blockLibrariesPluginLoaderInt->LoadPlugins(libraryPluginPath);

        std::unique_ptr<BlockLibrariesPluginLoader<bool>> blockLibrariesPluginLoaderBool = std::make_unique<BlockLibrariesPluginLoader<bool>>();
        this->factoryRegistryBool = blockLibrariesPluginLoaderBool->LoadPlugins(libraryPluginPath);

        std::unique_ptr<BlockLibrariesPluginLoader<std::string>> blockLibrariesPluginLoaderString = std::make_unique<BlockLibrariesPluginLoader<std::string>>();
        this->factoryRegistryString = blockLibrariesPluginLoaderString->LoadPlugins(libraryPluginPath);
    }


    std::shared_ptr<PySysLinkBase::ISimulationBlock> BlockFactoryCpp::CreateBlock(std::map<std::string, PySysLinkBase::ConfigurationValue> blockConfiguration, std::shared_ptr<PySysLinkBase::IBlockEventsHandler> blockEventsHandler)
    {
        std::string blockClass = PySysLinkBase::ConfigurationValueManager::TryGetConfigurationValue<std::string>("BlockClass", blockConfiguration);
        
        std::string signalValueType = "double";

        int inputPortNumber = PySysLinkBase::ConfigurationValueManager::TryGetConfigurationValue<int>("InputPortNumber", blockConfiguration);

        if (inputPortNumber > 0) {
            std::vector<PySysLinkBase::PortTypeMetadata> inputPortTypeMetadataVector = PySysLinkBase::ConfigurationValueManager::TryGetConfigurationValue<std::vector<PySysLinkBase::PortTypeMetadata>>("InputPortTypeMetadata", blockConfiguration);
            if (!inputPortTypeMetadataVector.empty()) {
                if (inputPortTypeMetadataVector[0].signalValueType.has_value()) {
                    signalValueType = inputPortTypeMetadataVector[0].signalValueType.value();
                }
            }
        }
        else
        {
            int outputPortNumber = PySysLinkBase::ConfigurationValueManager::TryGetConfigurationValue<int>("OutputPortNumber", blockConfiguration);
            if (outputPortNumber > 0) {
                std::vector<PySysLinkBase::PortTypeMetadata> outputPortTypeMetadataVector = PySysLinkBase::ConfigurationValueManager::TryGetConfigurationValue<std::vector<PySysLinkBase::PortTypeMetadata>>("OutputPortTypeMetadata", blockConfiguration);
                if (!outputPortTypeMetadataVector.empty()) {
                    if (outputPortTypeMetadataVector[0].signalValueType.has_value()) {
                        signalValueType = outputPortTypeMetadataVector[0].signalValueType.value();
                    }
                }
            }
        }

        LoggerInstance::GetLogger()->debug("{} type block to create with signal type {}...", blockClass, signalValueType);
        
        if (signalValueType == "double")
        {
            return this->CreateBlockFromRegistry<double>(this->factoryRegistryDouble, blockClass, blockConfiguration, blockEventsHandler);
        }
        else if (signalValueType == "complex_double")
        {
            return this->CreateBlockFromRegistry<std::complex<double>>(this->factoryRegistryComplex, blockClass, blockConfiguration, blockEventsHandler);
        }
        else if (signalValueType == "int")
        {
            return this->CreateBlockFromRegistry<int>(this->factoryRegistryInt, blockClass, blockConfiguration, blockEventsHandler);
        }
        else if (signalValueType == "bool")
        {
            return this->CreateBlockFromRegistry<bool>(this->factoryRegistryBool, blockClass, blockConfiguration, blockEventsHandler);
        }
        else if (signalValueType == "string")
        {
            return this->CreateBlockFromRegistry<std::string>(this->factoryRegistryString, blockClass, blockConfiguration, blockEventsHandler);
        }
        else
        {
            throw std::invalid_argument("signalValueType: " + signalValueType + " is not supported for block type BasicCpp");
        }
    }

} // namespace BlockTypeSupports::BasicCppSupport
