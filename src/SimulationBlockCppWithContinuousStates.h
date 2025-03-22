#ifndef SRC_SIMULATION_BLOCK_CPP_WITH_CONTINUOUS_STATES
#define SRC_SIMULATION_BLOCK_CPP_WITH_CONTINUOUS_STATES

#include <PySysLinkBase/ContinuousAndOde/ISimulationBlockWithContinuousStates.h>
#include "SimulationBlockCpp.h"

namespace BlockTypeSupports::BasicCppSupport
{
    template <typename T>
    class SimulationBlockCppWithContinuousStates : public PySysLinkBase::ISimulationBlockWithContinuousStates
    {
        private:
            std::shared_ptr<BlockTypes::BasicCpp::SimulationBlockWithContinuousStates<T>> simulationBlockWithContinuousStates;

        protected:
        std::shared_ptr<SimulationBlockCppCommons<T>> simulationBlockCppCommons;

        public:

            SimulationBlockCppWithContinuousStates(std::shared_ptr<BlockTypes::BasicCpp::SimulationBlockWithContinuousStates<T>> simulationBlock, 
                                                    std::map<std::string, PySysLinkBase::ConfigurationValue> blockConfiguration, 
                                                    std::shared_ptr<PySysLinkBase::IBlockEventsHandler> blockEventsHandler) 
                        : simulationBlockWithContinuousStates(simulationBlock),
                        PySysLinkBase::ISimulationBlockWithContinuousStates(blockConfiguration, blockEventsHandler)
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

            bool TryUpdateConfigurationValue(std::string keyName, PySysLinkBase::ConfigurationValue value)
            {
                return this->simulationBlockCppCommons->TryUpdateConfigurationValue(keyName, value);
            }

            const std::vector<std::pair<double, double>> GetEvents(const std::shared_ptr<PySysLinkBase::SampleTime> sampleTime, double eventTime, std::vector<double> eventTimeStates) const override
            {
                return this->simulationBlockCppCommons->GetEvents(sampleTime, eventTime, eventTimeStates);
            }

            const std::vector<double> GetKnownEvents(const std::shared_ptr<PySysLinkBase::SampleTime> resolvedSampleTime, double simulationStartTime, double simulationEndTime) const override
            {
                return this->simulationBlockCppCommons->GetKnownEvents(resolvedSampleTime, simulationStartTime, simulationEndTime);
            }
            

            const std::vector<double> GetContinuousStates() const
            {
                return this->simulationBlockWithContinuousStates->GetContinuousStates();
            }
            void SetContinuousStates(std::vector<double> newStates)
            {
                this->simulationBlockWithContinuousStates->SetContinuousStates(newStates);
            }

            const std::vector<double> GetContinuousStateDerivatives(const std::shared_ptr<PySysLinkBase::SampleTime> sampleTime, double currentTime) const
            {
                std::vector<T> inputValues = this->simulationBlockCppCommons->GetInputValues();
                return this->simulationBlockWithContinuousStates->GetContinuousStateDerivatives(inputValues, SampleTimeConversion::PySysLinkTimeToCpp(sampleTime), currentTime);
            }
            const std::vector<std::vector<double>> GetContinuousStateJacobians(const std::shared_ptr<PySysLinkBase::SampleTime> sampleTime, double currentTime) const
            {
                std::vector<T> inputValues = this->simulationBlockCppCommons->GetInputValues();
                return this->simulationBlockWithContinuousStates->GetContinuousStateJacobians(inputValues, SampleTimeConversion::PySysLinkTimeToCpp(sampleTime), currentTime);
            }
    };
}

#endif /* SRC_SIMULATION_BLOCK_CPP_WITH_CONTINUOUS_STATES */
