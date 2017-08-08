#!/usr/bin/env bash

# Check commit message for !==express to skip test run.
# Need to go two commit messages back because Travis merges PR branch into default branch.
OUTPUT="$(git log -2 --pretty=%B | awk '/./{line=$0} END{print line}' | grep '!==express' )"
echo "${OUTPUT}"
