#include <PySysLinkBase/IBlockFactory.h>
#include "BlockFactoryCpp.h"
#include "spdlog/spdlog.h"
#include <iostream>
#include "LoggerInstance.h"

extern "C" void RegisterBlockFactories(std::map<std::string, std::shared_ptr<PySysLinkBase::IBlockFactory>>& registry) {
    std::cout << "Call to RegisterBlockFactories" << std::endl;
    registry["BasicCpp"] = std::make_shared<BlockTypeSupports::BasicCppSupport::BlockFactoryCpp>();
    std::cout << "End of function" << std::endl;
}

extern "C" void RegisterSpdlogLogger(std::shared_ptr<spdlog::logger> logger) {
    BlockTypeSupports::BasicCppSupport::LoggerInstance::SetLogger(logger);
    BlockTypeSupports::BasicCppSupport::LoggerInstance::GetLogger()->debug("Logger from plugin BlockTypeSupportsBasicCpp!");
}
