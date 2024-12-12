#ifndef SRC_BLOCK_TYPE_SUPPORTS_BASIC_CPP_SUPPORT_SIMULATION_BLOCK_CPP
#define SRC_BLOCK_TYPE_SUPPORTS_BASIC_CPP_SUPPORT_SIMULATION_BLOCK_CPP

#include <string>
#include <vector>
#include <memory>
#include <PySysLinkBase/PortsAndSignalValues/InputPort.h>
#include <PySysLinkBase/PortsAndSignalValues/OutputPort.h>
#include <PySysLinkBase/SampleTime.h>
#include <PySysLinkBase/ContinuousState.h>
#include <PySysLinkBase/ISimulationBlock.h>
#include <BlockTypes/BasicCpp/SimulationBlock.h>
#include <BlockTypes/BasicCpp/SampleTime.h>


namespace BlockTypeSupports::BasicCppSupport
{
    class SimulationBlockCpp : public PySysLinkBase::ISimulationBlock {
        private:
            std::unique_ptr<BlockTypes::BasicCpp::SimulationBlock> simulationBlock;
            std::shared_ptr<PySysLinkBase::SampleTime> sampleTime;
            std::string id;
            std::string name;
        protected:
            std::vector<std::shared_ptr<PySysLinkBase::InputPort>> inputPorts;
            std::vector<std::shared_ptr<PySysLinkBase::OutputPort>> outputPorts;

        public:
            const std::string GetId() const;
            const std::string GetName() const;

            SimulationBlockCpp(std::unique_ptr<BlockTypes::BasicCpp::SimulationBlock> simulationBlock, std::map<std::string, PySysLinkBase::ConfigurationValue> blockConfiguration);
            std::shared_ptr<PySysLinkBase::SampleTime> GetSampleTime();
            void SetSampleTime(std::shared_ptr<PySysLinkBase::SampleTime> sampleTime);

            std::vector<std::shared_ptr<PySysLinkBase::InputPort>> GetInputPorts() const;
            const std::vector<std::shared_ptr<PySysLinkBase::OutputPort>> GetOutputPorts() const;

            const std::vector<std::shared_ptr<PySysLinkBase::OutputPort>> ComputeOutputsOfBlock(const std::shared_ptr<PySysLinkBase::SampleTime> sampleTime);
    };
}

#endif /* SRC_BLOCK_TYPE_SUPPORTS_BASIC_CPP_SUPPORT_SIMULATION_BLOCK_CPP */
