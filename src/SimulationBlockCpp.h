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
    template <typename I, typename O = I>
    class SimulationBlockCpp : public PySysLinkBase::ISimulationBlock {
        protected:
            std::vector<std::shared_ptr<PySysLinkBase::InputPort>> inputPorts;
            std::vector<std::shared_ptr<PySysLinkBase::OutputPort>> outputPorts;

            std::shared_ptr<PySysLinkBase::SampleTime> sampleTime;
            
            std::vector<I> GetInputValues() const
            {
                std::vector<I> inputValues = {};
                for (int i = 0; i < this->inputPorts.size(); i++)
                {
                    auto inputValue = this->inputPorts[i]->GetValue();
                    auto inputValueSignal = inputValue->TryCastToTyped<I>();
                    inputValues.push_back(inputValueSignal->GetPayload());
                }

                return inputValues;
            }

            void SetOutputValues(std::vector<O> outputValues)
            {
                for (int i = 0; i < this->outputPorts.size(); i++)
                {
                    std::shared_ptr<PySysLinkBase::UnknownTypeSignalValue> outputValue = this->outputPorts[i]->GetValue();
                    auto outputValueSignal = outputValue->TryCastToTyped<O>();
                    outputValueSignal->SetPayload(outputValues[i]);
                    this->outputPorts[i]->SetValue(std::make_shared<PySysLinkBase::SignalValue<O>>(*outputValueSignal));
                }
            }
            
        public:
            SimulationBlockCpp(std::map<std::string, PySysLinkBase::ConfigurationValue> blockConfiguration, 
                                std::shared_ptr<PySysLinkBase::IBlockEventsHandler> blockEventsHandler,
                                int inputPortNumber, int outputPortNumber, bool inputsHasDirectFeedthrough) 
                                : ISimulationBlock(blockConfiguration, blockEventsHandler) 
            {
                LoggerInstance::GetLogger()->debug("Creating basic simulation block cpp...");
                LoggerInstance::GetLogger()->debug("Input ports: {}, Output ports: {}", inputPortNumber, outputPortNumber);

           
                for (int i = 0; i < inputPortNumber; i++)
                {
                    std::shared_ptr<PySysLinkBase::UnknownTypeSignalValue> signalValue = std::make_shared<PySysLinkBase::SignalValue<I>>(PySysLinkBase::SignalValue<I>(I{}));
                    auto inputPort = std::make_shared<PySysLinkBase::InputPort>(PySysLinkBase::InputPort(inputsHasDirectFeedthrough, signalValue));
                    this->inputPorts.push_back(inputPort);
                }
                for (int i = 0; i < outputPortNumber; i++)
                {
                    std::shared_ptr<PySysLinkBase::UnknownTypeSignalValue> signalValue = std::make_shared<PySysLinkBase::SignalValue<O>>(PySysLinkBase::SignalValue<O>(O{}));
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
                std::vector<I> inputValues = this->GetInputValues();

                std::vector<O> outputValues = this->ComputeOutputsOfCppBlock(inputValues, sampleTime, currentTime, isMinorStep);
                this->SetOutputValues(outputValues);
                
                return this->GetOutputPorts();
            }

            virtual std::vector<O> ComputeOutputsOfCppBlock(const std::vector<I> inputs, const std::shared_ptr<PySysLinkBase::SampleTime> sampleTime, double currentTime, bool isMinorStep=false) = 0;


            virtual bool _TryUpdateConfigurationValue(std::string keyName, PySysLinkBase::ConfigurationValue value) {return false;}

            virtual const std::vector<std::pair<double, double>> GetEvents(const std::vector<I> inputs, const std::shared_ptr<PySysLinkBase::SampleTime> sampleTime, double eventTime, std::vector<double> eventTimeStates, bool includeKnownEvents=false) const
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
