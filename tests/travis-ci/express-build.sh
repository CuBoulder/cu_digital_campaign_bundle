#!/bin/env bash

# Get branch info for
OUTPUT="$(git tag -l --sort=-taggerdate | grep 2 -m 1)"
echo "${OUTPUT}"
