#include "BlockFactoryCpp.h"
#include <stdexcept>
#include "SimulationBlockCpp.h"
#include "spdlog/spdlog.h"
#include "BlockLibrariesPluginLoader.h"
#include <algorithm>
#include "LoggerInstance.h"
#include <PySysLinkBase/PortsAndSignalValues/PortTypeMetadata.h>

namespace BlockTypeSupports::BasicCppSupport
{
    BlockFactoryCpp::BlockFactoryCpp(std::map<std::string, PySysLinkBase::ConfigurationValue> pluginConfiguration)
    {
        std::string libraryPluginPath = PySysLinkBase::ConfigurationValueManager::TryGetConfigurationValue<std::string>("BasicCppSupport/libraryPluginPath", pluginConfiguration);

        std::unique_ptr<BlockLibrariesPluginLoader> blockLibrariesPluginLoader = std::make_unique<BlockLibrariesPluginLoader>();
        this->factoryRegistry = blockLibrariesPluginLoader->LoadPlugins(libraryPluginPath);
    }

    std::string ResolveSignalType(
            const std::vector<PySysLinkBase::PortTypeMetadata>& inputs,
            const std::vector<PySysLinkBase::PortTypeMetadata>& outputs)
    {
            
            for (const auto& p : inputs)
            {
                if (p.category == PySysLinkBase::PortCategory::FullySupportedSignalValue &&
                p.signalValueType.has_value())
                {
                        std::cout << "Resolved signal type from input port: " << p.signalValueType.value() << std::endl;
                        return p.signalValueType.value();
                }
            }
            for (const auto& p : outputs)
            {
                if (p.category == PySysLinkBase::PortCategory::FullySupportedSignalValue &&
                p.signalValueType.has_value())
                {
                        std::cout << "Resolved signal type from output port: " << p.signalValueType.value() << std::endl;
                        return p.signalValueType.value();
                }
            }

            throw std::invalid_argument("Could not resolve signal type from ports.");
    }

    std::string PortCategoryToString(PySysLinkBase::PortCategory cat)
    {
            using namespace PySysLinkBase;

            switch (cat)
            {
                case PortCategory::FullySupportedSignalValue: return "FullySupportedSignalValue";
                case PortCategory::Enumeration: return "Enumeration";
                case PortCategory::Structure: return "Structure";
                case PortCategory::PointerToObject: return "PointerToObject";
                case PortCategory::OtherType: return "OtherType";
                case PortCategory::Inherited: return "Inherited";
                case PortCategory::Unknown: return "Unknown";
                default: return "Invalid";
            }
    }
    
    std::string JoinPortTypes(const std::vector<PySysLinkBase::PortTypeMetadata>& types)
    {
            if (types.empty())
                return "[]";

            std::ostringstream oss;
            oss << "[";

            for (size_t i = 0; i < types.size(); ++i)
            {
                oss << PortCategoryToString(types[i].category);
                oss << "(";
                if (types[i].signalValueType.has_value())
                {
                        oss << types[i].signalValueType.value();
                }
                else
                {
                        oss << "no signal value type";
                }
                oss << ")";
                if (i + 1 < types.size())
                        oss << ", ";
            }

            oss << "]";
            return oss.str();
    }

    std::shared_ptr<PySysLinkBase::ISimulationBlock> BlockFactoryCpp::CreateBlock(std::map<std::string, PySysLinkBase::ConfigurationValue> blockConfiguration, std::shared_ptr<PySysLinkBase::IBlockEventsHandler> blockEventsHandler)
    {
        std::string blockClass = PySysLinkBase::ConfigurationValueManager::TryGetConfigurationValue<std::string>("BlockClass", blockConfiguration);
        
        int inputPortNumber = PySysLinkBase::ConfigurationValueManager::TryGetConfigurationValue<int>("InputPortNumber", blockConfiguration);
        int outputPortNumber = PySysLinkBase::ConfigurationValueManager::TryGetConfigurationValue<int>("OutputPortNumber", blockConfiguration);

        auto inputTypes = PySysLinkBase::ParsePortTypeMetadatas(
        PySysLinkBase::ConfigurationValueManager::TryGetConfigurationValue<std::vector<std::string>>(
            "InputPortTypes", blockConfiguration));

        auto outputTypes = PySysLinkBase::ParsePortTypeMetadatas(
        PySysLinkBase::ConfigurationValueManager::TryGetConfigurationValue<std::vector<std::string>>(
            "OutputPortTypes", blockConfiguration));
        
        std::string dataType;
        try {
            dataType = ResolveSignalType(inputTypes, outputTypes);
        }
        catch (const std::invalid_argument&)
        {
            throw std::invalid_argument(
                    "Could not resolve signal type for block '" + blockClass +
                    "'\n  InputPortTypes: " + JoinPortTypes(inputTypes) +
                    "\n  OutputPortTypes: " + JoinPortTypes(outputTypes)
            );
        }

        LoggerInstance::GetLogger()->debug("{} class block to create with type {}", blockClass, dataType);
        
        return this->CreateBlockFromRegistry(this->factoryRegistry, blockClass, blockConfiguration, blockEventsHandler, dataType, inputPortNumber, outputPortNumber);
    }  

} // namespace BlockTypeSupports::BasicCppSupport
