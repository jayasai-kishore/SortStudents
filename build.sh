#!/bin/bash
before_install:
  - chmod +x build.sh
  
echo "Environment: `uname -a`"
echo "Compiler: `$CXX --version`"

make || exit 1
