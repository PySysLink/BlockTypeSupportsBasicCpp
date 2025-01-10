#ifndef SRC_SIMULATION_BLOCK_CPP_WITH_CONTINUOUS_STATES
#define SRC_SIMULATION_BLOCK_CPP_WITH_CONTINUOUS_STATES

#include <PySysLinkBase/ContinuousAndOde/ISimulationBlockWithContinuousStates.h>
#include "SimulationBlockCpp.h"

namespace BlockTypeSupports::BasicCppSupport
{
    template <typename T>
    class SimulationBlockCppWithContinuousStates : public SimulationBlockCpp<T>, public PySysLinkBase::ISimulationBlockWithContinuousStates
    {
        private:
            std::shared_ptr<BlockTypes::BasicCpp::SimulationBlockWithContinuousStates<T>> simulationBlockWithContinuousStates;
        public:
            SimulationBlockCppWithContinuousStates(std::shared_ptr<BlockTypes::BasicCpp::SimulationBlockWithContinuousStates<T>> simulationBlock, 
                                                    std::map<std::string, PySysLinkBase::ConfigurationValue> blockConfiguration, 
                                                    std::shared_ptr<PySysLinkBase::IBlockEventsHandler> blockEventsHandler) 
                                                    : simulationBlockWithContinuousStates(simulationBlock),
                                                    SimulationBlockCpp<T>(simulationBlock, blockConfiguration, blockEventsHandler),
                                                    PySysLinkBase::ISimulationBlockWithContinuousStates(blockConfiguration, blockEventsHandler),
                                                    PySysLinkBase::ISimulationBlock(blockConfiguration, blockEventsHandler)
            {
                
            }

            const std::vector<double> GetContinuousStates() const
            {
                return this->simulationBlockWithContinuousStates->GetContinuousStates();
            }
            void SetContinuousStates(std::vector<double> newStates)
            {
                this->simulationBlockWithContinuousStates->SetContinuousStates(newStates);
            }

            const std::vector<double> GetContinousStateDerivatives(const std::shared_ptr<PySysLinkBase::SampleTime> sampleTime, double currentTime) const
            {
                std::vector<T> inputValues = this->GetInputValues();
                return this->simulationBlockWithContinuousStates->GetContinousStateDerivatives(inputValues, SampleTimeConversion::PySysLinkTimeToCpp(sampleTime), currentTime);
            }
            const std::vector<std::vector<double>> GetContinuousStateJacobians(const std::shared_ptr<PySysLinkBase::SampleTime> sampleTime, double currentTime) const
            {
                std::vector<T> inputValues = this->GetInputValues();
                return this->simulationBlockWithContinuousStates->GetContinuousStateJacobians(inputValues, SampleTimeConversion::PySysLinkTimeToCpp(sampleTime), currentTime);
            }

            const std::vector<double> GetEvents(const std::shared_ptr<PySysLinkBase::SampleTime> sampleTime, double currentTime) const
            {
                std::vector<T> inputValues = this->GetInputValues();
                return this->simulationBlockWithContinuousStates->GetEvents(inputValues, SampleTimeConversion::PySysLinkTimeToCpp(sampleTime), currentTime);
            }

            using SimulationBlockCpp<T>::GetSampleTime;
            using SimulationBlockCpp<T>::SetSampleTime;
            using SimulationBlockCpp<T>::GetInputPorts;
            using SimulationBlockCpp<T>::GetOutputPorts;
            using SimulationBlockCpp<T>::ComputeOutputsOfBlock;
            using SimulationBlockCpp<T>::TryUpdateConfigurationValue;
    };
}

#endif /* SRC_SIMULATION_BLOCK_CPP_WITH_CONTINUOUS_STATES */
