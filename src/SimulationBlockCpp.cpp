#include "SimulationBlockCpp.h"
#include <PySysLinkBase/PortsAndSignalValues/SignalValue.h>
#include <stdexcept>
#include "SampleTimeConversion.h"
#include "spdlog/spdlog.h"

namespace BlockTypeSupports::BasicCppSupport
{
    SimulationBlockCpp::SimulationBlockCpp(std::unique_ptr<BlockTypes::BasicCpp::SimulationBlock> simulationBlock, std::map<std::string, PySysLinkBase::ConfigurationValue> blockConfiguration, std::shared_ptr<PySysLinkBase::IBlockEventsHandler> blockEventsHandler) : ISimulationBlock(blockConfiguration, blockEventsHandler)
    {
        spdlog::get("default_pysyslink")->debug("Creating basic simulation block cpp...");
        this->simulationBlock = std::move(simulationBlock);


        std::vector<bool> inputsHasDirectFeedthrough = this->simulationBlock->InputsHasDirectFeedthrough();
        if (inputsHasDirectFeedthrough.size() != this->simulationBlock->GetInputPortAmmount()) {
            throw std::runtime_error("Mismatch between the number of input ports and the size of the inputsHasDirectFeedthrough vector");
        }
        for (int i = 0; i < this->simulationBlock->GetInputPortAmmount(); i++)
        {
            std::unique_ptr<PySysLinkBase::UnknownTypeSignalValue> signalValue = std::make_unique<PySysLinkBase::SignalValue<double>>(PySysLinkBase::SignalValue<double>(0.0));
            auto inputPort = std::make_unique<PySysLinkBase::InputPort>(PySysLinkBase::InputPort(inputsHasDirectFeedthrough[i], std::move(signalValue)));
            this->inputPorts.push_back(std::move(inputPort));
        }
        for (int i = 0; i < this->simulationBlock->GetOutputPortAmmount(); i++)
        {
            std::unique_ptr<PySysLinkBase::UnknownTypeSignalValue> signalValue = std::make_unique<PySysLinkBase::SignalValue<double>>(PySysLinkBase::SignalValue<double>(0.0));
            auto outputPort = std::make_unique<PySysLinkBase::OutputPort>(PySysLinkBase::OutputPort(std::move(signalValue)));
            this->outputPorts.push_back(std::move(outputPort));
        }

        spdlog::get("default_pysyslink")->debug("Ports configured...");

        std::shared_ptr<BlockTypes::BasicCpp::SampleTime> sampleTimeCpp = this->simulationBlock->GetSampleTime();
        this->sampleTime = SampleTimeConversion::CppSampleTimeToPySysLink(sampleTimeCpp);
        

        spdlog::get("default_pysyslink")->debug("Basic simulation block cpp created");
    }

    std::shared_ptr<PySysLinkBase::SampleTime> SimulationBlockCpp::GetSampleTime()
    {
        return this->sampleTime;
    }
    
    void SimulationBlockCpp::SetSampleTime(std::shared_ptr<PySysLinkBase::SampleTime> sampleTime)
    {
        this->sampleTime = sampleTime;
    }


    std::vector<std::shared_ptr<PySysLinkBase::InputPort>> SimulationBlockCpp::GetInputPorts() const
    {
        return this->inputPorts;
    }
            
    const std::vector<std::shared_ptr<PySysLinkBase::OutputPort>> SimulationBlockCpp::GetOutputPorts() const
    {
        return this->outputPorts;
    }

    const std::vector<std::shared_ptr<PySysLinkBase::OutputPort>> SimulationBlockCpp::ComputeOutputsOfBlock(const std::shared_ptr<PySysLinkBase::SampleTime> sampleTime)
    {
        std::vector<double> inputValues = {};
        spdlog::get("default_pysyslink")->debug("Value ammount expected: {}", this->simulationBlock->GetInputPortAmmount());
        spdlog::get("default_pysyslink")->debug("Value ammount available: {}", this->GetInputPorts().size());
        for (int i = 0; i < this->simulationBlock->GetInputPortAmmount(); i++)
        {
            auto inputValue = this->inputPorts[i]->GetValue();
            auto inputValueDouble = inputValue->TryCastToTyped<double>();
            inputValues.push_back(inputValueDouble->GetPayload());
        }
        spdlog::get("default_pysyslink")->debug("Input values acquired: {}", inputValues.size());
        for (const auto& value : inputValues)
        {
            spdlog::get("default_pysyslink")->debug(value);
        }
        std::vector<double> outputValues = this->simulationBlock->CalculateOutputs(inputValues, SampleTimeConversion::PySysLinkTimeToCpp(sampleTime));
        spdlog::get("default_pysyslink")->debug("Output values acquired: {}", outputValues.size());
        for (const auto& value : outputValues)
        {
            spdlog::get("default_pysyslink")->debug(value);
        }

        for (int i = 0; i < this->simulationBlock->GetOutputPortAmmount(); i++)
        {
            std::unique_ptr<PySysLinkBase::UnknownTypeSignalValue> outputValue = this->outputPorts[i]->GetValue();
            auto outputValueDouble = outputValue->TryCastToTyped<double>();
            outputValueDouble->SetPayload(outputValues[i]);
            this->outputPorts[i]->SetValue(std::make_unique<PySysLinkBase::SignalValue<double>>(*outputValueDouble));
        }
        
        return this->GetOutputPorts();
    }

    bool SimulationBlockCpp::TryUpdateConfigurationValue(std::string keyName, PySysLinkBase::ConfigurationValue value)
    {
        return this->simulationBlock->TryUpdateConfigurationValue(keyName, value);
    }

}
