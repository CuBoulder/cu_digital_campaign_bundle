#!/usr/bin/env bash

# Check commit message for !==express to skip test run.
# Need to go two commit messages back because Travis merges PR branch into default branch.
OUTPUTTT="$(git log -2 --pretty=%B | awk '/./{line=$0} END{print line}' | grep !==express)"
echo "${OUTPUTTT}"

# If !==express is in the commit message, then don't run Express tests.
if [ $(git log -2 --pretty=%B | awk '/./{line=$0} END{print line}' | grep !==express) ]; then
  echo "found !==express"
else
  echo "did not find it"
fi
