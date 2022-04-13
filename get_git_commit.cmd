@echo off
FOR /F "tokens=* USEBACKQ" %%F IN (`git describe --dirty --always --tags`) DO (
SET var=%%F
)
ECHO #ifndef GITHASH_H > include/pemsa/githash.h
ECHO #define GITHASH_H >> include/pemsa/githash.h
ECHO const std::string git_hash = "%var%"; >> include/my_project/githash.h
ECHO #endif >> include/pemsa/githash.h