#include "SampleTimeConversion.h"
#include <stdexcept>
#include <iostream>

namespace BlockTypeSupports::BasicCppSupport
{
    const std::shared_ptr<PySysLinkBase::SampleTime> SampleTimeConversion::CppSampleTimeToPySysLink(const std::shared_ptr<BlockTypes::BasicCpp::SampleTime> sampleTime)
    {
        BlockTypes::BasicCpp::SampleTimeType sampleTimeType = sampleTime->GetSampleTimeType();
        if (sampleTimeType == BlockTypes::BasicCpp::SampleTimeType::constant)
        {
            return std::make_shared<PySysLinkBase::SampleTime>(PySysLinkBase::SampleTimeType::constant);
        }
        else if (sampleTimeType == BlockTypes::BasicCpp::SampleTimeType::discrete)
        {
            return std::make_shared<PySysLinkBase::SampleTime>(PySysLinkBase::SampleTimeType::discrete, sampleTime->GetDiscreteSampleTime());
        }
        else if (sampleTimeType == BlockTypes::BasicCpp::SampleTimeType::continuous)
        {
            return std::make_shared<PySysLinkBase::SampleTime>(PySysLinkBase::SampleTimeType::continuous, sampleTime->GetContinuousSampleTimeGroup());
        }
        else if (sampleTimeType == BlockTypes::BasicCpp::SampleTimeType::inherited)
        {
            std::vector<PySysLinkBase::SampleTimeType> supportedSampleTimeTypes = {};
            std::vector<BlockTypes::BasicCpp::SampleTimeType> supportedSampleTimeTypesCpp = sampleTime->GetSupportedSampleTimeTypesForInheritance();
            std::cout << "Sample time account: " << supportedSampleTimeTypesCpp.size() << std::endl;

            for (int i = 0; i < supportedSampleTimeTypesCpp.size(); i++)
            {
                std::cout << "new sample time inher acquired" << std::endl;
                supportedSampleTimeTypes.push_back(SampleTimeConversion::CppSampleTimeTypeToPySysLink(supportedSampleTimeTypesCpp[i]));
            }
            std::cout << "Kaixo hemen nago py" << std::endl;
            return std::make_shared<PySysLinkBase::SampleTime>(PySysLinkBase::SampleTimeType::inherited, supportedSampleTimeTypes);
        }
        else 
        {
            throw std::runtime_error("Unsupported sample time type.");
        }
    }

    const std::shared_ptr<BlockTypes::BasicCpp::SampleTime> SampleTimeConversion::PySysLinkTimeToCpp(const std::shared_ptr<PySysLinkBase::SampleTime> sampleTime)
    {
        PySysLinkBase::SampleTimeType sampleTimeType = sampleTime->GetSampleTimeType();
        if (sampleTimeType == PySysLinkBase::SampleTimeType::constant)
        {
            return std::make_shared<BlockTypes::BasicCpp::SampleTime>(BlockTypes::BasicCpp::SampleTimeType::constant);
        }
        else if (sampleTimeType == PySysLinkBase::SampleTimeType::discrete)
        {
            return std::make_shared<BlockTypes::BasicCpp::SampleTime>(BlockTypes::BasicCpp::SampleTimeType::discrete, sampleTime->GetDiscreteSampleTime());
        }
        else if (sampleTimeType == PySysLinkBase::SampleTimeType::continuous)
        {
            return std::make_shared<BlockTypes::BasicCpp::SampleTime>(BlockTypes::BasicCpp::SampleTimeType::continuous, sampleTime->GetContinuousSampleTimeGroup());
        }
        else if (sampleTimeType == PySysLinkBase::SampleTimeType::inherited)
        {
            std::vector<BlockTypes::BasicCpp::SampleTimeType> supportedSampleTimeTypes = {};
            std::cout << "Kaixo hemen nago" << std::endl;
            std::vector<PySysLinkBase::SampleTimeType> supportedSampleTimeTypesPySysLink = sampleTime->GetSupportedSampleTimeTypesForInheritance();
            std::cout << "Akatxik ez!" << std::endl;
            for (int i = 0; i < supportedSampleTimeTypesPySysLink.size(); i++)
            {
                supportedSampleTimeTypes.push_back(SampleTimeConversion::PySysLinkTimeTypeToCpp(supportedSampleTimeTypesPySysLink[i]));
            }
            return std::make_shared<BlockTypes::BasicCpp::SampleTime>(BlockTypes::BasicCpp::SampleTimeType::inherited, supportedSampleTimeTypes);
        }
        else 
        {
            throw std::runtime_error("Unsupported sample time type.");
        }
    }

    const PySysLinkBase::SampleTimeType SampleTimeConversion::CppSampleTimeTypeToPySysLink(const BlockTypes::BasicCpp::SampleTimeType& sampleTimeType)
    {
        if (sampleTimeType == BlockTypes::BasicCpp::SampleTimeType::constant)
        {
            return PySysLinkBase::SampleTimeType::constant;
        }
        else if (sampleTimeType == BlockTypes::BasicCpp::SampleTimeType::continuous)
        {
            return PySysLinkBase::SampleTimeType::continuous;
        }
        else if (sampleTimeType == BlockTypes::BasicCpp::SampleTimeType::discrete)
        {
            return PySysLinkBase::SampleTimeType::discrete;
        }
        else if (sampleTimeType == BlockTypes::BasicCpp::SampleTimeType::inherited)
        {
            return PySysLinkBase::SampleTimeType::inherited;
        }
        else
        {
            throw std::runtime_error("Unsupported sample time type.");
        }
    }
    
    const BlockTypes::BasicCpp::SampleTimeType SampleTimeConversion::PySysLinkTimeTypeToCpp(const PySysLinkBase::SampleTimeType& sampleTimeType)
    {
        if (sampleTimeType == PySysLinkBase::SampleTimeType::constant)
        {
            return BlockTypes::BasicCpp::SampleTimeType::constant;
        }
        else if (sampleTimeType == PySysLinkBase::SampleTimeType::continuous)
        {
            return BlockTypes::BasicCpp::SampleTimeType::continuous;
        }
        else if (sampleTimeType == PySysLinkBase::SampleTimeType::discrete)
        {
            return BlockTypes::BasicCpp::SampleTimeType::discrete;
        }
        else if (sampleTimeType == PySysLinkBase::SampleTimeType::inherited)
        {
            return BlockTypes::BasicCpp::SampleTimeType::inherited;
        }
        else
        {
            throw std::runtime_error("Unsupported sample time type.");
        }
    }
} // namespace BlockTypeSupports::BasicCppSupport
