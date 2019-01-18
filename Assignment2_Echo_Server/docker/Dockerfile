### Build/test container ###
FROM yijunwu-config-parser:base as builder

COPY . /usr/src/project
WORKDIR /usr/src/project/build

RUN cmake ..
RUN make
RUN ctest --output-on_failure


### Deploy container ###
FROM ubuntu:latest as deploy

# TODO(!): Copy server output binary to "."
COPY --from=builder /usr/src/project/build/bin/server .

# TODO(!): Expose some port(s)
EXPOSE 80

# Use ENTRYPOINT to specify the binary name
# TODO(!): Update with real server name
ENTRYPOINT ["./server"]

# Use CMD to specify arguments to ENTRYPOINT
# TODO(!): Update with real server args
# Maybe we should add the config file name
CMD ["config_file"]
