# Web Server API Report
## 1. Request Handler Interface

There are two types of request handlers: static handler and echo handler. The static handlers is used for the static type, and the echo handler is used to echo the client's request.  These two type of handlers both inherit a virtual class, `request_handler`.  The virtual handler only has one method which is the following:

```c++
virtual void handleRequest(const request &request_, reply *reply_) noexcept = 0;
```

The `noexcept` keyward is used to avoid crushing the server, when the request is in bad form. The handler would gracefully handle the bad request to include `404` as the response code. The parsed request is passed to the handler, and the reply is returned through the `reply_` pointer. Since the handlers do not maintain any state, only one instance of each handler is needed.

#### Echo handler

The implementation of the echo handler is straightforward. It converts request to string, and added to the body of the http response. No io function is involved in this implementation. 

#### Static handler

The static handler fetchs the file based on the uri, and it would write the file to the body of the http response.

### How is Request Handler called?

Session objects have an asynchronous method that handles incoming request. Upon receiving every incoming request, it calls `handle_read_callback` function. If the request is valid, the function would ask dispatcher for the corresponding request handler and call its `handleRequest` function. Note that this process does not create a new request handler, it simply obtains the shared pointer to the request handler.

### Request Handler Testing

The basic idea behind testing request handler is initializing a request handler object and let it handle a request object. The reply returned by the handler should be the same as what is expected. Here are illustrations and examples for testin Echo Handler and Static Handler.

#### Echo handler Test

Given a string (e.g. `char input[1024] = "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n";`), an http request object is created by the request parser's `parse` method  by calling `request_parser.parse(req, input, input + strlen(input));`. 

The request is then passed to an echo handler to generate a reply.

The final step is to check whether the headers and body of the reply returned by the echo handler is what is expected. The body of the reply should be exactly the same as the input string.

#### Static handler Test

Just like the echo handler test, an input string is given (e.g. `char input[1024] = "GET /static/data/www/data1.dat HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n";`) to generate a request by request parser. The reply returned by the static handler needs to be checked.

In this particular case, the body of the reply should be exactly the same as the content of the file. 

## 2. Config File Format
When starting the server, we will use a config file as an argument for server configuration. Currently we are using Nginx Config File Format. Below is an example of our server config file. 

```py
#exmaple http.conf file
server {

    port 80; # bind server to port 80

    # Configuration of StaticHandler
    location /static/ StaticHandler {
        root /www;
    }

    # Map another URL path to StaticHandler
    location /static2/ StaticHandler {
        root /www2;
    }

    # Configuration of EchoHandler
    location /echo/ EchoHandler {
    
    }
}
```

### 2.1 Server-Level Parameters
The server-level parameters are specified inside the *server* domain. At this stage we are only using the keyword port to bind our server to a specified port number. We plan to add more server-level parameters in the future as descibed below:
  * error_log : specify the path to the log file where errors in server starting are written to
  * error_page : specifiy an html page that displays error information for each error code

### 2.2 Request Handler Configuration
Inside the server domain we configure the request handlers. The configuration of a request handler includes three parts:
  * Keyword : we use the keyword *location* to start the configuration of a request handler
  * URL Path : the url path in HTTP request
  * Request Handler Name : the name of request handler class mapped to the URL path. Parameters of request handler can be set inside the braces after the hanlder name.

For example, when we configure the static file handler, we will use the statement below. /static/ is the URL path included in HTTP request and `StaticHandler` is the request handler class we will use. The root parameter here is the prefix of the directory path that we will search for the file in static file serving.
```py
    # Configuration of StaticHandler
    location /static/ StaticHandler {
        root /www;
    }
```

When our config parser reads the config file, it will parse the config file to a `NginxConfig` object, and the `RequestHandlerDispatcher` will read the `NginxConfig` object and record the mappings between URL path and request handlers. Details are described in Dispatch Mechanism section.



## 3. Dispatch Mechanism
The `RequestHandlerDispatcher` class selects which handler to dispatch for a specific request URI. It has public interface of:
```c++
RequestHandlerDispatcher(const NginxConfig &config);
virtual std::shared_ptr<RequestHandler> getRequestHandler(const request &request_) const;
```

### 3.1 Implementation
`RequestHandlerDispatcher` uses a `std::map<PathUri, std::shared_ptr<RequestHandler>>` structure to store request handlers. The following design decisions are made:  
* **Singleton**: One dispatcher for each server
  * Handlers won't be destructed until server gets destructed
  * Each session accesses the same dispatcher object
  * The server doesn't have to initialize new dispatchers after initialization
* **Immutability**: Handler objects become immutable after initialization
  * Don't have to worry about handler states
  * Shared states (e.g. root directory of static file handlers) are immutable
  * States that are specific to a session (e.g. file buffers in static handlers) are instantiated on stack. No concurrency issues because there's no race condition

### 3.2  Initialization
Each `server` object contains a `RequestHandlerDispatcher` object, and the dispatcher is initialized during server initialization:  
1. Search for valid "location" blocks in the config file
2. For each of these blocks. initialize a specified request handler, and store its pointer in the map indexed by the URI it provided. This step is also referred to as "URI registering"

### 3.3 Dispatching
Dispatching is handled by the `getRequestHandler()` function, and it's called by a `session` object. For each incoming request, the dispatcher does longest prefix matching for the request's URI within the registered prefixes:  
* **Match**: The dispatcher returns the pointer to the request handler object responsible for this request. 
* **No Match**: The dispatcher returns `nullptr`, and expects `session` to handle this invalid request 
  * The intuition behind this design is that the dispatcher shouldn't assume how invalid requests should be handled.  

Note that `getRequestHandler()` is declared to be a `const` function to enforce the design decision that request handlers are immutable.