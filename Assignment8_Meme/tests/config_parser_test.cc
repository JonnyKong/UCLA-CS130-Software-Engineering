#include "gtest/gtest.h"
#include "config_parser.h"


class NginxConfigParserTest : public :: testing::Test{
protected:
  NginxConfigParser parser;
  NginxConfig out_config;
};



TEST_F(NginxConfigParserTest, SimpleConfig) {
  bool success = parser.Parse("conf/example_config", &out_config);
  EXPECT_TRUE(success);
}


TEST_F(NginxConfigParserTest, UnexistConfig) {
  bool success = parser.Parse("conf/unexisit_config", &out_config);
  EXPECT_FALSE(success);
}


TEST_F(NginxConfigParserTest, NestLoopConfig) {
  bool success = parser.Parse("conf/nested_loop_config", &out_config);
  std::string config_string = out_config.ToString();
  EXPECT_TRUE(success);
}


TEST_F(NginxConfigParserTest, InvalidRightParenthesisConfig) {
  bool success = parser.Parse("conf/invalid_right_parenthesis_config", &out_config);
  EXPECT_FALSE(success);
}


TEST_F(NginxConfigParserTest, InvalidLeftParenthesisConfig) {
  bool success = parser.Parse("conf/invalid_left_parenthesis_config", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, EmptyConfig) {
  bool success = parser.Parse("conf/empty_config", &out_config);
  EXPECT_TRUE(success);
}


TEST_F(NginxConfigParserTest, EmptyBlock) {
  bool success = parser.Parse("conf/empty_block", &out_config);
  EXPECT_TRUE(success);
}


TEST_F(NginxConfigParserTest, NginxConfigToString) {
  bool success = parser.Parse("conf/example_config_test", &out_config);
  std::string res =  "foo bar;\nserver {\n  port 8080;\n  server_name foo.com;\n  root /home/ubuntu/sites/foo/;\n}\n";
  std::string config_string = out_config.ToString();


  bool same = config_string.compare(res)==0;

  EXPECT_TRUE(same);
}

TEST_F(NginxConfigParserTest, NginxConfigToStringNested) {
  bool success = parser.Parse("conf/nested_loop_config", &out_config);

  std::string res =  "foo \"bar\";\nserver {\n  location / {\n    proxy_pass http://localhost:8080;\n  }\n}\n";
  std::string config_string = out_config.ToString(0);
  bool same = config_string.compare(res)==0;

  EXPECT_TRUE(same);
}


TEST_F(NginxConfigParserTest, GetPortNumber) {
  bool success = parser.Parse("conf/http.conf", &out_config);

  int port = out_config.get_port_from_config();
  bool same = port==80;

  EXPECT_TRUE(same);
}

TEST_F(NginxConfigParserTest, GetPortNumberChildBlock) {
  bool success = parser.Parse("conf/example_config", &out_config);

  int port = out_config.get_port_from_config();
  bool same = port==8080;

  EXPECT_TRUE(same);
}

TEST_F(NginxConfigParserTest, NestedPort) {
  bool success = parser.Parse("conf/nested_port", &out_config);

  int port = out_config.get_port_from_config();
  bool same = port==8080;

  EXPECT_TRUE(same);
}

TEST_F(NginxConfigParserTest, GetPortNumberNull) {
  bool success = parser.Parse("conf/empty_config", &out_config);

  int port = out_config.get_port_from_config();
  bool same = port==-1;

  EXPECT_TRUE(same);
}

TEST_F(NginxConfigParserTest, NegativePortNumber) {
  bool success = parser.Parse("conf/negative_port_config", &out_config);

  int port = out_config.get_port_from_config();
  bool same = port==-1;

  EXPECT_TRUE(same);
}

TEST_F(NginxConfigParserTest, LargePortNumber) {
  bool success = parser.Parse("conf/large_port_config", &out_config);

  int port = out_config.get_port_from_config();
  bool same = port==-1;

  EXPECT_TRUE(same);
}

TEST_F(NginxConfigParserTest, GetPortNumberNoPort) {
  bool success = parser.Parse("conf/no_port_config", &out_config);

  int port = out_config.get_port_from_config();
  bool same = port==-1;

  EXPECT_TRUE(same);
}

TEST_F(NginxConfigParserTest, GetPortNumberNoFile) {
  bool success = parser.Parse("conf/unexist_config", &out_config);

  int port = out_config.get_port_from_config();
  bool same = port==-1;

  EXPECT_TRUE(same);
}

TEST_F(NginxConfigParserTest, InvalidPort) {
  bool success = parser.Parse("conf/invalid_port", &out_config);

  int port = out_config.get_port_from_config();
  bool same = port==-1;

  EXPECT_TRUE(same);
}

TEST_F(NginxConfigParserTest, SingleQuote) {
  bool success = parser.Parse("conf/single_quote", &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, DoubleQuote) {
  bool success = parser.Parse("conf/double_quote", &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, SingleQuoteError) {
  bool success = parser.Parse("conf/single_quote_error", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, DoubleQuoteError) {
  bool success = parser.Parse("conf/double_quote_error", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, EofSingleQuote) {
  bool success = parser.Parse("conf/endl_single_quote", &out_config);
  EXPECT_FALSE(success);
}


TEST_F(NginxConfigParserTest, EofDoubleQuote) {
  bool success = parser.Parse("conf/endl_double_quote", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, StatementEndError) {
  bool success = parser.Parse("conf/statement_end_error", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, StartBlockError) {
  bool success = parser.Parse("conf/start_block_error", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, EndBlockError) {
  bool success = parser.Parse("conf/end_block_error", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, UnknownToken) {
  bool success = parser.Parse("conf/unknown_token", &out_config);
  EXPECT_FALSE(success);
}
