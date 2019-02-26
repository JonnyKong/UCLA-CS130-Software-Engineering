### Build/test container ###
FROM rubberduck_server:base as builder

COPY . /usr/src/project
WORKDIR /usr/src/project/build


RUN cmake ..
RUN make
RUN pwd
RUN find . -name "server"
RUN ctest --output-on_failure


### Deploy container ###
FROM ubuntu:latest as deploy

# TODO(!): Copy server output binary to "."
RUN mkdir -p ./bin
COPY --from=builder /usr/src/project/build/bin/server ./bin
COPY --from=builder /usr/src/project/conf/example.conf .

# Copy static data files
COPY --from=builder /usr/src/project/data .

# TODO(!): Expose some port(s)
EXPOSE 80

# Use ENTRYPOINT to specify the binary name
# TODO(!): Update with real server name
WORKDIR "./bin"
ENTRYPOINT ["./server"]

# Use CMD to specify arguments to ENTRYPOINT
# TODO(!): Update with real server args
CMD ["../example.conf"]
