LPVOID
GetHeap(
    SIZE_T nSize
    )
{
    return HeapAlloc(GetProcessHeap(), NULL, nSize);
}

BOOL
IsDebugHeap(
    VOID
)
{
    LPVOID HeapPtr;
    PDWORD ScanPtr;
    ULONG nMagic = 0;

    HeapPtr = GetHeap(0x100);

    ScanPtr = (PDWORD)HeapPtr;
    try {
        for(;;) {
            switch (*ScanPtr++) {
                case 0xABABABAB:
                case 0xBAADF00D:
                case 0xFEEEFEEE:
                    nMagic++;
                    break;
            }
        }
    }
    catch(...) {        
        return (nMagic > 10) ? TRUE : FALSE;
    }
}
