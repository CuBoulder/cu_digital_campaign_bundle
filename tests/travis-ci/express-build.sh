#!/usr/bin/env bash

# Get branch info for
cd $HOME/drupal/profiles/express

OUTPUT="$(git tag -l --sort=-taggerdate | grep 2 -m 1)"
echo "${OUTPUT}"
