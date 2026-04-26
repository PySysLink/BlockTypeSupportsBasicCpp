#ifndef SRC_BLOCK_TYPE_SUPPORTS_BASIC_CPP_SUPPORT_BLOCK_FACTORY_CPP
#define SRC_BLOCK_TYPE_SUPPORTS_BASIC_CPP_SUPPORT_BLOCK_FACTORY_CPP

#include <PySysLinkBase/IBlockFactory.h>
#include <PySysLinkBase/IBlockEventsHandler.h>
#include <BlockTypes/BasicCpp/IBasicCppBlockFactory.h>
#include <BlockTypes/BasicCpp/SimulationBlockWithContinuousStates.h>
#include "SimulationBlockCpp.h"
#include "SimulationBlockCppWithContinuousStates.h"
#include "IBasicCppBlockFactory.h"
#include <stdexcept>
#include <algorithm> 

namespace BlockTypeSupports::BasicCppSupport
{
    class BlockFactoryCpp : public PySysLinkBase::IBlockFactory
    {
        private:
            std::map<std::string, std::unique_ptr<IBasicCppBlockFactory>> factoryRegistry;

            std::shared_ptr<PySysLinkBase::ISimulationBlock> CreateBlockFromRegistry(std::map<std::string, std::unique_ptr<IBasicCppBlockFactory>>& registry, std::string blockClass, std::map<std::string, 
                                                                            PySysLinkBase::ConfigurationValue> blockConfiguration, std::shared_ptr<PySysLinkBase::IBlockEventsHandler> blockEventsHandler)
            {
                for (auto const& [key, val] : registry)
                {
                    std::vector<std::string> supportedBlockClasses = val->GetSupportedBlockClasses();
                    
                    int cnt = std::count(supportedBlockClasses.begin(), supportedBlockClasses.end(), blockClass);
                    if (cnt > 0)
                    {
                        return val->CreateBlock(blockClass, blockConfiguration, blockEventsHandler);
                    }
                }

                throw std::out_of_range("Block type with id: " + blockClass + " not found in module BasicCpp.");
            }
            
        public:
            BlockFactoryCpp(std::map<std::string, PySysLinkBase::ConfigurationValue> pluginConfiguration);
            std::shared_ptr<PySysLinkBase::ISimulationBlock> CreateBlock(std::map<std::string, PySysLinkBase::ConfigurationValue> blockConfiguration, std::shared_ptr<PySysLinkBase::IBlockEventsHandler> blockEventsHandler);
    };
} // namespace BlockTypeSupports::BasicCppSupport


#endif /* SRC_BLOCK_TYPE_SUPPORTS_BASIC_CPP_SUPPORT_BLOCK_FACTORY_CPP */
