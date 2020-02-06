#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sqlite3.h"

int main(int argc, char **argv) 
{
	sqlite3 *db = NULL;
	int result, nRow, nColumn;
	char * errmsg = NULL;
	char *sql = NULL;
	char **dbResult;
	
	result = sqlite3_open("test.db", &db);
	if(result != SQLITE_OK)
	{
		printf("open error\n");
		exit(-1);
	}
	//sql = "select * from stu_info;";
	sql = "select pwd from test where id='99603023';";
	result = sqlite3_get_table(db, sql,
			&dbResult, &nRow, &nColumn, &errmsg);
	if(result == SQLITE_OK)
	{
		int i, j, index;
		
		index = nColumn;
		for(i=0;i<nRow;i++)
		{
			printf("its %dst record\n", i+1);
			for(j=0;j<nColumn;j++)
			{
				printf("dbresult[%d]=%s: dbresult[%d]=%s\n",
						j,dbResult[j],index,dbResult[index]);
				index++;
				sleep(1);
			}
		}
		sqlite3_free_table(dbResult);
	}
	sqlite3_close(db);
	return 0;
}


