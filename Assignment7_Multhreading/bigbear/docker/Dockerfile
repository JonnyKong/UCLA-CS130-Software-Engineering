### Build/test container ###
# Define builder stage
FROM bigbear:base as builder

# Share work directory
COPY . /usr/src/projects/bigbear
WORKDIR /usr/src/projects/bigbear/build

# Build and test
RUN cmake ..
RUN make
RUN ctest --output-on_failure


### Deploy container ###
# Define deploy stage
FROM ubuntu:latest as deploy

# TODO(!): Copy server output binary to "."
COPY --from=builder /usr/src/projects/bigbear .
WORKDIR /usr/src/projects/bigbear/build
COPY --from=builder /usr/src/projects/bigbear/static_files /usr/src/projects/bigbear/static_files
COPY --from=builder /usr/src/projects/bigbear/build/bin/server .
COPY --from=builder /usr/src/projects/bigbear/sample_server_config .

# TODO(!): Expose some port(s)
EXPOSE 80
EXPOSE 81
EXPOSE 5566

# Use ENTRYPOINT to specify the binary name
# TODO(!): Update with real server name
ENTRYPOINT ["./server"]

# Use CMD to specify arguments to ENTRYPOINT
# TODO(!): Update with real server args
CMD ["sample_server_config"]
