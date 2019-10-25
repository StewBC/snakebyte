#!/usr/bin/env python3

# Copyright (c) 2019, Frank Buss
# All rights reserved.
# based on a PHP script by Barry Yost

# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:

# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.

# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.

# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# C example code how to use it:
#
# include "instrument.inc"
#
# static void loadInstrument(uint8_t channel, uint8_t* data)
# {
#     uint8_t i;
#     uint8_t ofs = 0;
#     writeYM2151Reg(0x20 + channel, data[ofs++]);
#     writeYM2151Reg(0x38 + channel, data[ofs++]);
#     for (i = 0; i < 4; i++) {
#         writeYM2151Reg(0x40 + channel, data[ofs++]);
#         writeYM2151Reg(0x60 + channel, data[ofs++]);
#         writeYM2151Reg(0x80 + channel, data[ofs++]);
#         writeYM2151Reg(0xa0 + channel, data[ofs++]);
#         writeYM2151Reg(0xc0 + channel, data[ofs++]);
#         writeYM2151Reg(0xe0 + channel, data[ofs++]);
#     }
# }
#
# void audioInit()
# {
#     uint16_t i;
#     for (i = 0; i < 256; i++) {
#         writeYM2151Reg(i, 0);
#     }
#     loadInstrument(0, arrayName);
# }

import sys
import argparse

# parse arguments
parser = argparse.ArgumentParser(formatter_class=argparse.RawTextHelpFormatter,
    description='Converts a DefleMask DMP instrument file to a C array.')
parser.add_argument('name', help='the C array name. If it is "bin", then a binary file is saved.')
parser.add_argument('input', help='the input file name')
parser.add_argument('output', help='the output file name')
args = parser.parse_args()

# read instrument file
dmp = open(args.input, "rb").read()

# test file
MAXSIZE = 0x33
FILE_VERSION = 0x0b
SYSTEM_YM2151 = 0x08
MODE_FM = 0x01
if len(dmp) > MAXSIZE:
  sys.exit("File is too large to be a DMP file.")
if dmp[0] != FILE_VERSION:
    print("WARNING: The first byte is not 0x%02x" % FILE_VERSION)
if dmp[1] != SYSTEM_YM2151:
  sys.exit("FATAL: This is not a YM2151 patch.")
if dmp[2] != MODE_FM:
   sys.exit("FATAL: The instrument mode is not set to FM.");

# convert to array
PMS = dmp[3] & 0x07
FB = dmp[4] & 0x07
CONNECT = dmp[5] & 0x07
AMS = dmp[6] & 0x03
val20 = 0xc0 | (FB << 3) | CONNECT
val38 = (PMS << 4) | AMS
result = [val20, val38]
dt1map = [7, 6, 5, 0, 1, 2, 3, 4]
for oper in range(4):
    offset = 7 + oper * 11
    MUL = dmp[offset+0] & 0x0f
    TL = dmp[offset+1] & 0x7f
    AR = dmp[offset+2] & 0x1f
    D1R = dmp[offset+3] & 0x1f
    D1L = dmp[offset+4] & 0x0f
    RR = dmp[offset+5]  & 0x0f
    AMSEN = dmp[offset+6] & 0x01
    KS = dmp[offset+7] & 0x03
    DT1 = dt1map[dmp[offset+8] & 0x07]
    DT2 = (dmp[offset+8] >> 4) & 0x03
    D2R = dmp[offset+9]  & 0x1f
    SSGEG = dmp[offset+10]
    val40 = (DT1 << 4) | MUL
    val60 = TL
    val80 = (KS << 6) | AR
    valA0 = (AMSEN << 7) | D1R
    valC0 = (DT2 << 6) | D2R
    valE0 = (D1L << 4) | RR
    result = result + [val40, val60, val80, valA0, valC0, valE0]

# save file
if args.name == 'bin':
    with open(args.output, "wb") as out:
        out.write(bytearray(result))
else:
    with open(args.output, "w") as out:
        out.write("uint8_t %s[] = {\n" % args.name)
        i = 0
        for b in result:
            out.write("0x%02x," % b)
            i = i + 1
            if i == 16:
                out.write("\n")
                i = 0
        out.write("};\n")
