#ifndef SRC_SIMULATION_BLOCK_CPP
#define SRC_SIMULATION_BLOCK_CPP

#include <string>
#include <vector>
#include <memory>
#include <PySysLinkBase/PortsAndSignalValues/InputPort.h>
#include <PySysLinkBase/PortsAndSignalValues/OutputPort.h>
#include <PySysLinkBase/PortsAndSignalValues/SignalValue.h>
#include <PySysLinkBase/SampleTime.h>
#include <PySysLinkBase/ISimulationBlock.h>
#include <PySysLinkBase/IBlockEventsHandler.h>
#include <BlockTypes/BasicCpp/SimulationBlock.h>
#include <BlockTypes/BasicCpp/SampleTime.h>
#include <spdlog/spdlog.h>

#include "SampleTimeConversion.h"
#include "LoggerInstance.h"
#include "SimulationBlockCppCommons.h"


namespace BlockTypeSupports::BasicCppSupport
{
    template <typename T>
    class SimulationBlockCpp : public PySysLinkBase::ISimulationBlock {
        protected:
            std::shared_ptr<SimulationBlockCppCommons<T>> simulationBlockCppCommons;

        public:
            SimulationBlockCpp(std::shared_ptr<BlockTypes::BasicCpp::SimulationBlock<T>> simulationBlock, 
                                std::map<std::string, PySysLinkBase::ConfigurationValue> blockConfiguration, 
                                std::shared_ptr<PySysLinkBase::IBlockEventsHandler> blockEventsHandler) 
                                : ISimulationBlock(blockConfiguration, blockEventsHandler) 
            {
                this->simulationBlockCppCommons = std::make_shared<SimulationBlockCppCommons<T>>(simulationBlock, blockConfiguration, blockEventsHandler);
            }


            const std::shared_ptr<PySysLinkBase::SampleTime> GetSampleTime() const override
            {
                return this->simulationBlockCppCommons->GetSampleTime();
            }
            
            void SetSampleTime(std::shared_ptr<PySysLinkBase::SampleTime> sampleTime)
            {
                this->simulationBlockCppCommons->SetSampleTime(sampleTime);
            }


            std::vector<std::shared_ptr<PySysLinkBase::InputPort>> GetInputPorts() const
            {
                return this->simulationBlockCppCommons->GetInputPorts();
            }
                    
            const std::vector<std::shared_ptr<PySysLinkBase::OutputPort>> GetOutputPorts() const
            {
                return this->simulationBlockCppCommons->GetOutputPorts();
            }

            const std::vector<std::shared_ptr<PySysLinkBase::OutputPort>> _ComputeOutputsOfBlock(const std::shared_ptr<PySysLinkBase::SampleTime> sampleTime, double currentTime, bool isMinorStep=false)
            {
                return this->simulationBlockCppCommons->_ComputeOutputsOfBlock(sampleTime, currentTime, isMinorStep);

            }

            bool _TryUpdateConfigurationValue(std::string keyName, PySysLinkBase::ConfigurationValue value)
            {
                return this->simulationBlockCppCommons->TryUpdateConfigurationValue(keyName, value);
            }

            const std::vector<std::pair<double, double>> GetEvents(const std::shared_ptr<PySysLinkBase::SampleTime> sampleTime, double eventTime, std::vector<double> eventTimeStates, bool includeKnownEvents=false) const override
            {
                return this->simulationBlockCppCommons->GetEvents(sampleTime, eventTime, eventTimeStates, includeKnownEvents);
            }

            const std::vector<double> GetKnownEvents(const std::shared_ptr<PySysLinkBase::SampleTime> resolvedSampleTime, double simulationStartTime, double simulationEndTime) const override
            {
                return this->simulationBlockCppCommons->GetKnownEvents(resolvedSampleTime, simulationStartTime, simulationEndTime);
            }
    };
}

#endif /* SRC_SIMULATION_BLOCK_CPP */
