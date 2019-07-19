VOID
    LdrpInitialize (
       IN PCONTEXT Context,
       IN PVOID SystemArgument1,
       IN PVOID SystemArgument2
    )

    Routine Description:

       This function is called as a User-Mode APC routine as the first
       user-mode code executed by a new thread. It's function is to initialize
       loader context, perform module initialization callouts...
……
//#if DBG
        if (TRUE)
//#else
//        if (Peb->BeingDebugged || Peb->ReadImageFileExecOptions)
//#endif
        {
            PWSTR pw;

            pw = (PWSTR)Peb->ProcessParameters->ImagePathName.Buffer;
            if (!(Peb->ProcessParameters->Flags & RTL_USER_PROC_PARAMS_NORMALIZED)) {
                pw = (PWSTR)((PCHAR)pw + (ULONG_PTR)(Peb->ProcessParameters));
                }
            UnicodeImageName.Buffer = pw;
            UnicodeImageName.Length = Peb->ProcessParameters->ImagePathName.Length;
            UnicodeImageName.MaximumLength = UnicodeImageName.Length;

            //
            //  Hack for NT4 SP4.  So we don't overload another GlobalFlag
            //  bit that we have to be "compatible" with for NT5, look for
            //  another value named "DisableHeapLookaside".
            //

            LdrQueryImageFileExecutionOptions( &UnicodeImageName,
                                               L"DisableHeapLookaside",
                                               REG_DWORD,
                                               &RtlpDisableHeapLookaside,
                                               sizeof( RtlpDisableHeapLookaside ),
                                               NULL
                                             );

            st = LdrQueryImageFileExecutionOptions( &UnicodeImageName,
                                                    L"GlobalFlag",
                                                    REG_DWORD,
                                                    &Peb->NtGlobalFlag,
                                                    sizeof( Peb->NtGlobalFlag ),
                                                    NULL
                                                  );
            if (!NT_SUCCESS( st )) {

                if (Peb->BeingDebugged) {
		// 
		// 这里改写了 NtGlobalFlag
		//
                    Peb->NtGlobalFlag |= FLG_HEAP_ENABLE_FREE_CHECK |
                                         FLG_HEAP_ENABLE_TAIL_CHECK |
                                         FLG_HEAP_VALIDATE_PARAMETERS;
                    }
                }
