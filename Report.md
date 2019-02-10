# Web Server API Report
## 1. Request Handler Interface

## 2. Config File Format

## 3. Dispatch Mechanism
The `RequestHandlerDispatcher` class selects which handler to dispatch for a specific request URI. It has public interface of:
```
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
Dispatching is handled by the `getRequestHandler()` function, and it's called by. For each incoming request, the dispatcher does longest prefix matching for the request's URI within the registered prefixes:  
* If there's match, the dispatcher returns the pointer to the request handler object. 
* Otherwise, the dispatcher returns `nullptr`, and expects `session` to handle this invalid request 
  * The intuition behind this design is that the dispatcher shouldn't assume how invalid requests should be handled.  

Note that `getRequestHandler()` is declared to be a `const` function to enforce the design decision that request handlers are immutable.