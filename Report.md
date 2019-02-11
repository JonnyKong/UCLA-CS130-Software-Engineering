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