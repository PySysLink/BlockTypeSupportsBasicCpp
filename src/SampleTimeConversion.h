#ifndef SRC_BLOCK_TYPE_SUPPORTS_BASIC_CPP_SUPPORT_SAMPLE_TIME_CONVERSION
#define SRC_BLOCK_TYPE_SUPPORTS_BASIC_CPP_SUPPORT_SAMPLE_TIME_CONVERSION

#include <PySysLinkBase/SampleTime.h>
#include <BlockTypes/BasicCpp/SampleTime.h>
#include <memory>

namespace BlockTypeSupports::BasicCppSupport
{
    class SampleTimeConversion
    /*
    This boilerplate code is necessary, because it can not be ensured for all block type support modules that they will use the same
    sample time types as PySysLinkBase. Since a block type should not have dependency on any PySysLink implementation (e.g. PySysLinkBase),
    it must include its own sample time types definitions, and the support module has to convert them to PySysLinkBase
    */
    {
        public:
        const static std::shared_ptr<PySysLinkBase::SampleTime> CppSampleTimeToPySysLink(const std::shared_ptr<BlockTypes::BasicCpp::SampleTime> sampleTime);
        const static std::shared_ptr<BlockTypes::BasicCpp::SampleTime> PySysLinkTimeToCpp(const std::shared_ptr<PySysLinkBase::SampleTime> sampleTime);
        const static PySysLinkBase::SampleTimeType CppSampleTimeTypeToPySysLink(const BlockTypes::BasicCpp::SampleTimeType& sampleTimeType);
        const static BlockTypes::BasicCpp::SampleTimeType PySysLinkTimeTypeToCpp(const PySysLinkBase::SampleTimeType& sampleTimeType);
    };
} // namespace name


#endif /* SRC_BLOCK_TYPE_SUPPORTS_BASIC_CPP_SUPPORT_SAMPLE_TIME_CONVERSION */
