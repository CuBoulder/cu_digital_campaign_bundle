#!/bin/env bash

# Get branch info for
OUTPUT="$(git log | grep -m 1 commit)"
echo "${OUTPUT}"
