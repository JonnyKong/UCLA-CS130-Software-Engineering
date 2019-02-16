# UCLA-CS130-Software-Engineering

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