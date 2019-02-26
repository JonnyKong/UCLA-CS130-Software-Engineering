#include "gtest/gtest.h"
#include "config_parser.h"
#include "gmock/gmock.h"

using namespace testing;

// Mock Parser
class MockNginxConfigParser : public NginxConfigParser {
 public:
  MockNginxConfigParser() {
    // // Delegate all calls to real object
    // // Have to be specified before actual calling
    // ON_CALL(*this, ParseToken(_, _))
    //   .WillByDefault(Invoke(&real_, &NginxConfigParser::ParseToken));
  }
  MOCK_METHOD2(ParseToken, TokenType(std::istream* input, std::string* value));
 private:
  NginxConfigParser real_;
};

// Mock fixture
class NginxConfigParserTestFixture : public ::testing::Test {
 protected:
  // MockNginxConfigParser parser;
  NginxConfigParser parser;
  NginxConfig out_config;
  // void SetUp() override { }
  // void TearDown() override { }
};

// SimpleConfig
TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;
  bool success = parser.Parse("example_config", &out_config);
  EXPECT_TRUE(success);
}

// SimpleConfig in fixture
TEST_F(NginxConfigParserTestFixture, SimpleConfigFixture) {
  // EXPECT_CALL(parser, ParseToken(_, _)).Times(AtLeast(1)); 
  bool success = parser.Parse("example_config", &out_config);
  EXPECT_TRUE(success);
}

// Should success with space before colon
TEST_F(NginxConfigParserTestFixture, SpaceBeforeColon) {
  // EXPECT_CALL(parser, ParseToken(_, _)).Times(AtLeast(1));
  bool success = parser.Parse("example_config1", &out_config);
  EXPECT_TRUE(success);
}

// Should success with comments (this test have 15 comments)
TEST_F(NginxConfigParserTestFixture, Comments) {
  bool success = parser.Parse("example_config2", &out_config);
  EXPECT_TRUE(success);
}

// Should success with nested blocks
TEST_F(NginxConfigParserTestFixture, NestedBlocks) {
  bool success = parser.Parse("example_config3", &out_config);
  EXPECT_TRUE(success);
}

// Should fail with invalid numebr of curly braces
TEST_F(NginxConfigParserTestFixture, InvalidBraces) {
  bool success = parser.Parse("example_config4", &out_config);
  EXPECT_FALSE(success);
}

// Should success with {}
TEST_F(NginxConfigParserTestFixture, AdjacentCurlyBraces) {
  bool success = parser.Parse("example_config5", &out_config);
  EXPECT_TRUE(success);
}

// Should fail with statement without token
TEST_F(NginxConfigParserTestFixture, StatementWithoutToken) {
  bool success = parser.Parse("example_config6", &out_config);
  EXPECT_FALSE(success);
}

// Should success with statement with multiple tokens
TEST_F(NginxConfigParserTestFixture, StatementWithMultipleToken) {
  bool success = parser.Parse("example_config7", &out_config);
  EXPECT_TRUE(success);
}

// Should success config without statements
TEST_F(NginxConfigParserTestFixture, ConfigWithoutStatements) {
  bool success = parser.Parse("example_config8", &out_config);
  EXPECT_TRUE(success);
}

// Should success config with multiple statements
TEST_F(NginxConfigParserTestFixture, ConfigWithMultipleStatements) {
  bool success = parser.Parse("example_config9", &out_config);
  EXPECT_TRUE(success);
}

// Should success escape within quoted strings
TEST_F(NginxConfigParserTestFixture, EscapeWithinQuotedStrings) {
  bool success = parser.Parse("example_config10", &out_config);
  EXPECT_TRUE(success);
}

// Should fail quoted strings followed not by whitespace
TEST_F(NginxConfigParserTestFixture, QuotedStringsFollowedByWhitespace) {
  bool success = parser.Parse("example_config11", &out_config);
  EXPECT_FALSE(success);
}

// Should success with empty config
TEST_F(NginxConfigParserTestFixture, EmptyConfig) {
  bool success = parser.Parse("example_config12", &out_config);
  EXPECT_TRUE(success);
}

// Should success with invalid left parenthesis
TEST_F(NginxConfigParserTestFixture, InvalidLeftParenthesis) {
  bool success = parser.Parse("example_config13", &out_config);
  EXPECT_FALSE(success);
}