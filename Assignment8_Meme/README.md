# UCLA-CS130-Software-Engineering

## 1. Overall Structure of Source Code

```
                    +----------------------------+
                    |        server_main.cc      +-----------------------+
                    +-------------+--------------+             get parsed|
                                  |                            config    |
                                  |                                      |
                                  v                                      v
                    +----------------------------+         +-------------+--------------+
               +----+          server.h          +----+    |      config_parser.h       |
               |    +----------------------------+    |    +----------------------------+
               |                                      |
init a session |                      setup dispatcher|
object for each|                      based on parsed |
request        v                      config          v
 +-------------+--------------+        +--------------+-------------+
 |         session.h          +------->+        dispatcher.h        |
 +-------------+--------------+  get   +--------------+-------------+
               |                 hdlr                 |
    get parsed |                         return an    |
    request    v                         instance of  v
 +-------------+--------------+        +--------------+-------------+
 |      request_parser.h      |        |  request_handler.h         |
 +----------------------------+        |  +-request_handler_static.h|
                                       |  +-request_handler_echo.h  |
                                       |  +-request_handler_error.h |
                                       |  +-request_handler_status.h|
                                       +----------------------------+
```

The primary function of this program is to set up a server that can do simple echoing, serve a static file, return the request stats, and show the error page. The program is written in C++ and Boost library. The chart above shows the basic layout of the source code.  The main file is server_main.cc, which sets up the server object and parses the config file. The config file parsing is done by config_parser.h. Upon the initialization of the server, the server object creates a dispatcher. The dispatcher is generated based on the config files, it maps a path to a specific handler. For each incoming requestion, a session would be generated. Each HTTP request is passed through the request_parser.h. And the dispatcher would return its corresponding handler to each session.

### Logging

Although logging is not shown in the chart above, it is used in basically every file to record some important steps of the server. `logger.h` contains the schema of the logger class. It adopts singleton design pattern to make sure there is only one running instance of the logger. The logger has a specific method for different level the of severity and it has a specific logging function for HTTP request.

`void logTraceHTTPrequest(request http_request, tcp::socket& m_socket);`

This method above the signature for logging HTTP class.

### HTTP Folder
This folder contains several classes related to HTTP (not shown on the graph):

* `header.h`: Defines struct for common header format
* `request.h`: Defines struct for HTTP requests
* `reply.h`: Defines for HTTP replies
* `request_parser.h`: Parses HTTP requests
* `mime_types.h`: Provides methods for converting file extensions to MIME types

### server.h

Defines class `server`. A server object is initiated  

### session.h

Defines class `session`. A session object is initiated by the server upon receiving a new request.

### dispatcher.h

Defines class `RequestHandlerDispatcher`. Each server object initializes and owns a `RequestHandlerDispatcher` class. Given a `request` object, the dispatcher returns a base class pointer to a new handler object that has been registered using longest prefix matching.

### request_handler.h

Defines virtual base class `RequestHandler`. Given a `request` object, the handlers return a pointer to a new `reply` object. The current implementation includes several kinds of handlers that implement this interface:

* `request_handler_echo.h`: Defines class `RequestHandlerEcho`. It returns a `response` whose body field resembles the `request`.
* `request_handler_static.h`: Defines class `RequestHandlerEcho`. It returns a `response` containing the requested file with corresponding MIME type formats.
* `request_handler_status.h`: Defines class `RequestHandlerStatus`. It returns a `response` containing the current server status (request history, registered handlers, etc).
* `request_handler_error.h`: Defiles class `RequestHandlerError`. It returns a `response` containing a 404 HTML page. This handler is registered with path `/`, so this handler will be invoked for requests to unspecified paths.









## 2. Build, Test and Run
### Build
Use cmake to perform out-of-source build:
```bash
$ mkdir -p build
$ cd build
$ cmake ..
$ make
```

### Test

There are two ways to run tests:

1. go to the `build` directory of the project and run `make test`
2. go to the `tests` directory of the project and run `../build/bin/<test_foo>`

Currently, there are in total 6 test binary files, `config_parser_test`, `logger_test`, `reply_test`, `request_handler_test`, `request_parser_test`, `session_test`

### Tests Breakdown

#### a. Unit Test

There are now unit tests for 6 classes. The following is a description about what each of them is about.

1. reply_test: It tests whether the reply generated given a status code is valid and correct.
2. request_parser_test: It tests whether the parser can report invalid HTTP request and convert valid request string to request object with the correct headers and body.
3. config_parser_test: It tests whether the parser can correctly parse the config file to a NginxConfig object.
4. logger_test: It tests whether the string output by logger is as expected in each severity.
5. session_test: This is a mock test that tests whether the session object passes valid/invalid requests to request handler and shuts down afterwards correctly.
6. request_handler_test: It tests whether the request handler handles all types of requests (echo, static, status) correctly and returns the correct HTTP response object.

#### b. Integration Test

The integration test for this project is implemented by shell script `/tests/test.sh`. To run it separately, run it from the `/build` directory:  
```
cd build && ../tests/test.sh
```
The Integration test consists of three tests:
1. A successful echo response of a valid request
2. A failed echo response because of insufficient request headers
3. A failed echo response because of invalid request body

