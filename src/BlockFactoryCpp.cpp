#include "BlockFactoryCpp.h"
#include <stdexcept>
#include "SimulationBlockCpp.h"
#include "spdlog/spdlog.h"
#include "BlockLibrariesPluginLoader.h"
#include <algorithm>
#include "LoggerInstance.h"
#include <PySysLinkBase/PortsAndSignalValues/PortTypeMetadata.h>

namespace BlockTypeSupports::BasicCppSupport
{
    BlockFactoryCpp::BlockFactoryCpp(std::map<std::string, PySysLinkBase::ConfigurationValue> pluginConfiguration)
    {
        std::string libraryPluginPath = PySysLinkBase::ConfigurationValueManager::TryGetConfigurationValue<std::string>("BasicCppSupport/libraryPluginPath", pluginConfiguration);

        std::unique_ptr<BlockLibrariesPluginLoader> blockLibrariesPluginLoader = std::make_unique<BlockLibrariesPluginLoader>();
        this->factoryRegistry = blockLibrariesPluginLoader->LoadPlugins(libraryPluginPath);
    }


    std::shared_ptr<PySysLinkBase::ISimulationBlock> BlockFactoryCpp::CreateBlock(std::map<std::string, PySysLinkBase::ConfigurationValue> blockConfiguration, std::shared_ptr<PySysLinkBase::IBlockEventsHandler> blockEventsHandler)
    {
        std::string blockClass = PySysLinkBase::ConfigurationValueManager::TryGetConfigurationValue<std::string>("BlockClass", blockConfiguration);
        

        LoggerInstance::GetLogger()->debug("{} type block to create...", blockClass);
        
        return this->CreateBlockFromRegistry(this->factoryRegistry, blockClass, blockConfiguration, blockEventsHandler);
    }

} // namespace BlockTypeSupports::BasicCppSupport
