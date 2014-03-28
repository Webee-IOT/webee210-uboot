#!/bin/sh
#install Netpbm first
jpegtopnm $1 | ppmquant 31 | ppmtobmp -bpp 8 > $2
