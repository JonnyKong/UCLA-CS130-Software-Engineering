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

Although logging is not shown in the chart above, it is used in basically every file to record some important steps of the server. logger.h contains the schema of the logger class. It adopts singleton design pattern to make sure there is only one running instance of the logger. The logger has a specific method for different level the of severity and it has a specific logging function for HTTP request.

### HTTP Folder

### `session.h`

### `server.h`

### `dispatcher.h`

### `request_handler.h`



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