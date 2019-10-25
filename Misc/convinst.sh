#!/bin/sh

./dmp2c.py pingInstrument ping.dmp ../src/cx16/ping.inc
./dmp2c.py crashInstrument crash.dmp ../src/cx16/crash.inc

