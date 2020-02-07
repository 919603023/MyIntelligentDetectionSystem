#include <stdio.h> /* FILE */
#include <fcntl.h>/* O_RDWR */
#include <string.h>/* memset() */
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sqlite3.h"
#define register 0
#define changepassword 1
#define logon 3
int MySqlite3(char *sql,char *tmp,char *tmp2,char *tmp3,int type)
{
    sqlite3 *db = NULL;
	int result, nRow, nColumn;
	char * errmsg = NULL;
	char **dbResult;
	
	result = sqlite3_open("test.db", &db);
	if(result != SQLITE_OK)
	{
		printf("sqlite3 error: open\n");
		
	}
	result = sqlite3_get_table(db, sql,
			&dbResult, &nRow, &nColumn, &errmsg);
	
if(type == register)
{
	if(result == SQLITE_OK)
	{
		int i, j;
		
		
		if(nRow == 0)
		{	
			printf("此用户未注册\n");
			return 0;
		}
		else 
		{
			if(strcmp(dbResult[1],tmp) == 0)
			{
				printf("登录成功\n");
			}
			else
			{
				printf("登陆失败，用户名或密码不正确\n");
			}
		}
		
		sqlite3_free_table(dbResult);
	}
}
else if(type == changepassword)
{
if(result == SQLITE_OK)
	{
		int i, j;	
		if(nRow == 0)
		{	
			printf("此用户未注册\n");
			return 0;
		}
		else 
		{
			if(strcmp(dbResult[1],tmp2) == 0)
			{
				char buf[100] =""; 
		    	sprintf(buf,"update test set pwd='%s' where id='%s'",tmp3,tmp); 
				result = sqlite3_get_table(db, buf,&dbResult, &nRow, &nColumn, &errmsg);
				printf("更改成功\n");
			}
			else
			{
				printf("更改失败，用户名或密码不正确\n");
			}
		}
		
		sqlite3_free_table(dbResult);
	}
}
else if(type == logon)
{
	if(result == SQLITE_OK)
	{
		int i, j;	
		if(nRow == 0)
		{	
			char buf[100] =""; 
		    sprintf(buf,"insert into test values ('%s','%s');",tmp,tmp2); 
			result = sqlite3_get_table(db, buf,&dbResult, &nRow, &nColumn, &errmsg);
			printf("注册成功\n");
		}
		else 
		{
				printf("此用户已被注册\n");
			
		}
		
		sqlite3_free_table(dbResult);
	}
}
	sqlite3_close(db);
 
}
int main(int argc, char *argv[])
{
	printf("content-type:text/html\n\n");
	char *len = getenv("CONTENT_LENGTH");
        // 再使用fgets的实际的数据
        char data[128]="";
        fgets(data,atoi(len)+1,stdin);

	if(strncmp("register:",data,9) == 0)
	{
		// printf("%s\n",data);
		char user[50] = "",passwd[50] = "";
		sscanf(data,"%*[^:]:%[^,],%s",user,passwd);
		char sql[100]="";
		sprintf(sql,"select pwd from test where id=\"%s\";",user);
		// printf("%s\n",sql);
		// printf("%s,%s\n",user,passwd);
		MySqlite3(sql,passwd,NULL,NULL,register);
	}
	else if(strncmp("Changepassword:",data,15) == 0)
	{
		char user[50] = "",oldpasswd[50] = "",newpasswd[50] = "";
		sscanf(data,"%*[^:]:%[^,],%[^,],%s",user,oldpasswd,newpasswd);
		char sql[100]="";
		
		sprintf(sql,"select pwd from test where id=\"%s\";",user);
		printf("%s\n",sql);
		MySqlite3(sql,user,oldpasswd,newpasswd,changepassword);
	}
	else if(strncmp("Logon:",data,6) == 0)
	{
		char user[50] = "",passwd[50] = "";
		sscanf(data,"%*[^:]:%[^,],%s",user,passwd);
		char sql[100]="";
		sprintf(sql,"select pwd from test where id=\"%s\";",user);
		MySqlite3(sql,user,passwd,NULL,logon);
	}
	
return 0;
}
