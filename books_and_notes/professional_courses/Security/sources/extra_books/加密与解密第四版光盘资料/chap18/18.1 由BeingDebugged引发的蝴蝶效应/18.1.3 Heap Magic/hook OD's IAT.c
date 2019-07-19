//coded by okdodo
//for hook OD's IAT
//----------------------------------------------------------------------------
void WINAPI HookOneAPI(LPCTSTR ModuleName,LPCTSTR ApiName,PROC NewFuction,PROC&OldFuction)
{
    DWORD size ;
    HMODULE hInstance=GetModuleHandle(NULL);
    PROC HookAPIAddr=GetProcAddress(GetModuleHandle(ModuleName),ApiName);
    OldFuction=HookAPIAddr ;
    PIMAGE_IMPORT_DESCRIPTOR pImportDesc=(PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData (hInstance,TRUE,IMAGE_DIRECTORY_ENTRY_IMPORT,&size);
    if(pImportDesc==NULL)return ;
    for(;pImportDesc->Name;pImportDesc++)
    {
        LPSTR pszDllName=(LPSTR)((PBYTE)hInstance+pImportDesc->Name);
        if(lstrcmpiA(pszDllName,ModuleName)==0)break ;
    }
    if(pImportDesc->Name==NULL)return ;
    PIMAGE_THUNK_DATA pThunk=(PIMAGE_THUNK_DATA)((PBYTE)hInstance+pImportDesc->FirstThunk);
    for(;pThunk->u1.Function;pThunk++)
    {
        PROC*ppfn=(PROC*)&pThunk->u1.Function ;
        if(*ppfn==HookAPIAddr)
        {
            MEMORY_BASIC_INFORMATION mbi ;
            ZeroMemory(&mbi,sizeof(MEMORY_BASIC_INFORMATION));
            VirtualQuery(ppfn,&mbi,sizeof(MEMORY_BASIC_INFORMATION));
            VirtualProtect(mbi.BaseAddress,mbi.RegionSize,PAGE_READWRITE,&mbi.Protect);
            *ppfn=*NewFuction ;
            DWORD dwOldProtect ;
            VirtualProtect(mbi.BaseAddress,mbi.RegionSize,mbi.Protect,&dwOldProtect);
            return ;
        }
    }
}


void fake_heap(DWORD dwProcessId,int byt)
{
    HANDLE hProcess ;
    t_thread *t_main_thread ;
    MEMORY_BASIC_INFORMATION mbi_thunk ;
    
    hProcess=OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwProcessId);
    if(hProcess!=NULL)
    {

                t_main_thread=Findthread(Plugingetvalue(VAL_MAINTHREADID));
                if(t_main_thread!=NULL)
                {
                        DWORD dw_tmp=t_main_thread->reg.base[4];
                        Readmemory(&dw_tmp,(dw_tmp+0x18),4,MM_RESTORE);         //TEB
                        Readmemory(&dw_tmp,(dw_tmp+0x30),4,MM_RESTORE);         //PEB
                        Writememory(&byt,(dw_tmp+0x02),1,MM_RESTORE);           //DebugPresent
                }
    }
 
    if(hProcess!=NULL) CloseHandle(hProcess);

}

BOOL WINAPI MyContinueDebugEvent(DWORD dwProcessId,DWORD dwThreadId,DWORD dwContinueStatus)
{
    if(dwContinueStatus==DBG_CONTINUE)
    {
        if(g_lpDebugEvent->dwDebugEventCode==LOAD_DLL_DEBUG_EVENT)
        {
            if(!bl_heap_faked)
            {
                fake_heap(dwProcessId,0);       //hide
                bl_heap_faked=true ;
            }
            else
            {
                if(!bl_heap_un_faked)
                {
                    fake_heap(dwProcessId,1);
                    bl_heap_un_faked=true ;
                }
            }
            Tls_Show((DWORD)g_lpDebugEvent->u.LoadDll.lpBaseOfDll);
        }

        else if(g_lpDebugEvent->dwDebugEventCode==EXCEPTION_DEBUG_EVENT)
        {
            if(bl_heap_un_faked)
            {
                fake_heap(dwProcessId,0);     //hide
            }
        }

        else if(g_lpDebugEvent->dwDebugEventCode==OUTPUT_DEBUG_STRING_EVENT)
        {
            dwContinueStatus=DBG_EXCEPTION_NOT_HANDLED;
        }

        else if(g_lpDebugEvent->dwDebugEventCode==RIP_EVENT)
        {
            dwContinueStatus=DBG_EXCEPTION_NOT_HANDLED;
        }

    }
    return pContinueDebugEvent(dwProcessId,dwThreadId,dwContinueStatus);
}

BOOL WINAPI MyWaitForDebugEvent(LPDEBUG_EVENT lpDebugEvent, DWORD dwMilliseconds)
{

  g_lpDebugEvent=lpDebugEvent;

  BOOL bRet = pWaitForDebugEvent(lpDebugEvent,dwMilliseconds);


  switch(lpDebugEvent->dwDebugEventCode) 
  {
    case EXCEPTION_DEBUG_EVENT:
    {    
    // 第1个int 3调试事件
    if(EXCEPTION_BREAKPOINT == lpDebugEvent->u.Exception.ExceptionRecord.ExceptionCode)
    {
        ;
    }

    }
    break;
    
    case EXIT_PROCESS_DEBUG_EVENT:
    {
        ;
    }
    break;

    case RIP_EVENT:
    {
        lpDebugEvent->u.Exception.ExceptionRecord.ExceptionCode=OUTPUT_DEBUG_STRING_EVENT;
        //pContinueDebugEvent(lpDebugEvent->dwProcessId,lpDebugEvent->dwThreadId,DBG_EXCEPTION_NOT_HANDLED);
    }
    break;
    
    default:
    break;
  }
  
  return bRet;
}


void HOOKIAT()
{
        PROC OriProcAddress = NULL;
        HookOneAPI("kernel32.dll","WaitForDebugEvent",(PROC)MyWaitForDebugEvent,(PROC)OriProcAddress);
        pWaitForDebugEvent = (OriWaitForDebugEvent)OriProcAddress;
        HookOneAPI("kernel32.dll","ContinueDebugEvent",(PROC)MyContinueDebugEvent,(PROC)OriProcAddress);
        pContinueDebugEvent = (OriContinueDebugEvent)OriProcAddress;
} 
 

