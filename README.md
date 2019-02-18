# UCLA-CS130-Software-Engineering

## Getting Started

### Overall Structure of Source Code

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

### How to Add a Request Handler
Here we use the `StatusHandler` class as an example to demonstrate how to add a new request handler to the server.

1. Declare the new status handler, specify the url path and other attributes (if any) for `StatusHandler` in the HTTP config file
```python
#http.conf 

#add the declaration of new status request handler
handler status{
	location /status;
}
```
   
2. Define the class for the new request handler. The `StatusHandler` class here uses public inheritance to inherit the `RequestHanlder` base class and overrides the `handleRequest` method. The header file `request_handler_status.h` should be in `include\request_handler` folder and the .cc file `request_handler_status.cc` should be in `src\request_handler` folder.
```C++
/**
 * request_handler_status.h
 */

#ifndef REQUEST_HANDLER_STATUS_H
#define REQUEST_HANDLER_STATUS_H

#include <iostream>
//include the request handler base class header file
#include "request_handler.h"

//inhertis the Request Handler base class
class RequestHandlerStatus : public RequestHandler {
public:
    //Define the constructor of RequestHandlerStatus class. In our implementation we simply do nothing, you can initialize attributes or call other methods in your own request handler
    explicit RequestHandlerStatus(const NginxConfig &config) {}    
    //override the handlerRequest method
    std::unique_ptr<reply> handleRequest(const request &request_) noexcept override;

   //define other methods of status request handler 
    std::string requestToString(const request &request_);
    ...

};

#endif  // REQUEST_HANDLER_STATUS_H
```
In the .cc file we implement the methods of `StatusHandler`
```C++
/**
 * request_handler_status.cc
 */
#include <iostream>
#include <cassert>

//include the request_handler_status.h file
#include "request_handler/request_handler_status.h"
//include other header files if needed
#include "request_handler_dispatcher.h"
#include "session.h"

std::unique_ptr<reply> RequestHandlerStatus::handleRequest(const request &request_) noexcept {
   //implement the handleRequest method here
   ...
}

std::string RequestHandlerStatus::requestToString(const request &request_) {
   //implement the requestToString method here
   ...
}
```
3. In `request_handler_dispatcher.h`, add the handler type for the new added status handler 
```C++
/* Handler types */
static const HandlerType StaticHandler =    "static";
static const HandlerType EchoHandler =      "echo";
//add HandlerType for status handler
static const HandlerType StatusHandler =      "status";
```
4. Include the `request_handler_status.h` header file in `request_handler_dispatcher.cc`, add the option for `StatusHandler` in the matching of prefixs and request handlers in `RequestHandlerDispatcher::getRequestHandler`

```C++
/**
 * request_handler_dispatcher.cc
 */
#include <iostream>

#include "request_handler_dispatcher.h"
#include "request_handler/request_handler.h"
#include "request_handler/request_handler_static.h"
#include "request_handler/request_handler_echo.h"
#include "request_handler/request_handler_error.h"
//include the new added request handler
#include "request_handler/request_handler_status.h"

...

//Add an matching option in getRequestHandler method
std::unique_ptr<RequestHandler>
RequestHandlerDispatcher::getRequestHandler(const request &request_) const {

   //other codes
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
   else if (handler_configs_.find(matched_prefix)->second->tokens_[1] == EchoHandler) {
        return std::make_unique<RequestHandlerEcho>(
            *(handler_configs_.find(matched_prefix)->second->child_block_)
        );
   }
   // add the matching of StatusHandler's prefix and class object 
   else if (handler_configs_.find(matched_prefix)->second->tokens_[1] == StatusHandler) {
        return std::make_unique<RequestHandlerStatus>(
            *(handler_configs_.find(matched_prefix)->second->child_block_)
        );
   }

   //other codes
   ...
}
```

### Prerequisites

```
Boost::system,Boost::filesystem, Boost::regex
```

### Installing

A step by step series of examples that tell you how to get a development env running

Say what the step will be

To build the project, run these following commands from the root directory:

```
$ mkdir build (only do this if build directory has not been created)
$ cd build
$ cmake ..
$ make
```

### CMakeLists

The CMakeLists file specifies rules that connect all components of the project. If you would like to add your own component, here is the instruction on how to do so:

1. Add an independent class and test for that class,

   `add_library(new_class src/new_class.cc)`

   Append `new_class` to the argument list of  `target_link_libraries(server ...)`  

   `add_executable(new_class_test tests/new_class_test.cc)`

   `target_link_libraries(new_class_test new_class gtest_main)`

2. Add a new request handler and test for that handler

   Append `src/new_handler.cc` to the argument list of  `add_library(request_handler ...)`

   `target_link_libraries(new_handler_test new_handler gtest_main)`

3. Include new test in the test coverage report

   `gtest_discover_tests(new_class_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)`

   Append `new_class_test` to the argument list of `generate_coverage_report(TARGETS ...)`

Note that the instruction above is the minimum that you have to do to build successfully. If there are more dependencies for the class, you need to include them in each step as well.



## Running the existing tests

There are two ways to run tests: 

1. go to the `build` directory of the project and run `make test` 
2. go to the `tests` directory of the project and run `../build/bin/certain_test` (replace `certain_test` with the specific test file that you want to run)

Currently, there are in total 6 test binary files, `config_parser_test`, `logger_test`, `reply_test`, `request_handler_test`, `request_parser_test`, `session_test`

### Tests Breakdown

#### Unit Test

There are now unit tests for 6 classes. The following is a description about what each of them is about.

1. reply_test: It tests whether the reply generated given a status code is valid and correct.
2. request_parser_test: It tests whether the parser can report invalid HTTP request and convert valid request string to request object with the correct headers and body.
3. config_parser_test: It tests whether the parser can correctly parse the config file to a NginxConfig object.
4. logger_test: It tests whether the string output by logger is as expected in each severity.
5. session_test: This is a mock test that tests whether the session object passes valid/invalid requests to request handler and shuts down afterwards correctly. 
6. request_handler_test: It tests whether the request handler handles all types of requests (echo, static, status) correctly and returns the correct HTTP response object.

#### Integration Test

The integration test for this project is implemented by shell script`/tests/test.sh`. To run it separately, just run `./tests/test.sh` from the root directory. The Integration test now consists of three tests:

1. A successful echo response of a valid request
2. A failed echo response because of insufficient request headers
3. A failed echo response because of invalid request body

## Deployment

To run the server, run the following command from the root directory

`./build/bin/server ./conf/http.conf`

You can use your own configuration file as the argument for the program.

## Built With

- [Dropwizard](http://www.dropwizard.io/1.0.2/docs/) - The web framework used
- [Maven](https://maven.apache.org/) - Dependency Management
- [ROME](https://rometools.github.io/rome/) - Used to generate RSS Feeds

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags). 

## Authors

- **Billie Thompson** - *Initial work* - [PurpleBooth](https://github.com/PurpleBooth)

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

- Hat tip to anyone whose code was used
- Inspiration
- etc