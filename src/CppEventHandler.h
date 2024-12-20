#ifndef SRC_CPP_EVENT_HANDLER
#define SRC_CPP_EVENT_HANDLER

#include <BlockTypes/BasicCpp/IEventHandler.h>
#include <PySysLinkBase/IBlockEventsHandler.h>
#include <memory>

namespace BlockTypeSupports::BasicCppSupport
{
    class CppEventHandler : public BlockTypes::BasicCpp::IEventHandler
    {
        private:
        std::shared_ptr<PySysLinkBase::IBlockEventsHandler> pySysLinkEventsHandler;
        public:
        CppEventHandler(std::shared_ptr<PySysLinkBase::IBlockEventsHandler> pySysLinkEventsHandler);

        void NotifyNewValueEvent(double simulationTime, std::string valueId, BlockTypes::BasicCpp::ConfigurationValue value);
    };
} // namespace naBlockTypeSupports::BasicCppSupportme


#endif /* SRC_CPP_EVENT_HANDLER */
