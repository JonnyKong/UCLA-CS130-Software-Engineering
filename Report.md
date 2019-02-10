# Web Server API Report
## 1. Request Handler Interface

There are two types of request handlers: static handler and echo handler. The static handlers is used for the static type, and the echo handler is used to echo the client's request.  These two type of handlers both inherit a virtual class, `request_handler`.  The virtual handler only has one method which is the following:

```c++
virtual void handleRequest(const request &request_, reply *reply_) noexcept = 0;
```

The `noexcept` keyward is used to avoid crushing the server, when the request is in bad form. The handler would gracefully handle the bad request to include `404` as the response code.

### How is Request Handler called?



### Request Handler Testing



## 2. Config File Format

## 3. Dispatch Mechanism