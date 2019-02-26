#include "gtest/gtest.h"
#include "config_parser.h"


class NginxConfigParserTest : public ::testing::Test {
  protected:
    NginxConfigParser parser;

};

// Basic Ngnix config
TEST_F(NginxConfigParserTest, ValidConfig) {

    NginxConfig config;
    std::vector<std::string> expectedOrder = {"root", "listen", "static1", "echo1", "static2", "echo2"};

    std::unordered_map<std::string, std::string> expectedFlatParamsServer { {"root", "/usr/src/projects/bigbear"},
                                                                          {"listen", "5476"}};
    std::unordered_map<std::string, std::string> expectedFlatParamsStatic1 { {"location", "/static"},
                                                                           {"root", "static_files/some_files"}};
    std::unordered_map<std::string, std::string> expectedFlatParamsStatic2 { {"location", "/static2"},
                                                                           {"root", "static_files/more_files"}};
    std::unordered_map<std::string, std::string> expectedFlatParamsEcho1 { {"location", "/echo"}};

    std::unordered_map<std::string, std::string> expectedFlatParamsEcho2 { {"location", "/echo2"}};

    bool success = parser.Parse("valid_config", &config);

    std::unordered_map<std::string, std::string> actualFlatParamsServer = config.getFlatParameters();
    std::unordered_map<std::string, std::string> actualFlatParamsStatic1 = config.getNestedParameters()["static1"]->getFlatParameters();
    std::unordered_map<std::string, std::string> actualFlatParamsStatic2 = config.getNestedParameters()["static2"]->getFlatParameters();
    std::unordered_map<std::string, std::string> actualFlatParamsEcho1 = config.getNestedParameters()["echo1"]->getFlatParameters();
    std::unordered_map<std::string, std::string> actualFlatParamsEcho2 = config.getNestedParameters()["echo2"]->getFlatParameters();

    EXPECT_TRUE(success);
    EXPECT_EQ(config.getOrderOfParams(), expectedOrder);
    EXPECT_EQ(actualFlatParamsServer, expectedFlatParamsServer);
    EXPECT_EQ(actualFlatParamsStatic1, expectedFlatParamsStatic1);
    EXPECT_EQ(actualFlatParamsStatic2, expectedFlatParamsStatic2);
    EXPECT_EQ(actualFlatParamsEcho1, expectedFlatParamsEcho1);
    EXPECT_EQ(actualFlatParamsEcho2, expectedFlatParamsEcho2);
}

TEST_F(NginxConfigParserTest, MissingBrace) {
    NginxConfig config;
    bool success = parser.Parse("missing_braces", &config);

    EXPECT_FALSE(success);
}