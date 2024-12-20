#include "BlockFactoryCpp.h"
#include <stdexcept>
// #include <BlockLibraries/BasicBlocks/Constant.h>
// #include <BlockLibraries/BasicBlocks/Sumator.h>
// #include <BlockLibraries/BasicBlocks/Display.h>
#include "SimulationBlockCpp.h"
#include "spdlog/spdlog.h"
#include "BlockLibrariesPlugingLoader.h"
#include <algorithm>
#include "CppEventHandler.h"

namespace BlockTypeSupports::BasicCppSupport
{
    BlockFactoryCpp::BlockFactoryCpp()
    {
        std::unique_ptr<BlockLibrariesPlugingLoader> blockLibrariesPlugingLoader = std::make_unique<BlockLibrariesPlugingLoader>();
        this->factoryRegistry = blockLibrariesPlugingLoader->LoadPlugins("/usr/local/lib");
    }

    // BlockFactoryCpp::~BlockFactoryCpp()
    // {

    // }

    std::unique_ptr<PySysLinkBase::ISimulationBlock> BlockFactoryCpp::CreateBlock(std::map<std::string, PySysLinkBase::ConfigurationValue> blockConfiguration, std::shared_ptr<PySysLinkBase::IBlockEventsHandler> blockEventsHandler)
    {
        std::string blockClass = PySysLinkBase::ConfigurationValueManager::TryGetConfigurationValue<std::string>("BlockClass", blockConfiguration);

        spdlog::get("default_pysyslink")->debug("{} type block to create...", blockClass);
        

        for (auto const& [key, val] : this->factoryRegistry)
        {
            std::vector<std::string> supportedBlockClasses = val->GetSupportedBlockClasses();
            
            int cnt = std::count(supportedBlockClasses.begin(), supportedBlockClasses.end(), blockClass);
            if (cnt > 0)
            {
                std::shared_ptr<CppEventHandler> cppEventHandler = std::make_shared<CppEventHandler>(blockEventsHandler);
                std::unique_ptr<BlockTypes::BasicCpp::SimulationBlock> simulationBlock = val->CreateBlock(blockClass, blockConfiguration, cppEventHandler);
                return std::make_unique<SimulationBlockCpp>(std::move(simulationBlock), blockConfiguration, blockEventsHandler);
            }
        }

        throw std::out_of_range("Block type with id: " + blockClass + " not found in module BasicCpp.");

    }

} // namespace BlockTypeSupports::BasicCppSupport
