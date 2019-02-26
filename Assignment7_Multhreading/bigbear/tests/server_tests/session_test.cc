#include "gtest/gtest.h"
#include "server.h"
#include "static_file_request_handler.h"
#include "action_request_handler.h"
#include "error_request_handler.h"
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <iostream>

using ::testing::Return;
using ::testing::_;

//class MockActionHandler : public ActionRequestHandler
//{
//    public:
//        MOCK_METHOD1(canHandleRequest, bool(HttpRequest req));
//        MOCK_METHOD1(handleRequest, void(std::string* response));
//};
//
//class MockStaticHandler : public StaticFileRequestHandler
//{
//    public:
//        MOCK_METHOD1(canHandleRequest, bool(HttpRequest req));
//        MOCK_METHOD1(handleRequest, void(std::string* response));
//};
//
//class SessionTest : public ::testing::Test
//{
//    protected:
//            boost::asio::io_service io_service_;
//            session new_session = session(io_service_);
//            boost::system::error_code err;
//            size_t bytesTransferred;
//            MockStaticHandler *m_staticHandler;
//            MockActionHandler *m_actionHandler;
//
//    void SetUp() override
//    {
//        m_staticHandler = new MockStaticHandler;
//        m_actionHandler = new MockActionHandler;
//        new_session.sessionRequestHandlers = new std::vector<RequestHandler*>;
//        new_session.sessionRequestHandlers  -> emplace_back(m_staticHandler);
//        new_session.sessionRequestHandlers -> emplace_back(m_actionHandler);
//    }
//
//    void TearDown() override
//    {
//        // maybe these deletions should be handle by new_session's destructor, I will delete them here for now
//        delete m_staticHandler;
//        delete m_actionHandler;
//        delete new_session.sessionRequestHandlers;
//    }
//
//};
//
//TEST_F(SessionTest, ActionRequestTest)
//{
//    char inputData[] = "GET /echo HTTP/1.1\r\nContent-Type: text/plain\r\n\r\n";
//    strcpy(new_session.data_, inputData);
//    EXPECT_CALL(*m_staticHandler, canHandleRequest(_)).Times(1).WillOnce(Return(false));
//    EXPECT_CALL(*m_actionHandler, canHandleRequest(_)).Times(1).WillOnce(Return(true));
//    EXPECT_CALL(*m_staticHandler, handleRequest(_)).Times(0);
//    EXPECT_CALL(*m_actionHandler, handleRequest(_)).Times(1);
//    new_session.handleRead(err, bytesTransferred);
//}
//
//
//TEST_F(SessionTest, StaticFileRequestTest)
//{
//    char inputData[] = "GET /static/somefile HTTP/1.1\r\nContent-Type: text/plain\r\n\r\n";
//    strcpy(new_session.data_, inputData);
//    EXPECT_CALL(*m_staticHandler, canHandleRequest(_)).Times(1).WillOnce(Return(true));
//    EXPECT_CALL(*m_actionHandler, canHandleRequest(_)).Times(0);
//    EXPECT_CALL(*m_staticHandler, handleRequest(_)).Times(1);
//    EXPECT_CALL(*m_actionHandler, handleRequest(_)).Times(0);
//    new_session.handleRead(err, bytesTransferred);
//}
//
//TEST_F(SessionTest, ErrorRequestTest)
//{
//    std::string _data = "Incorect HTTP Input";
//    ErrorRequestHandler errorHandler;
//    errorHandler.handleRequest(&_data);
//    std::string expectedOutput =  "HTTP/1.1 400 Bad Request\r\n"
//                                  "Content-Length: 23\r\n"
//                                  "Content-Type: text/plain\r\n"
//                                  "\r\n"
//                                  "400 Error: Bad request\n";
//    EXPECT_STREQ(_data.c_str(), expectedOutput.c_str());
//}
