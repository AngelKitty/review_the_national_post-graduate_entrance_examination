#ifndef __ASSEMBLY__H__
#define __ASSEMBLY__H__

ULONG _GetCr0();
ULONG _GetCr2();
ULONG _GetCr3();
ULONG _GetCr4();
ULONG _GetEflags();
NTSTATUS _MakeHyperExitCall();
USHORT _GetLdtrSelector();
USHORT _GetTrSelector();
ULONG _GetGdtBase();
USHORT _GetGdtLimit();
USHORT _GetIdtLimit();
VOID _ExecuteInvd();
VOID _SetIdtr( ULONG Base, ULONG Limit );
ULONG64 _WriteMsr(ULONG MSRIndex,ULONG LowPart,ULONG HighPart);
USHORT _GetCsSelector();
USHORT _GetDsSelector();
USHORT _GetEsSelector();
USHORT _GetFsSelector();
USHORT _GetGsSelector();
USHORT _GetSsSelector();

VOID _ExecuteVmxOn( ULONG PtrLowPart, ULONG PtrHighPart );
VOID _ExecuteVmPtrLd( ULONG PtrLowPart, ULONG PtrHighPart );
VOID _ExecuteVmClear( ULONG PtrLowPart, ULONG PtrHighPart );
VOID _WriteVMCS( ULONG Field, ULONG Value );
ULONG _ReadVMCS( ULONG Field );
ULONG _ExecuteVmLaunch( VOID );
ULONG _VmLaunchFailValid( VOID );

VOID _ExecuteVmxOff( ULONG_PTR RegEsp, ULONG_PTR RegEip );
VOID _GuestExitPoint(VOID);
VOID _GuestEntryPoint( PVOID _GuestEsp );

ULONG _VmFailInvalid();
ULONG _GetIdtBase();
ULONG64 _ReadMsr( ULONG Index );
VOID _SetCr2( ULONG_PTR ulValue );
VOID _SetCr4( ULONG_PTR ulValue );

NTSTATUS __stdcall _LoadHypervisor(ULONG ulValue );

VOID __stdcall _ExecuteCpuId( ULONG fn, OUT PULONG ret_eax, OUT PULONG ret_ebx, OUT PULONG ret_ecx, OUT PULONG ret_edx );
ULONG __stdcall _ExecuteVmcall( ULONG eax, ULONG ebx, ULONG ecx, ULONG edx );
VOID __stdcall _InvVpidAllContext();
VOID _InvEptAllContext();
ULONG _AttachGuestProcess();
VOID _DetachTargetProcess( ULONG OldCr3);
#endif __ASSEMBLY__H__