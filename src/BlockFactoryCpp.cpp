#include "BlockFactoryCpp.h"
#include <stdexcept>
#include "SimulationBlockCpp.h"
#include "spdlog/spdlog.h"
#include "BlockLibrariesPluginLoader.h"
#include <algorithm>
#include "CppEventHandler.h"
#include "LoggerInstance.h"

namespace BlockTypeSupports::BasicCppSupport
{
    BlockFactoryCpp::BlockFactoryCpp()
    {
        std::unique_ptr<BlockLibrariesPluginLoader<double>> blockLibrariesPluginLoaderDouble = std::make_unique<BlockLibrariesPluginLoader<double>>();
        this->factoryRegistryDouble = blockLibrariesPluginLoaderDouble->LoadPlugins("/usr/local/lib");

        std::unique_ptr<BlockLibrariesPluginLoader<std::complex<double>>> blockLibrariesPluginLoaderComplex = std::make_unique<BlockLibrariesPluginLoader<std::complex<double>>>();
        this->factoryRegistryComplex = blockLibrariesPluginLoaderComplex->LoadPlugins("/usr/local/lib");
    }


    std::shared_ptr<PySysLinkBase::ISimulationBlock> BlockFactoryCpp::CreateBlock(std::map<std::string, PySysLinkBase::ConfigurationValue> blockConfiguration, std::shared_ptr<PySysLinkBase::IBlockEventsHandler> blockEventsHandler)
    {
        std::string blockClass = PySysLinkBase::ConfigurationValueManager::TryGetConfigurationValue<std::string>("BlockClass", blockConfiguration);
        
        std::string signalType = "Double";
        try
        {
            signalType = PySysLinkBase::ConfigurationValueManager::TryGetConfigurationValue<std::string>("SignalType", blockConfiguration);
        } catch(std::out_of_range) {}

        LoggerInstance::GetLogger()->debug("{} type block to create with signal type {}...", blockClass, signalType);
        
        if (signalType == "Double")
        {
            return this->CreateBlockFromRegistry<double>(this->factoryRegistryDouble, blockClass, blockConfiguration, blockEventsHandler);
        }
        else if (signalType == "Complex")
        {
            return this->CreateBlockFromRegistry<std::complex<double>>(this->factoryRegistryComplex, blockClass, blockConfiguration, blockEventsHandler);
        }
        else
        {
            throw std::invalid_argument("SignalType: " + signalType + " is not supported for block type BasicCpp");
        }
    }

} // namespace BlockTypeSupports::BasicCppSupport
