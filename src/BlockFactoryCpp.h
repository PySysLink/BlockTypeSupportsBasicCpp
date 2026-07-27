#ifndef SRC_BLOCK_TYPE_SUPPORTS_BASIC_CPP_SUPPORT_BLOCK_FACTORY_CPP
#define SRC_BLOCK_TYPE_SUPPORTS_BASIC_CPP_SUPPORT_BLOCK_FACTORY_CPP

#include <PySysLinkBase/IBlockFactory.h>
#include <PySysLinkBase/IBlockEventsHandler.h>
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
                                                                            PySysLinkBase::ConfigurationValue> blockConfiguration, std::shared_ptr<PySysLinkBase::IBlockEventsHandler> blockEventsHandler,
                                                                            std::string inputDataType, std::string outputDataType, int inputPortNumber, int outputPortNumber)
            {
                std::vector<std::string> availableBlockClasses;

                for (auto const& [key, val] : registry)
                {
                    std::vector<std::string> supportedBlockClasses = val->GetSupportedBlockClasses();

                    availableBlockClasses.insert(
                        availableBlockClasses.end(),
                        supportedBlockClasses.begin(),
                        supportedBlockClasses.end());
                    
                    int cnt = std::count(supportedBlockClasses.begin(), supportedBlockClasses.end(), blockClass);
                    if (cnt > 0)
                    {
                        return val->CreateBlock(blockClass, blockConfiguration, blockEventsHandler, inputDataType, outputDataType, inputPortNumber, outputPortNumber);
                    }
                }

                std::sort(availableBlockClasses.begin(), availableBlockClasses.end());
                availableBlockClasses.erase(
                    std::unique(availableBlockClasses.begin(), availableBlockClasses.end()),
                    availableBlockClasses.end());

                std::ostringstream oss;
                oss << "Block type '" << blockClass
                    << "' not found in module BasicCpp.\n"
                    << "Available block classes:";

                for (const auto& name : availableBlockClasses)
                    oss << "\n  - " << name;

                throw std::out_of_range(oss.str());
            }
            
        public:
            BlockFactoryCpp(std::map<std::string, PySysLinkBase::ConfigurationValue> pluginConfiguration);
            std::shared_ptr<PySysLinkBase::ISimulationBlock> CreateBlock(std::map<std::string, PySysLinkBase::ConfigurationValue> blockConfiguration, std::shared_ptr<PySysLinkBase::IBlockEventsHandler> blockEventsHandler);
    };
} // namespace BlockTypeSupports::BasicCppSupport


#endif /* SRC_BLOCK_TYPE_SUPPORTS_BASIC_CPP_SUPPORT_BLOCK_FACTORY_CPP */
