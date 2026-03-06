#!/usr/bin/env bash

# Set -euo pipefail:  Exit immediately if a command exits with a non-zero status.
# -e: Exit if a command exits with a non-zero status.
# -u: Treat unset variables as an error.
# -o pipefail:  If a command in a pipeline fails, the whole pipeline fails.
set -euo pipefail

curl -X PUT http://localhost:8080/users/1 \
	-H "Content-Type: application/json" \
	-H "Authorization: FakeHackerToken" \
	-d '{"username":"alice_update","age":"22"}'

exit 0
