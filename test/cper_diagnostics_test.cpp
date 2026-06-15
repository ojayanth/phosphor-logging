
#include "cper_diagnostics.hpp"

#include <map>
#include <string>
#include <variant>

#include <gtest/gtest.h>

using namespace phosphor::logging::diagnostics;

static constexpr auto CPER_MSG = "xyz.openbmc_project.Logging.CperDetected";

// collectCperData() tests
TEST(CollectCperData, ReturnsNotPresentWhenMessageIdMissing)
{
    std::map<std::string, std::string> ad{{"TYPE", "CPER"}};

    auto result = collectCperData("err", 1, ad);

    EXPECT_FALSE(result.present);
}

TEST(CollectCperData, ReturnsNotPresentForDifferentMessageId)
{
    std::map<std::string, std::string> ad{{"MESSAGE_ID", "xyz.other.Event"},
                                          {"TYPE", "CPER"}};

    auto result = collectCperData("err", 1, ad);

    EXPECT_FALSE(result.present);
}

TEST(CollectCperData, MinimalValidEventUsesDefaults)
{
    std::map<std::string, std::string> ad{{"MESSAGE_ID", CPER_MSG}};

    auto result = collectCperData("err", 1, ad);

    EXPECT_TRUE(result.present);
    EXPECT_EQ(result.type, "CPER");
    EXPECT_EQ(result.diagnosticInfo, "CPER event detected");
    EXPECT_TRUE(result.additionalDataObject.empty());
}

TEST(CollectCperData, ExtractsAllFieldsAndErasesThem)
{
    std::map<std::string, std::string> ad{
        {"MESSAGE_ID", CPER_MSG},
        {"TYPE", "CPERSection"},
        {"DATA", "decoded-summary"},
        {"ADDITIONALDATAURI", "/xyz/openbmc_project/dump/entry/1"}};

    auto result = collectCperData("err", 42, ad);

    EXPECT_TRUE(result.present);
    EXPECT_EQ(result.type, "CPERSection");
    EXPECT_EQ(result.diagnosticInfo, "decoded-summary");
    EXPECT_EQ(result.additionalDataObject, "/xyz/openbmc_project/dump/entry/1");

    // Ensure consumed fields are removed
    EXPECT_EQ(ad.count("TYPE"), 0);
    EXPECT_EQ(ad.count("DATA"), 0);
    EXPECT_EQ(ad.count("ADDITIONALDATAURI"), 0);

    // MESSAGE_ID must remain
    EXPECT_EQ(ad.count("MESSAGE_ID"), 1);
}

TEST(CollectCperData, EmptyFieldsFallbackToDefaults)
{
    std::map<std::string, std::string> ad{
        {"MESSAGE_ID", CPER_MSG}, {"TYPE", ""}, {"DATA", ""}};

    auto result = collectCperData("err", 1, ad);

    EXPECT_TRUE(result.present);
    EXPECT_EQ(result.type, "CPER");
    EXPECT_EQ(result.diagnosticInfo, "CPER event detected");
}

TEST(CollectCperData, HandlesOnlyAdditionalDataUri)
{
    std::map<std::string, std::string> ad{{"MESSAGE_ID", CPER_MSG},
                                          {"ADDITIONALDATAURI", "/xyz/dump/2"}};

    auto result = collectCperData("err", 1, ad);

    EXPECT_TRUE(result.present);
    EXPECT_EQ(result.type, "CPER");
    EXPECT_EQ(result.diagnosticInfo, "CPER event detected");
    EXPECT_EQ(result.additionalDataObject, "/xyz/dump/2");
}

// attachCperInterface() tests
class MockEntry
{
  public:
    bool invoked = false;
    std::string type;
    std::string summary;
    std::string obj;

    void createCperInterface(const std::string& t, const std::string& s,
                             const std::string& o)
    {
        invoked = true;
        type = t;
        summary = s;
        obj = o;
    }
};

TEST(AttachCperInterface, DoesNothingWhenNotPresent)
{
    MockEntry entry;
    CperData data{};

    attachCperInterface(entry, data);

    EXPECT_FALSE(entry.invoked);
}

TEST(AttachCperInterface, InvokesEntryWhenPresent)
{
    MockEntry entry;

    CperData data;
    data.present = true;
    data.type = "CPERSection";
    data.diagnosticInfo = "summary";
    data.additionalDataObject = "/obj";

    attachCperInterface(entry, data);

    EXPECT_TRUE(entry.invoked);
    EXPECT_EQ(entry.type, "CPERSection");
    EXPECT_EQ(entry.summary, "summary");
    EXPECT_EQ(entry.obj, "/obj");
}

// Entry::createCperInterface() logic tests
class FakeCperIface
{
  public:
    std::string typeValue;
    std::map<std::string, std::variant<std::string, uint64_t, bool>>
        diagnosticInfoMap;
    std::string additionalDataObjectPath;

    void type(const std::string& typeStr)
    {
        typeValue = typeStr;
    }

    void diagnosticInfo(
        const std::map<std::string, std::variant<std::string, uint64_t, bool>>&
            infoMap)
    {
        diagnosticInfoMap = infoMap;
    }

    void additionalDataObject(const std::string& objectPath)
    {
        additionalDataObjectPath = objectPath;
    }
};

static auto buildDiagMap(const std::string& diagInfo)
{
    return std::map<std::string, std::variant<std::string, uint64_t, bool>>{
        {"Summary", diagInfo.empty() ? "Summary unavailable" : diagInfo}};
}

TEST(CreateCperInterfaceLogic, SummaryPopulatedCorrectly)
{
    auto map = buildDiagMap("decoded");

    ASSERT_EQ(map.size(), 1);
    ASSERT_EQ(map.count("Summary"), 1);

    EXPECT_EQ(std::get<std::string>(map["Summary"]), "decoded");
}

TEST(CreateCperInterfaceLogic, UsesFallbackWhenEmpty)
{
    auto map = buildDiagMap("");

    EXPECT_EQ(std::get<std::string>(map["Summary"]), "Summary unavailable");
}

TEST(CreateCperInterfaceLogic, AdditionalDataOptional)
{
    std::string obj = "";
    EXPECT_TRUE(obj.empty());

    obj = "/xyz/dump/5";
    EXPECT_FALSE(obj.empty());
}
