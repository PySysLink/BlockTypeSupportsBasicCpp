#include <PySysLinkBase/IBlockFactory.h>
#include "BlockFactoryCpp.h"
#include "spdlog/spdlog.h"

extern "C" void RegisterBlockFactories(std::map<std::string, std::unique_ptr<PySysLinkBase::IBlockFactory>>& registry) {
    registry["BasicCpp"] = std::make_unique<BlockTypeSupports::BasicCppSupport::BlockFactoryCpp>();
}

extern "C" void RegisterSpdlogLogger(std::shared_ptr<spdlog::logger> logger) {
    spdlog::get("default_pysyslink")->debug("Logger from pluging BlockTypeSupportsBasicCpp!");
}