### Run

To run the server, run the following command from the root directory
```bash
./build/bin/server ./conf/http.conf
```
You can use your own configuration file as the argument for the program.







## 3. How to Add a Request Handler
Here we use the `StatusHandler` class as an example to demonstrate how to add a new request handler to the server.

1. Declare the new status handler, specify the url path and other attributes (if any) for `StatusHandler` in the HTTP config file
```python
# File: conf/http.conf
handler status{
    location /status;
}
```

2. Define the class for the new request handler. The `StatusHandler` class here uses public inheritance to inherit the `RequestHanlder` base class and overrides the `handleRequest` method. The header file `request_handler_status.h` should be in `include/request_handler` folder and the .cc file `request_handler_status.cc` should be in `src/request_handler` folder.
```C++
/* File: include/request_handler/request_handler_status.h */
#include <iostream>
#include "request_handler.h"

// Inhertis the base class
class RequestHandlerStatus : public RequestHandler {
public:
    // Define the constructor of RequestHandlerStatus class. In our implementation
    //  we simply do nothing, you can initialize attributes or call other methods
    //  in your own request handler
explicit RequestHandlerStatus(const NginxConfig &config) {}    
    // Override the handlerRequest method
    std::unique_ptr<reply> handleRequest(const request &request_) noexcept override;

    // Define other methods of status request handler
    std::string requestToString(const request &request_);
    ...
};
```
In the .cc file we implement the methods of `StatusHandler`:
```C++
/* File: request_handler_status.cc */
#include <iostream>
#include <cassert>

// Include the request_handler_status.h file
#include "request_handler/request_handler_status.h"
// Include other header files if needed
#include "request_handler_dispatcher.h"
#include "session.h"

std::unique_ptr<reply> RequestHandlerStatus::handleRequest(const request &request_) noexcept {
   // Implement the handleRequest method here
   ...
}

std::string RequestHandlerStatus::requestToString(const request &request_) {
   // Implement the requestToString method here
   ...
}
```
3. In `request_handler_dispatcher.h`, add the handler type for the new added status handler
```C++
/* File: include/request_handler_dispatcher.h */
// Handler types already defined
static const HandlerType StaticHandler = "static";
static const HandlerType EchoHandler =   "echo";
...
// Add HandlerType for status handler
static const HandlerType StatusHandler = "status";
```
4. Include the `request_handler_status.h` header file in `request_handler_dispatcher.cc`, add the option for `StatusHandler` in the matching of prefixs and request handlers in `RequestHandlerDispatcher::getRequestHandler`

```C++
/* File: src/request_handler/request_handler_dispatcher.cc */
// Handlers already defined
#include "request_handler_dispatcher.h"
#include "request_handler/request_handler.h"
...
// Include the new added request handler
#include "request_handler/request_handler_status.h"
...

// Add an matching option in getRequestHandler method
std::unique_ptr<RequestHandler>
RequestHandlerDispatcher::getRequestHandler(const request &request_) const {

   // Other codes
   ...

   if (handler_configs_.find(matched_prefix) == handler_configs_.end()) {
        // Failed to match to any handler
        NginxConfigStatement empty_statement;
        return std::make_unique<RequestHandlerError>(
            NginxConfig()
        );
   }
   else if (handler_configs_.find(matched_prefix)->second->tokens_[1] == StaticHandler) {
        return std::make_unique<RequestHandlerStatic>(
            *(handler_configs_.find(matched_prefix)->second->child_block_),
            matched_prefix
        );
   }
   ...

   // Add the matching of StatusHandler's prefix and class object
   else if (handler_configs_.find(matched_prefix)->second->tokens_[1] == StatusHandler) {
        return std::make_unique<RequestHandlerStatus>(
            *(handler_configs_.find(matched_prefix)->second->child_block_)
        );
   }

   // Other codes
   ...
}
```
5. Append the new handler `src/request_handler_status.cc` to the argument list of `add_library(request_handler ...)` in `CMakeLists.txt`
```python
# File: CMakeLists.txt
add_library(request_handler
            src/request_handler_dispatcher.cc
            src/request_handler/request_handler_static.cc
            src/request_handler/request_handler_echo.cc
            src/request_handler/request_handler_error.cc
            # link the new request handler class
            src/request_handler/request_handler_status.cc
            src/http/mime_types.cc)
```

6. Add unit tests for the new request handler in `test/request_handler_test.cc`
```C++
/* File: test/request_handler_test.cc */
// Other includes
...
// Include the header file of new request handler
#include "request_handler/request_handler_static.h"
...
// Other codes
...
class RequestHandlerTest : public :: testing::Test {
protected:
    // Other member variables
    ...
    // Add a NginxConfig object for the new handler
    NginxConfig status_handler_config;
    ...
}
...
// Add unit test the new handler
```

Note that the instruction above is the minimum that you have to do to build successfully. If there are more dependencies for the class, you need to include them in each step as well. Also, these libs are required:
* Boost::system
* Boost::filesystem
* Boost::regex
