#!/usr/bin/env bash


# Set -euo pipefail:  Exit immediately if a command exits with a non-zero status.
# -e: Exit if a command exits with a non-zero status.
# -u: Treat unset variables as an error.
# -o pipefail:  If a command in a pipeline fails, the whole pipeline fails.
set -euo pipefail

# Directly compile method
# g++ main.cpp -o app -lcpr -lcurl -lssl -lcrypto

# compile using cmake 
cmake -S . -B build
cmake --build build


exit 0
