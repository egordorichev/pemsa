#!/bin/bash
hash=$(git describe --dirty --always --tags)
echo "#ifndef GITHASH_H" > include/pemsa/githash.h
echo "#define GITHASH_H" >> include/pemsa/githash.h
echo "const std::string git_hash = \"$hash\";" >> include/pemsa/githash.h
echo "#endif" >> include/pemsa/githash.h