
#include "yCFtp.h"#include<stdio.h>

int main()
{
	yCFtp f;
	
	if(f.Connect("192.168.0.11"))
		printf("Connect succ\n");
	else printf("Connect err\n");

	if(f.Login("oracle","oracle"))
		printf("Login succ\n");
	else printf("Login err\n");

	if(f.Get("bb","aa"))
		printf("Get succ\n");
	else printf("Get err\n");


	if(f.Put("te","myputbakgg"))
		printf("Put succ\n");
	else printf("Put err\n");

	if(f.Get("bb2","aa2"))
		printf("Get succ\n");
	else printf("Get err\n");

	if(f.Rename("aa","aa3"))
		printf("Rename succ\n");
	else printf("Rename err\n");

	if(f.Delete("myputbakgg"))
		printf("Delete succ\n");
	else printf("Delete err\n");

	if(f.Mkdir("hhh"))
		printf("Mkdir succ\n");
	else printf("Mkdir err\n");

	if(f.Put("te","hhh/gg"))
		printf("Put succ\n");
	else printf("Put err\n");

	if(f.Rmdir("ff"))
		printf("Rmdir succ\n");
	else printf("Rmdir err\n");

	char pPwd[256];
	if(f.Pwd(pPwd))
	{
		printf("Pwd succ\n");
		printf("pPwd=%s\n",pPwd);
	}
	else printf("Pwd err\n");

	if(f.Cd("hhh"))
	{
		printf("Cd succ\n");
		if(f.Put("te","gg2"))
			printf("Put succ\n");
		else printf("Put err\n");

	}
	else printf("Cd err\n");

	if(f.Pwd(pPwd))
	{
		printf("Pwd succ\n");
		printf("pPwd=%s\n",pPwd);
	}
	else printf("Pwd err\n");

	if(f.Cdup())
		printf("Cdup succ\n");
	else printf("Cdup err\n");

	if(f.Pwd(pPwd))
	{
		printf("Pwd succ\n");
		printf("pPwd=%s\n",pPwd);
	}
	else printf("Pwd err\n");

	int nSize;
	if(f.Size("aa3",&nSize))
	{
		printf("Size succ\n");
		printf("aa3.nSize=%d\n",nSize);
	}
	else printf("Size err\n");

	if(f.Ls("lsls","/"))
	{
		printf("Ls succ\n");
	}
	else printf("Ls err\n");

	if(f.Nlist(NULL,"/"))
	{
		printf("Nlist succ\n");
	}
	else printf("Nlist err\n");

	if(f.Site("chmod 755 aa2"))
	{
		printf("Site succ\n");
	}
	else printf("Site err\n");

	f.Quit();

	return 1;
}
