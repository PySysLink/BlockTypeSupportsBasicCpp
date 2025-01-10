#ifndef SRC_SIMULATION_BLOCK_CPP
#define SRC_SIMULATION_BLOCK_CPP

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


namespace BlockTypeSupports::BasicCppSupport
{
    template <typename T>
    class SimulationBlockCpp : public virtual PySysLinkBase::ISimulationBlock {
        protected:
            std::shared_ptr<BlockTypes::BasicCpp::SimulationBlock<T>> simulationBlock;
            std::shared_ptr<PySysLinkBase::SampleTime> sampleTime;
            std::string id;
            std::string name;
        
            std::vector<std::shared_ptr<PySysLinkBase::InputPort>> inputPorts;
            std::vector<std::shared_ptr<PySysLinkBase::OutputPort>> outputPorts;

            std::vector<T> GetInputValues() const
            {
                std::vector<T> inputValues = {};
                for (int i = 0; i < this->simulationBlock->GetInputPortAmmount(); i++)
                {
                    auto inputValue = this->inputPorts[i]->GetValue();
                    auto inputValueSignal = inputValue->TryCastToTyped<T>();
                    inputValues.push_back(inputValueSignal->GetPayload());
                }

                return inputValues;
            }

            void SetOutputValues(std::vector<T> outputValues)
            {
                for (int i = 0; i < this->simulationBlock->GetOutputPortAmmount(); i++)
                {
                    std::shared_ptr<PySysLinkBase::UnknownTypeSignalValue> outputValue = this->outputPorts[i]->GetValue();
                    auto outputValueSignal = outputValue->TryCastToTyped<T>();
                    outputValueSignal->SetPayload(outputValues[i]);
                    this->outputPorts[i]->SetValue(std::make_shared<PySysLinkBase::SignalValue<T>>(*outputValueSignal));
                }
            }

        public:
            SimulationBlockCpp(std::shared_ptr<BlockTypes::BasicCpp::SimulationBlock<T>> simulationBlock, 
                                std::map<std::string, PySysLinkBase::ConfigurationValue> blockConfiguration, 
                                std::shared_ptr<PySysLinkBase::IBlockEventsHandler> blockEventsHandler) 
                                : ISimulationBlock(blockConfiguration, blockEventsHandler) 
            {
                spdlog::get("default_pysyslink")->debug("Creating basic simulation block cpp...");
                this->simulationBlock = simulationBlock;


                std::vector<bool> inputsHasDirectFeedthrough = this->simulationBlock->InputsHasDirectFeedthrough();
                if (inputsHasDirectFeedthrough.size() != this->simulationBlock->GetInputPortAmmount()) {
                    throw std::runtime_error("Mismatch between the number of input ports and the size of the inputsHasDirectFeedthrough vector");
                }
                for (int i = 0; i < this->simulationBlock->GetInputPortAmmount(); i++)
                {
                    std::shared_ptr<PySysLinkBase::UnknownTypeSignalValue> signalValue = std::make_shared<PySysLinkBase::SignalValue<T>>(PySysLinkBase::SignalValue<T>(0.0));
                    auto inputPort = std::make_shared<PySysLinkBase::InputPort>(PySysLinkBase::InputPort(inputsHasDirectFeedthrough[i], signalValue));
                    this->inputPorts.push_back(inputPort);
                }
                for (int i = 0; i < this->simulationBlock->GetOutputPortAmmount(); i++)
                {
                    std::shared_ptr<PySysLinkBase::UnknownTypeSignalValue> signalValue = std::make_shared<PySysLinkBase::SignalValue<T>>(PySysLinkBase::SignalValue<T>(0.0));
                    auto outputPort = std::make_shared<PySysLinkBase::OutputPort>(PySysLinkBase::OutputPort(signalValue));
                    this->outputPorts.push_back(outputPort);
                }

                spdlog::get("default_pysyslink")->debug("Ports configured...");

                std::shared_ptr<BlockTypes::BasicCpp::SampleTime> sampleTimeCpp = this->simulationBlock->GetSampleTime();
                this->sampleTime = SampleTimeConversion::CppSampleTimeToPySysLink(sampleTimeCpp);
                

                spdlog::get("default_pysyslink")->debug("Basic simulation block cpp created");
            }

            std::shared_ptr<PySysLinkBase::SampleTime> GetSampleTime() override
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

            const std::vector<std::shared_ptr<PySysLinkBase::OutputPort>> ComputeOutputsOfBlock(const std::shared_ptr<PySysLinkBase::SampleTime> sampleTime, double currentTime)
            {
                std::vector<T> inputValues = this->GetInputValues();

                std::vector<T> outputValues = this->simulationBlock->CalculateOutputs(inputValues, SampleTimeConversion::PySysLinkTimeToCpp(sampleTime), currentTime);
                this->SetOutputValues(outputValues);
                
                return this->GetOutputPorts();
            }

            bool TryUpdateConfigurationValue(std::string keyName, PySysLinkBase::ConfigurationValue value)
            {
                return this->simulationBlock->TryUpdateConfigurationValue(keyName, value);
            }
    };
}

#endif /* SRC_SIMULATION_BLOCK_CPP */
