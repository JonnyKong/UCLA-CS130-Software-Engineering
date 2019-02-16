# UCLA-CS130-Software-Engineering

## Getting Started

### Overall Structure of Source Code

```
                    +----------------------------+
                    |        server_main.h       +-----------------------+
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

TODO brief explaination of the program, descritiption

### Logging

### HTTP Folder
This folder contains several classes related to HTTP (not shown on the graph):

* `header.h`: Defines struct for common header format
* `request.h`: Defines struct for HTTP requests
* `reply.h`: Defines for HTTP replies
* `request_parser.h`: Parses HTTP requests 
* `mime_types.h`: Provides methods for converting file extensions to MIME types

### server.h

Defines class `session`. A server object is initiated  

### session.h

Defines class `session`. A session object is initiated by the server upon receiving a new request.

### dispatcher.h

Defines class `RequestHandlerDispatcher`. Each server object initializes and owns a `RequestHandlerDispatcher` class. Given a `request` object, the dispatcher returns a base class pointer to a new handler object that has been registered using longest prefix matching.

### request_handler.h

Defines virtual base class `RequestHandler`. Given a `request` object, the handlers returns a pointer to a new `reply` object. Current implementation includes several kinds of handlers that implements this interface:

* `request_handler_echo.h`: Defines class `RequestHandlerEcho`. It returns a `response` whose body field resembles the `request`.
* `request_handler_static.h`: Defines class `RequestHandlerEcho`. It returns a `response` containing the requested file with corresponding MIME type formats.
* `request_handler_status.h`: Defines class `RequestHandlerStatus`. It returns a `response` containing the current server status (request history, registered handlers, etc).
* `request_handler_error.h`: Defiles class `RequestHandlerError`. It returns a `response` containing a 404 html page. This handler is registered with path `/`, so this handler will be invoked for requests to unspecified paths.

### Prerequisites

What things you need to install the software and how to install them

```
Give examples
```

### Installing

A step by step series of examples that tell you how to get a development env running

Say what the step will be

```
Give the example
```

And repeat

```
until finished
```

End with an example of getting some data out of the system or using it for a little demo

## Running the tests

Explain how to run the automated tests for this system

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

### And coding style tests

Explain what these tests test and why

```
Give an example
```

## Deployment

Add additional notes about how to deploy this on a live system

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