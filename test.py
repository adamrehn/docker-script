#!/usr/bin/env docker-script
#!python:latest python
import os, sys

details = os.uname()

print('Uname:    ', details.sysname, details.release, details.machine)
print('Guest CWD:', os.getcwd())
print('Host CWD: ', os.environ['HOST_CWD'])
print('argv:     ', sys.argv)
