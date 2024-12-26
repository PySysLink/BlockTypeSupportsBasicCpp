#ifndef SRC_BLOCK_TYPE_SUPPORTS_BASIC_CPP_SUPPORT_SIMULATION_BLOCK_CPP
#define SRC_BLOCK_TYPE_SUPPORTS_BASIC_CPP_SUPPORT_SIMULATION_BLOCK_CPP

#include <string>
#include <vector>
#include <memory>
#include <PySysLinkBase/PortsAndSignalValues/InputPort.h>
#include <PySysLinkBase/PortsAndSignalValues/OutputPort.h>
#include <PySysLinkBase/PortsAndSignalValues/SignalValue.h>
#include <PySysLinkBase/SampleTime.h>
#include <PySysLinkBase/ContinuousState.h>
#include <PySysLinkBase/ISimulationBlock.h>
#include <PySysLinkBase/IBlockEventsHandler.h>
#include <BlockTypes/BasicCpp/SimulationBlock.h>
#include <BlockTypes/BasicCpp/SampleTime.h>
#include <spdlog/spdlog.h>

#include "SampleTimeConversion.h"


namespace BlockTypeSupports::BasicCppSupport
{
    template <typename T>
    class SimulationBlockCpp : public PySysLinkBase::ISimulationBlock {
        private:
            std::unique_ptr<BlockTypes::BasicCpp::SimulationBlock<T>> simulationBlock;
            std::shared_ptr<PySysLinkBase::SampleTime> sampleTime;
            std::string id;
            std::string name;
        protected:
            std::vector<std::shared_ptr<PySysLinkBase::InputPort>> inputPorts;
            std::vector<std::shared_ptr<PySysLinkBase::OutputPort>> outputPorts;

        public:
            SimulationBlockCpp(std::unique_ptr<BlockTypes::BasicCpp::SimulationBlock<T>> simulationBlock, std::map<std::string, PySysLinkBase::ConfigurationValue> blockConfiguration, std::shared_ptr<PySysLinkBase::IBlockEventsHandler> blockEventsHandler) : ISimulationBlock(blockConfiguration, blockEventsHandler)
            {
                spdlog::get("default_pysyslink")->debug("Creating basic simulation block cpp...");
                this->simulationBlock = std::move(simulationBlock);


                std::vector<bool> inputsHasDirectFeedthrough = this->simulationBlock->InputsHasDirectFeedthrough();
                if (inputsHasDirectFeedthrough.size() != this->simulationBlock->GetInputPortAmmount()) {
                    throw std::runtime_error("Mismatch between the number of input ports and the size of the inputsHasDirectFeedthrough vector");
                }
                for (int i = 0; i < this->simulationBlock->GetInputPortAmmount(); i++)
                {
                    std::unique_ptr<PySysLinkBase::UnknownTypeSignalValue> signalValue = std::make_unique<PySysLinkBase::SignalValue<T>>(PySysLinkBase::SignalValue<T>(0.0));
                    auto inputPort = std::make_unique<PySysLinkBase::InputPort>(PySysLinkBase::InputPort(inputsHasDirectFeedthrough[i], std::move(signalValue)));
                    this->inputPorts.push_back(std::move(inputPort));
                }
                for (int i = 0; i < this->simulationBlock->GetOutputPortAmmount(); i++)
                {
                    std::unique_ptr<PySysLinkBase::UnknownTypeSignalValue> signalValue = std::make_unique<PySysLinkBase::SignalValue<T>>(PySysLinkBase::SignalValue<T>(0.0));
                    auto outputPort = std::make_unique<PySysLinkBase::OutputPort>(PySysLinkBase::OutputPort(std::move(signalValue)));
                    this->outputPorts.push_back(std::move(outputPort));
                }

                spdlog::get("default_pysyslink")->debug("Ports configured...");

                std::shared_ptr<BlockTypes::BasicCpp::SampleTime> sampleTimeCpp = this->simulationBlock->GetSampleTime();
                this->sampleTime = SampleTimeConversion::CppSampleTimeToPySysLink(sampleTimeCpp);
                

                spdlog::get("default_pysyslink")->debug("Basic simulation block cpp created");
            }

            std::shared_ptr<PySysLinkBase::SampleTime> GetSampleTime()
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
                std::vector<T> inputValues = {};
                spdlog::get("default_pysyslink")->debug("Value ammount expected: {}", this->simulationBlock->GetInputPortAmmount());
                spdlog::get("default_pysyslink")->debug("Value ammount available: {}", this->GetInputPorts().size());
                for (int i = 0; i < this->simulationBlock->GetInputPortAmmount(); i++)
                {
                    auto inputValue = this->inputPorts[i]->GetValue();
                    auto inputValueSignal = inputValue->TryCastToTyped<T>();
                    inputValues.push_back(inputValueSignal->GetPayload());
                }

                std::vector<T> outputValues = this->simulationBlock->CalculateOutputs(inputValues, SampleTimeConversion::PySysLinkTimeToCpp(sampleTime), currentTime);


                for (int i = 0; i < this->simulationBlock->GetOutputPortAmmount(); i++)
                {
                    std::unique_ptr<PySysLinkBase::UnknownTypeSignalValue> outputValue = this->outputPorts[i]->GetValue();
                    auto outputValueSignal = outputValue->TryCastToTyped<T>();
                    outputValueSignal->SetPayload(outputValues[i]);
                    this->outputPorts[i]->SetValue(std::make_unique<PySysLinkBase::SignalValue<T>>(*outputValueSignal));
                }
                
                return this->GetOutputPorts();
            }

            bool TryUpdateConfigurationValue(std::string keyName, PySysLinkBase::ConfigurationValue value)
            {
                return this->simulationBlock->TryUpdateConfigurationValue(keyName, value);
            }
    };
}

#endif /* SRC_BLOCK_TYPE_SUPPORTS_BASIC_CPP_SUPPORT_SIMULATION_BLOCK_CPP */
