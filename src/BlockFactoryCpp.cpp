#include "BlockFactoryCpp.h"
#include <stdexcept>
#include "SimulationBlockCpp.h"
#include "spdlog/spdlog.h"
#include "BlockLibrariesPlugingLoader.h"
#include <algorithm>
#include "CppEventHandler.h"
#include "LoggerInstance.h"

namespace BlockTypeSupports::BasicCppSupport
{
    BlockFactoryCpp::BlockFactoryCpp()
    {
        std::unique_ptr<BlockLibrariesPlugingLoader<double>> blockLibrariesPlugingLoaderDouble = std::make_unique<BlockLibrariesPlugingLoader<double>>();
        this->factoryRegistryDouble = blockLibrariesPlugingLoaderDouble->LoadPlugins("/usr/local/lib");

        std::unique_ptr<BlockLibrariesPlugingLoader<std::complex<double>>> blockLibrariesPlugingLoaderComplex = std::make_unique<BlockLibrariesPlugingLoader<std::complex<double>>>();
        this->factoryRegistryComplex = blockLibrariesPlugingLoaderComplex->LoadPlugins("/usr/local/lib");
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
