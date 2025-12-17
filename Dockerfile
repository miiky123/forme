FROM gcc:latest
# Base image providing GCC and a Linux environment

RUN apt-get update && apt-get install -y cmake
# Install CMake for configuring and generating build files

COPY . /usr/src/project_mkh
# Copy the entire project directory into the container

WORKDIR /usr/src/project_mkh
# Set working directory to the project root

# Directory where files will be stored by FileStorage
ENV RLE_STORAGE_DIR=/usr/src/project_mkh/data

# Make sure the directory exists inside the container
RUN mkdir -p ${RLE_STORAGE_DIR}

# Create a build directory (out-of-source build)
RUN mkdir build

# Enter the build directory
WORKDIR /usr/src/project_mkh/build

# Configure the project with CMake, then build it with Make
RUN cmake .. && make

CMD ["bash"]

FROM gcc:latest AS server
# Image for running the built server

WORKDIR /usr/src/project_mkh
# Copy everything from the first (unnamed) build stage (index 0)
COPY --from=0 /usr/src/project_mkh /usr/src/project_mkh

ENV RLE_STORAGE_DIR=/usr/src/project_mkh/data
WORKDIR /usr/src/project_mkh/build

# The server executable name is "project_mkh"
# (defined in CMakeLists.txt by: add_executable(project_mkh ...))
CMD ["./project_mkh"]


FROM gcc:latest AS client_cpp
# Separate lightweight image for the standalone C++ client

WORKDIR /usr/src/client
# Client workspace

COPY clients/client.cpp .
# Copy only the C++ client source (no server code)

RUN g++ -std=c++17 client.cpp -o client
# Build the client executable

CMD ["./client"]
# Server IP + port will be passed from console (argv)



FROM python:3.11-slim AS client_py
# Lightweight Python environment for the Python client

WORKDIR /usr/src/client
# Client workspace

COPY clients/Pclient.py .
# Copy Python client

CMD ["python", "Pclient.py"]
# Server IP + port will be passed from console (argv)
