#!/usr/bin/env bash

# Get branch info for
git fetch --tags
OUTPUT="$(git describe --tags `git rev-list --tags --max-count=1`)"
echo "${OUTPUT}"
