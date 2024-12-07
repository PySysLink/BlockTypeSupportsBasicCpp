#include "BlockFactoryCpp.h"
#include <stdexcept>
// #include <BlockLibraries/BasicBlocks/Constant.h>
// #include <BlockLibraries/BasicBlocks/Sumator.h>
// #include <BlockLibraries/BasicBlocks/Display.h>
#include "SimulationBlockCpp.h"
#include <iostream>
#include "BlockLibrariesPlugingLoader.h"
#include <algorithm>

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

    std::unique_ptr<PySysLinkBase::ISimulationBlock> BlockFactoryCpp::CreateBlock(std::map<std::string, PySysLinkBase::ConfigurationValue> blockConfiguration)
    {
        std::string blockClass = PySysLinkBase::ConfigurationValueManager::TryGetConfigurationValue<std::string>("BlockClass", blockConfiguration);

        std::cout << blockClass << " type block to create..." << std::endl;
        

        for (auto const& [key, val] : this->factoryRegistry)
        {
            std::vector<std::string> supportedBlockClasses = val->GetSupportedBlockClasses();
            
            int cnt = std::count(supportedBlockClasses.begin(), supportedBlockClasses.end(), blockClass);
            if (cnt > 0)
            {
                std::unique_ptr<BlockTypes::BasicCpp::SimulationBlock> simulationBlock = val->CreateBlock(blockClass, blockConfiguration);
                return std::make_unique<SimulationBlockCpp>(std::move(simulationBlock), blockConfiguration);
            }
        }

        throw std::out_of_range("Block type with id: " + blockClass + " not found in module BasicCpp.");

    }

} // namespace BlockTypeSupports::BasicCppSupport
