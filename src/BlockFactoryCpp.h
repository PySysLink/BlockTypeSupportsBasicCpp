#ifndef SRC_BLOCK_TYPE_SUPPORTS_BASIC_CPP_SUPPORT_BLOCK_FACTORY_CPP
#define SRC_BLOCK_TYPE_SUPPORTS_BASIC_CPP_SUPPORT_BLOCK_FACTORY_CPP

#include <PySysLinkBase/IBlockFactory.h>
#include <PySysLinkBase/IBlockEventsHandler.h>
#include <BlockTypes/BasicCpp/IBasicCppBlockFactory.h>

namespace BlockTypeSupports::BasicCppSupport
{
    class BlockFactoryCpp : public PySysLinkBase::IBlockFactory
    {
        private:
            std::map<std::string, std::unique_ptr<BlockTypes::BasicCpp::IBasicCppBlockFactory>> factoryRegistry;
        public:
            BlockFactoryCpp();
            std::unique_ptr<PySysLinkBase::ISimulationBlock> CreateBlock(std::map<std::string, PySysLinkBase::ConfigurationValue> blockConfiguration, std::shared_ptr<PySysLinkBase::IBlockEventsHandler> blockEventsHandler);
    };
} // namespace BlockTypeSupports::BasicCppSupport


#endif /* SRC_BLOCK_TYPE_SUPPORTS_BASIC_CPP_SUPPORT_BLOCK_FACTORY_CPP */
