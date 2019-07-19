#coding=utf-8
##《加密与解密》第四版
##code by DarkNess0ut

import os
import sys

def Getasm(ea_from, ea_to, range1, range2):
    fp = open("code.txt","w")
    ea = ea_from
    while ea < ea_to:
        cmd = GetMnem(ea)
        if cmd == "mov" or cmd == "lea":
            opcode = Dword(NextNotTail(ea)-4)
            if opcode < 0: #opcode < 0,处理 mov  edx, [ebp-350]指令，否则处理mov  edx, [ebp+350]
                opcode = (~opcode + 1)
            Message("-> %08X %08X\n" % (ea, opcode))

            if range1 <= opcode <= range2:
                delta = opcode - range1
                MakeComm(ea, "// +0x%04X" % delta) # 加注释到IDA中
                fp.write("%08X %s\n" % (ea, GetDisasm(ea)))
        ea = NextNotTail(ea)
    fp.close()
    Message("OK!")
Getasm(0x401000,0x40F951,0x41AE68,0x0041AEC1);