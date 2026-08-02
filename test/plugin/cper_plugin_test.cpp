#include "plugin/cper_descriptor.hpp"
#include "plugin/cper_plugin.hpp"
#include "plugin/plugin_manager.hpp"
#include "plugin/plugin_registry.hpp"

#include <gtest/gtest.h>

namespace phosphor::logging::plugin::cper
{

TEST(CperPluginTest, RegisterAndCreatePlugin)
{
    PluginRegistry registry;

    registerPlugin(registry);

    PluginManager manager(registry);

    auto bus = sdbusplus::bus::new_default();

    std::string objectPath = "/xyz/openbmc_project/logging/entry/1";

    PluginContext context{
        bus,
        objectPath,
    };

    Descriptor descriptor(DiagnosticDataType::CPER, "notification-guid",
                          "section-guid", -1);

    auto plugin = manager.create(context, descriptor);

    ASSERT_NE(plugin, nullptr);
    EXPECT_EQ(plugin->type(), plugin::Type::cper);
}

TEST(CperPluginTest, CreatePluginWithCperSectionType)
{
    PluginRegistry registry;

    registerPlugin(registry);

    PluginManager manager(registry);

    auto bus = sdbusplus::bus::new_default();

    PluginContext context{
        bus,
        "/xyz/openbmc_project/logging/entry/1",
    };

    Descriptor descriptor(DiagnosticDataType::CPERSection, "notification-guid",
                          "section-guid", -1);

    auto plugin = manager.create(context, descriptor);

    ASSERT_NE(plugin, nullptr);
    EXPECT_EQ(plugin->type(), plugin::Type::cper);
}

TEST(CperPluginTest, RegistrationLookup)
{
    PluginRegistry registry;

    registerPlugin(registry);

    auto factory = registry.lookup(plugin::Type::cper);

    ASSERT_NE(factory, nullptr);
}

TEST(CperPluginTest, FactoryCreate)
{
    Factory factory;

    auto bus = sdbusplus::bus::new_default();

    std::string objectPath = "/xyz/openbmc_project/logging/entry/1";

    PluginContext context{
        bus,
        objectPath,
    };

    Descriptor descriptor(DiagnosticDataType::CPER, "notification-guid",
                          "section-guid", -1);

    auto plugin = factory.create(context, descriptor);

    ASSERT_NE(plugin, nullptr);
    EXPECT_EQ(plugin->type(), plugin::Type::cper);
}

} // namespace phosphor::logging::plugin::cper
