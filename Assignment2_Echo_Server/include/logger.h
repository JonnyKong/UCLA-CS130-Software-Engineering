#ifndef LOG_H
#define LOG_H
#include <boost/asio.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/signals2.hpp>
#include "http/request.h"
using boost::asio::ip::tcp;
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
using namespace logging::trivial;
using http::server::request;
class Logger {
    public:
    src::severity_logger<severity_level> lg;
    Logger();
    void init();
    // this should be called when the server is initilized
    void logServerInitialization();
    void logTraceFile(std::string info_message);
    void logErrorFile(std::string error_message);
    void logDebugFile(std::string debug_message);
    void logWarningFile(std::string warning_message);
    void logTraceHTTPrequest(request http_request, tcp::socket& m_socket);
    void logSig();
    // static void ctrlCHandler();
    static Logger * logger;
    //static functions, might be deprecated
    static void logTrace();
    static void logDebug();
    static void logWarning();
    static void logError();
    static void logFatal();
    
    static Logger * getLogger() {
        if (Logger::logger==0) Logger::logger = new Logger();
        return Logger::logger;
    }
};
#endif
