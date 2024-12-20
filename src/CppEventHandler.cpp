#include "CppEventHandler.h"
#include <PySysLinkBase/BlockEvents/ValueUpdateBlockEvent.h>

namespace BlockTypeSupports::BasicCppSupport
{
    CppEventHandler::CppEventHandler(std::shared_ptr<PySysLinkBase::IBlockEventsHandler> pySysLinkEventsHandler)
    {
        this->pySysLinkEventsHandler = pySysLinkEventsHandler;
    }

    void CppEventHandler::NotifyNewValueEvent(double simulationTime, std::string valueId, BlockTypes::BasicCpp::ConfigurationValue value)
    {
        this->pySysLinkEventsHandler->BlockEventCallback(std::make_shared<PySysLinkBase::ValueUpdateBlockEvent>(simulationTime, "BasicCppBlock" + valueId, value));
    }
}