# 1. Use the exact same OS you are using locally (Debian 12 Bookworm)
FROM debian:bookworm

# 2. Install all the necessary build tools and dependencies
# (We add 'rm -rf /var/lib/apt/lists/*' at the end to keep the Docker image size small)
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libcurl4-openssl-dev \
    libssl-dev \
    libasio-dev \
    && rm -rf /var/lib/apt/lists/*

# 3. Download, build, and globally install CPR
WORKDIR /tmp
RUN git clone https://github.com/libcpr/cpr.git && \
    cd cpr && \
    cmake -S . -B build -DCPR_USE_SYSTEM_CURL=ON && \
    cmake --build build -j$(nproc) && \
    cmake --install build && \
    cd .. && rm -rf cpr

# 4. Download and globally install Crow
RUN git clone https://github.com/CrowCpp/Crow.git && \
    cd Crow && \
    cmake -S . -B build -DCROW_BUILD_EXAMPLES=OFF -DCROW_BUILD_TESTS=OFF && \
    cmake --install build && \
    cd .. && rm -rf Crow

# 5. Set up your actual project working directory
WORKDIR /app

# 6. Copy your C++ source code and CMakeLists.txt into the container
COPY . .

RUN chmod +x compile.sh && ./compile.sh

# 4. Tell Docker which port this container will listen on
EXPOSE 8080

# 5. Run the executable that your compile.sh generated in the root of /app
# (Change "my_server_app" to whatever the actual name of your executable is)
CMD ["./app"]
