#!/usr/bin/env python
import os
import sys
import re
lockupdict = {};
if len(sys.argv) != 2:
    print "Usage: conv.py file_name"
    sys.exit(1)

f=open(sys.argv[1], 'rb')
lines = f.readlines();
for i in lines:
    splitList = re.split(' ', i);
    print splitList
    lockupdict[int(splitList[0], 16)]= splitList[1]

for i in range(0, 127):
    if (lockupdict.has_key(i)):
        print "'" + lockupdict[i][:-1] + "'/*%d*/, \\" % i
    else:
        print 'EMPTY /*%d*/, \\' % i
