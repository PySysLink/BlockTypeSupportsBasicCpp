#ifndef SRC_SIMULATION_BLOCK_CPP_COMMONS
#define SRC_SIMULATION_BLOCK_CPP_COMMONS

#include <string>
#include <vector>
#include <memory>
#include <PySysLinkBase/PortsAndSignalValues/InputPort.h>
#include <PySysLinkBase/PortsAndSignalValues/OutputPort.h>
#include <PySysLinkBase/PortsAndSignalValues/SignalValue.h>
#include <PySysLinkBase/SampleTime.h>
#include <PySysLinkBase/ContinuousAndOde/ContinuousState.h>
#include <PySysLinkBase/ISimulationBlock.h>
#include <PySysLinkBase/IBlockEventsHandler.h>
#include <BlockTypes/BasicCpp/SimulationBlock.h>
#include <BlockTypes/BasicCpp/SampleTime.h>
#include <spdlog/spdlog.h>

#include "SampleTimeConversion.h"
#include "LoggerInstance.h"


namespace BlockTypeSupports::BasicCppSupport
{
    template <typename T>
    class SimulationBlockCppCommons
    {
        protected:
            std::shared_ptr<BlockTypes::BasicCpp::SimulationBlock<T>> simulationBlock;
            std::shared_ptr<PySysLinkBase::SampleTime> sampleTime;
        
            std::vector<std::shared_ptr<PySysLinkBase::InputPort>> inputPorts;
            std::vector<std::shared_ptr<PySysLinkBase::OutputPort>> outputPorts;


        public:
            SimulationBlockCppCommons(std::shared_ptr<BlockTypes::BasicCpp::SimulationBlock<T>> simulationBlock, 
                                std::map<std::string, PySysLinkBase::ConfigurationValue> blockConfiguration, 
                                std::shared_ptr<PySysLinkBase::IBlockEventsHandler> blockEventsHandler) 
            {
                LoggerInstance::GetLogger()->debug("Creating basic simulation block cpp...");
                this->simulationBlock = simulationBlock;


                std::vector<bool> inputsHasDirectFeedthrough = this->simulationBlock->InputsHasDirectFeedthrough();
                if (inputsHasDirectFeedthrough.size() != this->simulationBlock->GetInputPortAmount()) {
                    throw std::runtime_error("Mismatch between the number of input ports and the size of the inputsHasDirectFeedthrough vector");
                }
                for (int i = 0; i < this->simulationBlock->GetInputPortAmount(); i++)
                {
                    std::shared_ptr<PySysLinkBase::UnknownTypeSignalValue> signalValue = std::make_shared<PySysLinkBase::SignalValue<T>>(PySysLinkBase::SignalValue<T>(0.0));
                    auto inputPort = std::make_shared<PySysLinkBase::InputPort>(PySysLinkBase::InputPort(inputsHasDirectFeedthrough[i], signalValue));
                    this->inputPorts.push_back(inputPort);
                }
                for (int i = 0; i < this->simulationBlock->GetOutputPortAmount(); i++)
                {
                    std::shared_ptr<PySysLinkBase::UnknownTypeSignalValue> signalValue = std::make_shared<PySysLinkBase::SignalValue<T>>(PySysLinkBase::SignalValue<T>(0.0));
                    auto outputPort = std::make_shared<PySysLinkBase::OutputPort>(PySysLinkBase::OutputPort(signalValue));
                    this->outputPorts.push_back(outputPort);
                }

                LoggerInstance::GetLogger()->debug("Ports configured...");

                std::shared_ptr<BlockTypes::BasicCpp::SampleTime> sampleTimeCpp = this->simulationBlock->GetSampleTime();
                this->sampleTime = SampleTimeConversion::CppSampleTimeToPySysLink(sampleTimeCpp);
                

                LoggerInstance::GetLogger()->debug("Basic simulation block cpp created");
            }

            const std::shared_ptr<PySysLinkBase::SampleTime> GetSampleTime() const 
            {
                return this->sampleTime;
            }
            
            void SetSampleTime(std::shared_ptr<PySysLinkBase::SampleTime> sampleTime)
            {
                this->sampleTime = sampleTime;
            }


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

                std::vector<T> outputValues = this->simulationBlock->CalculateOutputs(inputValues, SampleTimeConversion::PySysLinkTimeToCpp(sampleTime), currentTime, isMinorStep);
                this->SetOutputValues(outputValues);
                
                return this->GetOutputPorts();
            }

            bool TryUpdateConfigurationValue(std::string keyName, PySysLinkBase::ConfigurationValue value)
            {
                return this->simulationBlock->TryUpdateConfigurationValue(keyName, value);
            }

            const std::vector<std::pair<double, double>> GetEvents(const std::shared_ptr<PySysLinkBase::SampleTime> sampleTime, double eventTime, std::vector<double> eventTimeStates, bool includeKnownEvents=false) const 
            {
                std::vector<T> inputValues = this->GetInputValues();
                return this->simulationBlock->GetEvents(inputValues, SampleTimeConversion::PySysLinkTimeToCpp(sampleTime), eventTime, eventTimeStates, includeKnownEvents);
            }

            const std::vector<double> GetKnownEvents(const std::shared_ptr<PySysLinkBase::SampleTime> resolvedSampleTime, double simulationStartTime, double simulationEndTime) const 
            {
                return this->simulationBlock->GetKnownEvents(SampleTimeConversion::PySysLinkTimeToCpp(resolvedSampleTime), simulationStartTime, simulationEndTime);
            }

            std::vector<T> GetInputValues() const
            {
                std::vector<T> inputValues = {};
                for (int i = 0; i < this->simulationBlock->GetInputPortAmount(); i++)
                {
                    auto inputValue = this->inputPorts[i]->GetValue();
                    auto inputValueSignal = inputValue->TryCastToTyped<T>();
                    inputValues.push_back(inputValueSignal->GetPayload());
                }

                return inputValues;
            }

            void SetOutputValues(std::vector<T> outputValues)
            {
                for (int i = 0; i < this->simulationBlock->GetOutputPortAmount(); i++)
                {
                    std::shared_ptr<PySysLinkBase::UnknownTypeSignalValue> outputValue = this->outputPorts[i]->GetValue();
                    auto outputValueSignal = outputValue->TryCastToTyped<T>();
                    outputValueSignal->SetPayload(outputValues[i]);
                    this->outputPorts[i]->SetValue(std::make_shared<PySysLinkBase::SignalValue<T>>(*outputValueSignal));
                }
            }
    };

}

#endif