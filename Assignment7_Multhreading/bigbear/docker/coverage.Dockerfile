### Generate test coverage ###
# Define builder stage
FROM bigbear:base as builder

# Share work directory
COPY . /usr/src/projects/bigbear
WORKDIR /usr/src/projects/bigbear/build

RUN cmake ..
RUN make

WORKDIR /usr/src/projects/bigbear/build_coverage

# Build and test
RUN cmake -DCMAKE_BUILD_TYPE=Coverage ..
RUN make coverage

