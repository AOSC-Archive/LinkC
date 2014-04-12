#!/bin/bash

make clean
rm -r Server
rm -r .deps
rm -r Makefile
rm -r src/Makefile
rm -r src/network/.deps
rm -r src/network/Makefile
rm -r src/network/p2p/.deps
rm -r src/network/p2p/Makefile
rm -r src/network/network_protocol/.deps
rm -r src/network/network_protocol/Makefile
rm -r src/utilities/db_ctl/.deps
rm -r src/utilities/list/.deps
rm -r src/utilities/user/.deps
rm -r src/utilities/user/Makefile
rm -r src/utilities/Makefile
rm -r src/utilities/db_ctl/Makefile
rm -r src/utilities/list/Makefile
