#!/usr/bin/env bash

#Deletes bpl and bc files that were last modified more than 2 hours ago
find . -maxdepth 1 -name "*.bpl" -mmin +120 -delete
find . -maxdepth 1 -name "*.bc" -mmin +120 -delete
find . -maxdepth 1 -name "*.bc-*" -mmin +120 -delete
find . -maxdepth 1 -name "*.ll" -mmin +120 -delete
find . -maxdepth 1 -name "*.dot" -mmin +120 -delete
find . -maxdepth 1 -name "*.i" -mmin +120 -delete
find . -maxdepth 1 -name "*.c" -mmin +120 -delete
