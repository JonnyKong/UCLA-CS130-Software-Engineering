#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/asio.hpp>
#include <boost/asio/signal_set.hpp>
#include "http/request.h"
#include <boost/signals2.hpp>
#include "logger.h"
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
using namespace logging::trivial;
using boost::asio::ip::tcp;


Logger::Logger() {
    init();
    logging::add_common_attributes();
    logging::record rec = lg.open_record();
    logging::add_console_log(std::cout, boost::log::keywords::format = ">> %Message%");
}
void Logger::init() {
    logging::add_file_log
    (
        keywords::file_name = "../log/SYSLOG_%N.log",
        keywords::rotation_size = 10 * 1024 * 1024,
        keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
        keywords::format = "[%TimeStamp%]:[%ThreadID%]:%Message%",
        keywords::auto_flush = true                             /*< log record format >*/
    );
}

void Logger::logServerInitialization() {
    BOOST_LOG_SEV(lg, trace) << "Trace: " << "Server has been initialized";
}

void Logger::logTraceFile(std::string trace_message) {
    BOOST_LOG_SEV(lg, trace) << "Trace: " << trace_message;
}

void Logger::logErrorFile(std::string error_message){
    BOOST_LOG_SEV(lg, error) << "Error: " << error_message;
}
void Logger::logDebugFile(std::string debug_message){
    BOOST_LOG_SEV(lg, debug) << "Debug: " << debug_message;
}

void Logger::logWarningFile(std::string warning_message){
    BOOST_LOG_SEV(lg, warning) << "Warning: " << warning_message;
}

void Logger::logSig() {
    BOOST_LOG_SEV(Logger::lg, warning) << "Warning: "  << "Shutting down the server...";
}

void Logger::logTraceHTTPrequest(const request& http_request, tcp::socket& m_socket) {
    std::stringstream stream;
    stream << "Trace: ";
    stream << http_request.method << " " << http_request.uri
    << " HTTP " << http_request.http_version_major << "." << http_request.http_version_minor;
    stream << " IP: " << m_socket.remote_endpoint().address().to_string();
    BOOST_LOG_SEV(lg, trace) << stream.str();
}

void Logger::logRequestHealthMetrics(const request& http_request, tcp::socket& m_socket, const std::string res_status="200") {
    std::stringstream stream;
    stream << "ResponseMetrics::";
    stream << http_request.method << " " << http_request.uri
    << " HTTP/" << http_request.http_version_major << "." << http_request.http_version_minor;
    stream << " IP: " << m_socket.remote_endpoint().address().to_string();
    stream << " Response Status: " << res_status;
    BOOST_LOG_SEV(lg, trace) << stream.str();
}

Logger* Logger::logger = 0;
