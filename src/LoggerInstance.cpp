#include "LoggerInstance.h"

namespace BlockTypeSupports::BasicCppSupport
{
    std::shared_ptr<spdlog::logger> LoggerInstance::s_logger = nullptr;
} // namespace BlockTypeSupports::BasicCppSupport
