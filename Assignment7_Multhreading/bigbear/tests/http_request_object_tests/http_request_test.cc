#include "gtest/gtest.h"
#include "http_request.h"

class HttpRequestTest : public ::testing::Test {
  protected:
  	HttpRequest request;
};

TEST_F(HttpRequestTest, partialRequest)
{
	std::string firstHalf = "GET /static/bearlist.txt HT";

	bool successfullyParsed = request.parseHttpRequest(firstHalf);
	EXPECT_FALSE(successfullyParsed);
	EXPECT_FALSE(request.isComplete);
}

TEST_F(HttpRequestTest, requestInTwoPieces)
{
	std::string firstHalf = "GET /static/bearlist.txt HT";
	std::string secondHalf = "TP/1.1\r\nUser-Agent: nc/0.0.1\r\nHost: 127.0.0.1\r\nAccept: */*\r\n\r\n";

	request.parseHttpRequest(firstHalf);
	bool successfullyParsed = request.finishParsingRequest(secondHalf);

	EXPECT_TRUE(successfullyParsed);
	EXPECT_TRUE(request.isComplete);
	EXPECT_STREQ(request.httpVersion.c_str(), "HTTP/1.1");
}

TEST_F(HttpRequestTest, requestInThreePieces)
{
	std::string firstPart = "GET /static/bearlist.txt HT";
	std::string secondPart = "TP/1.1\r\nUse";
	std::string thirdPart = "r-Agent: nc/0.0.1\r\nHost: 127.0.0.1\r\nAccept: */*\r\n\r\n";

	request.parseHttpRequest(firstPart);
	request.finishParsingRequest(secondPart);
	bool successfullyParsed = request.finishParsingRequest(thirdPart);

	EXPECT_TRUE(successfullyParsed);
	EXPECT_TRUE(request.isComplete);
	EXPECT_STREQ(request.headerFields[USER_AGENT].c_str(), "nc/0.0.1");
}