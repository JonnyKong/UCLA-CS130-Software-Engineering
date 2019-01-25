#include "gtest/gtest.h"
#include "config_parser.h"


class NginxConfigParserTest : public :: testing::Test{
protected:
  NginxConfigParser parser;
  NginxConfig out_config;
};

TEST_F(NginxConfigParserTest, SimpleConfig) {
  bool success = parser.Parse("example_config", &out_config);
  EXPECT_TRUE(success);
}


TEST_F(NginxConfigParserTest, NestLoopConfig) {
  bool success = parser.Parse("nested_loop_config", &out_config);
  EXPECT_TRUE(success);
}


TEST_F(NginxConfigParserTest, InvalidRightParenthesisConfig) {
  bool success = parser.Parse("invalid_right_parenthesis_config", &out_config);
  EXPECT_FALSE(success);
}


TEST_F(NginxConfigParserTest, InvalidLeftParenthesisConfig) {
  bool success = parser.Parse("invalid_left_parenthesis_config", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, EmptyConfig) {
  bool success = parser.Parse("empty_config", &out_config);
  EXPECT_TRUE(success);
}


TEST_F(NginxConfigParserTest, EmptyBlock) {
  bool success = parser.Parse("empty_block", &out_config);
  EXPECT_TRUE(success);
}


TEST_F(NginxConfigParserTest, NginxConfigToString) {
  bool success = parser.Parse("example_config", &out_config);

  std::string res =  "foo \"bar\";\nserver {\n  port 80;\n  server_name foo.com;\n  root /home/ubuntu/sites/foo/;\n}\n";
  std::string config_string = out_config.ToString();
  bool same = config_string.compare(res)==0;

  EXPECT_TRUE(same);
}

TEST_F(NginxConfigParserTest, GetPortNumber) {
  bool success = parser.Parse("test_port_number", &out_config);

  int port = out_config.get_port_from_config(&out_config);
  bool same = port==80;

  EXPECT_TRUE(same);
}

TEST_F(NginxConfigParserTest, GetPortNumberChildBlock) {
  bool success = parser.Parse("example_config", &out_config);

  int port = out_config.get_port_from_config(&out_config);
  bool same = port==80;

  EXPECT_TRUE(same);
}
