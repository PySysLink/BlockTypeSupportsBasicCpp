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
#include <spdlog/spdlog.h>

#include "LoggerInstance.h"


namespace BlockTypeSupports::BasicCppSupport
{
    template <typename T>
    class SimulationBlockCpp : public PySysLinkBase::ISimulationBlock {
        protected:
            std::vector<std::shared_ptr<PySysLinkBase::InputPort>> inputPorts;
            std::vector<std::shared_ptr<PySysLinkBase::OutputPort>> outputPorts;

            std::shared_ptr<PySysLinkBase::SampleTime> sampleTime;
            
            std::vector<T> GetInputValues() const
            {
                std::vector<T> inputValues = {};
                for (int i = 0; i < this->inputPorts.size(); i++)
                {
                    auto inputValue = this->inputPorts[i]->GetValue();
                    auto inputValueSignal = inputValue->TryCastToTyped<T>();
                    inputValues.push_back(inputValueSignal->GetPayload());
                }

                return inputValues;
            }

            void SetOutputValues(std::vector<T> outputValues)
            {
                for (int i = 0; i < this->outputPorts.size(); i++)
                {
                    std::shared_ptr<PySysLinkBase::UnknownTypeSignalValue> outputValue = this->outputPorts[i]->GetValue();
                    auto outputValueSignal = outputValue->TryCastToTyped<T>();
                    outputValueSignal->SetPayload(outputValues[i]);
                    this->outputPorts[i]->SetValue(std::make_shared<PySysLinkBase::SignalValue<T>>(*outputValueSignal));
                }
            }
            
        public:
            SimulationBlockCpp(std::map<std::string, PySysLinkBase::ConfigurationValue> blockConfiguration, 
                                std::shared_ptr<PySysLinkBase::IBlockEventsHandler> blockEventsHandler,
                                int inputPortAmount, int outputPortAmount, const std::vector<bool>& inputsHasDirectFeedthrough) 
                                : ISimulationBlock(blockConfiguration, blockEventsHandler) 
            {
                LoggerInstance::GetLogger()->debug("Creating basic simulation block cpp...");

                if (inputsHasDirectFeedthrough.size() != inputPortAmount) {
                    throw std::runtime_error("Mismatch between the number of input ports and the size of the inputsHasDirectFeedthrough vector");
                }
                for (int i = 0; i < inputPortAmount; i++)
                {
                    std::shared_ptr<PySysLinkBase::UnknownTypeSignalValue> signalValue = std::make_shared<PySysLinkBase::SignalValue<T>>(PySysLinkBase::SignalValue<T>(0.0));
                    auto inputPort = std::make_shared<PySysLinkBase::InputPort>(PySysLinkBase::InputPort(inputsHasDirectFeedthrough[i], signalValue));
                    this->inputPorts.push_back(inputPort);
                }
                for (int i = 0; i < outputPortAmount; i++)
                {
                    std::shared_ptr<PySysLinkBase::UnknownTypeSignalValue> signalValue = std::make_shared<PySysLinkBase::SignalValue<T>>(PySysLinkBase::SignalValue<T>(0.0));
                    auto outputPort = std::make_shared<PySysLinkBase::OutputPort>(PySysLinkBase::OutputPort(signalValue));
                    this->outputPorts.push_back(outputPort);
                }

                LoggerInstance::GetLogger()->debug("Ports configured...");               
                LoggerInstance::GetLogger()->debug("Basic simulation block cpp created");
            }

            const virtual std::shared_ptr<PySysLinkBase::SampleTime> GetSampleTime() const { return this->sampleTime; }
            virtual void SetSampleTime(std::shared_ptr<PySysLinkBase::SampleTime> sampleTime) { this->sampleTime = sampleTime; }


            std::vector<std::shared_ptr<PySysLinkBase::InputPort>> GetInputPorts() const
            {
                return this->inputPorts;
            }
                    
            const std::vector<std::shared_ptr<PySysLinkBase::OutputPort>> GetOutputPorts() const
            {
                return this->outputPorts;
            }

            const std::vector<std::shared_ptr<PySysLinkBase::OutputPort>> _ComputeOutputsOfBlock(const std::shared_ptr<PySysLinkBase::SampleTime> sampleTime, double currentTime, bool isMinorStep=false)
            {
                std::vector<T> inputValues = this->GetInputValues();

                std::vector<T> outputValues = this->ComputeOutputsOfCppBlock(inputValues, sampleTime, currentTime, isMinorStep);
                this->SetOutputValues(outputValues);
                
                return this->GetOutputPorts();
            }

            virtual std::vector<T> ComputeOutputsOfCppBlock(const std::vector<T> inputs, const std::shared_ptr<PySysLinkBase::SampleTime> sampleTime, double currentTime, bool isMinorStep=false) = 0;


            virtual bool _TryUpdateConfigurationValue(std::string keyName, PySysLinkBase::ConfigurationValue value) {return false;}

            virtual const std::vector<std::pair<double, double>> GetEvents(const std::vector<T> inputs, const std::shared_ptr<PySysLinkBase::SampleTime> sampleTime, double eventTime, std::vector<double> eventTimeStates, bool includeKnownEvents=false) const
            {
                return {};
            }

            virtual const std::vector<double> GetKnownEvents(const std::shared_ptr<PySysLinkBase::SampleTime> resolvedSampleTime, double simulationStartTime, double simulationEndTime) const
            {
                return {};
            }
    };
}

#endif /* SRC_SIMULATION_BLOCK_CPP */
