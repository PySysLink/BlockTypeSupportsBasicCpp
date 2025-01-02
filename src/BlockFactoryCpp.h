#ifndef SRC_BLOCK_TYPE_SUPPORTS_BASIC_CPP_SUPPORT_BLOCK_FACTORY_CPP
#define SRC_BLOCK_TYPE_SUPPORTS_BASIC_CPP_SUPPORT_BLOCK_FACTORY_CPP

#include <PySysLinkBase/IBlockFactory.h>
#include <PySysLinkBase/IBlockEventsHandler.h>
#include <BlockTypes/BasicCpp/IBasicCppBlockFactory.h>
#include <BlockTypes/BasicCpp/SimulationBlockWithContinuousStates.h>
#include "SimulationBlockCpp.h"
#include "SimulationBlockCppWithContinuousStates.h"
#include "CppEventHandler.h"
#include <stdexcept>

namespace BlockTypeSupports::BasicCppSupport
{
    class BlockFactoryCpp : public PySysLinkBase::IBlockFactory
    {
        private:
            std::map<std::string, std::unique_ptr<BlockTypes::BasicCpp::IBasicCppBlockFactory<double>>> factoryRegistryDouble;
            std::map<std::string, std::unique_ptr<BlockTypes::BasicCpp::IBasicCppBlockFactory<std::complex<double>>>> factoryRegistryComplex;

            template <typename T>
            std::unique_ptr<SimulationBlockCpp<T>> CreateBlockFromRegistry(std::map<std::string, std::unique_ptr<BlockTypes::BasicCpp::IBasicCppBlockFactory<T>>>& registry, std::string blockClass, std::map<std::string, 
                                                                            PySysLinkBase::ConfigurationValue> blockConfiguration, std::shared_ptr<PySysLinkBase::IBlockEventsHandler> blockEventsHandler)
            {
                for (auto const& [key, val] : registry)
                {
                    std::vector<std::string> supportedBlockClasses = val->GetSupportedBlockClasses();
                    
                    int cnt = std::count(supportedBlockClasses.begin(), supportedBlockClasses.end(), blockClass);
                    if (cnt > 0)
                    {
                        std::shared_ptr<CppEventHandler> cppEventHandler = std::make_shared<CppEventHandler>(blockEventsHandler);
                        std::shared_ptr<BlockTypes::BasicCpp::SimulationBlock<T>> simulationBlock = val->CreateBlock(blockClass, blockConfiguration, cppEventHandler);

                        std::shared_ptr<BlockTypes::BasicCpp::SimulationBlockWithContinuousStates<T>> blockWithContinuousStates = std::dynamic_pointer_cast<BlockTypes::BasicCpp::SimulationBlockWithContinuousStates<T>>(simulationBlock);
                        if (blockWithContinuousStates)
                        {
                            return std::make_unique<SimulationBlockCppWithContinuousStates<T>>(std::move(blockWithContinuousStates), blockConfiguration, blockEventsHandler);
                        }
                        else
                        {
                            return std::make_unique<SimulationBlockCpp<T>>(std::move(simulationBlock), blockConfiguration, blockEventsHandler);
                        }
                    }
                }

                throw std::out_of_range("Block type with id: " + blockClass + " not found in module BasicCpp.");
            }
            
        public:
            BlockFactoryCpp();
            std::unique_ptr<PySysLinkBase::ISimulationBlock> CreateBlock(std::map<std::string, PySysLinkBase::ConfigurationValue> blockConfiguration, std::shared_ptr<PySysLinkBase::IBlockEventsHandler> blockEventsHandler);
    };
} // namespace BlockTypeSupports::BasicCppSupport


#endif /* SRC_BLOCK_TYPE_SUPPORTS_BASIC_CPP_SUPPORT_BLOCK_FACTORY_CPP */
