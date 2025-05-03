#!/usr/bin/env python3
import sys,struct,os
out,*files=sys.argv[1:]
img=b''
for f in files:
    data=open(f,'rb').read()
    name=os.path.basename(f).encode()[:32]
    name+=b'\0'*(32-len(name))
    img+=name+struct.pack('<I',len(data))+data
img+=b'\0'*32+struct.pack('<I',0)
open(out,'wb').write(img)
