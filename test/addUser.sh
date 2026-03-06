#!/usr/bin/env bash


# Set -euo pipefail:  Exit immediately if a command exits with a non-zero status.
# -e: Exit if a command exits with a non-zero status.
# -u: Treat unset variables as an error.
# -o pipefail:  If a command in a pipeline fails, the whole pipeline fails.
set -euo pipefail

curl -X POST http://localhost:8080/users \
     -H "Content-Type: application/json" \
     -d '{"username":"alice","age":"22"}'


exit 0
