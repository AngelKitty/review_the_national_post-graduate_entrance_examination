#include <stdio.h>
#include <stdlib.h>

#ifndef _WIN32
#include <unistd.h>
#else
#include <fcntl.h>
#include <io.h>
#include <string.h>
#include <stdarg.h>
#include <tchar.h>
#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
#include <SYS\STAT.H>
#endif

#include <stdarg.h>
#include <fcntl.h>
#include <errno.h>
#define OPT_RUNAS_DAEMON 1
#define OPT_CLOSE_STDERR (1<<1)

#define MAX_MESSAGE_SIZE 1024

int g_loglvl=0;
FILE * fptr_log=NULL;
void log_init()
{
    if(fptr_log!=NULL)
        return;
    fptr_log=fopen("filetrap.log", "w");
}
void log_close()
{
    if(fptr_log!=NULL)
        fclose(fptr_log);
}
void d4d(int nlevel, const char * szFormat, ...)
{
    char msg[MAX_MESSAGE_SIZE]={0};
    va_list va;

    if(nlevel<g_loglvl)
        return;

    va_start( va, szFormat );

    _vsnprintf( msg, 
		MAX_MESSAGE_SIZE, szFormat, va );

    va_end(va);

    // write to file ...
    if(fptr_log)
       fwrite(msg, strlen(msg), 1, fptr_log);

    write(STDOUT_FILENO, (char *)msg, strlen(msg));
}
int daemon_init(unsigned int options)
{
    int i;

#ifndef _WIN32
    pid_t pid;
    
    pid=fork();
    if(pid<0) // fork failed
    {
       return -1;
    }
    else if (pid !=0 )
    {
        // parent exits 
        exit(0); 
    }
#endif
    
    if(options&OPT_CLOSE_STDERR)
    {
        for(i=0;i<=STDERR_FILENO;i++)
        {   
           d4d(0, "standard handle %d is being closed\n", i);
           close(i);
        }
    }

    return 0;
}
void usage(const char * msg)
{
    if(msg)
       puts(msg);

    puts("filetrap to triger a serious bug\n"
           "- Raymond Zhang (rev 1.0)\n\n"
           "filetrap [[-|/]d]  run as daemon (default)\n" 
           "         [[-|/]c]  run as console\n"    
           "         [[-|/]w]  take workaround\n" 
           "         [[-|/]h]  show this help\n"
        );
}
int parse_cmdline(int argc, char **argv,
           unsigned int * options)
{
    int i=1,j=0;

    while(i<argc)
    {
       if(argv[i][0]=='-'
           ||argv[i][0]=='/')
          j++; 
       switch(argv[i][j]) 
       {
       case 'h':
       case '?':
          usage(NULL);
          break; 
       case 'd': //run as daemon
          *options |= OPT_RUNAS_DAEMON;
          break;
       case 'c':
          *options &= ~OPT_RUNAS_DAEMON;
          break;
       case 'w': //workaround
          *options &= ~OPT_CLOSE_STDERR;
          break;
       case 'l':
          if( ++i >= argc )
          {
             usage( "-l missing log level value\n" );
             return -1;
          }
          g_loglvl=atoi(argv[i]);
          break;   
       default:
          d4d(0, "bad argument[%d][%d]: %s\n", i, j, argv[i]);
          break;     
       }
       i++;   j=0;
    }
    d4d(0, "got cmd line options 0x%x\n", *options);

    return 0;
}

void do_business()
{
    int    fd_a,fd_b,fd_c;

    d4d(0, "begin business logic\n");
    
    fd_a=open("file_a.txt", O_RDWR|O_CREAT|O_TRUNC);
    if(fd_a<0)
       d4d(0, "open file_a failed for %d\n", errno);       

    fd_b=open("file_b.txt", O_RDWR|O_CREAT);
    if(fd_b<0)
       d4d(0, "open file_b failed for %d\n", errno);       
    
    fd_c=open("file_c.txt", O_RDWR|O_CREAT);
    if(fd_c<0)
       d4d(0, "open file_c failed for %d\n", errno);       

    d4d(0, "fd_a=%d, fd_b=%d, fd_c=%d\n", fd_a, fd_b, fd_c);

    //
    // business logic run here
    //
    d4d(0, "business logic runs\n");

    // done, cleanup   
    if(fd_a>-1) close(fd_a);
    if(fd_b>-1) close(fd_b);
    if(fd_c>-1) close(fd_c);
}
int main( int argc, char **argv )
{ 
    int ret=0;
    unsigned int options=OPT_RUNAS_DAEMON|OPT_CLOSE_STDERR;
    
    log_init();

    ret=parse_cmdline(argc, argv, &options); 
    if(ret<0)
        goto TAG_RET;

    if( (options&OPT_RUNAS_DAEMON)
          && ((ret=daemon_init(options))<0)) 
    {
        d4d(0, "run as daemon failed\n");
        ret=-1; 
    }        

    // conduct business now
    do_business();

TAG_RET:
    d4d(0, "exit with %d, bye\n", ret);

    log_close();
    return ret;	
}
