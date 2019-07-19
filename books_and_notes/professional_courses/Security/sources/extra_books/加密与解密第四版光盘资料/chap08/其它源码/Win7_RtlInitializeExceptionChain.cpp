void WINAPI RtlInitializeExceptionChain(_EXCEPTION_REGISTRATION_RECORD *FinalErr)
{
  struct _TEB *CurrentTeb; 

  if ( RtlpProcessECVDisabled != 1 )
  {
    CurrentTeb = NtCurrentTeb();
    FinalErr->Next = (struct _EXCEPTION_REGISTRATION_RECORD *)-1;
    FinalErr->Handler = FinalExceptionHandler;
    if ( CurrentTeb->NtTib.ExceptionList == (_EXCEPTION_REGISTRATION_RECORD *)-1 )
    {
      CurrentTeb->NtTib.ExceptionList = FinalErr;
      CurrentTeb->SameTebFlags |= 0x200u;
    }
  }
}