#ifndef SRC_IBASIC_CPP_BLOCK_FACTORY
#define SRC_IBASIC_CPP_BLOCK_FACTORY

#include <memory>
#include <PySysLinkBase/ISimulationBlock.h>
#include <map>
#include <PySysLinkBase/ConfigurationValue.h>
#include <PySysLinkBase/IBlockEventsHandler.h>
#include <string>
#include <vector>

namespace BlockTypeSupports::BasicCppSupport
{
   class IBasicCppBlockFactory {
      public:
         virtual std::vector<std::string> GetSupportedBlockClasses() const = 0;
         virtual std::unique_ptr<PySysLinkBase::ISimulationBlock> CreateBlock(std::string blockClass, std::map<std::string, PySysLinkBase::ConfigurationValue> blockConfiguration, std::shared_ptr<PySysLinkBase::IBlockEventsHandler> blockEventsHandler) = 0;
   };
}

#endif /* SRC_IBASIC_CPP_BLOCK_FACTORY */