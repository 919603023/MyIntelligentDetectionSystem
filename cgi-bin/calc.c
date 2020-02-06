#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    //用printf输出
    printf("content-type:text/html\n\n");

    //获取服务器收到的数据 如果是POST方式 
        //先用getenv得到数据的长度  再使用fgets的实际的数据
    //getenv的参数为："CONTENT_LENGTH"

    // 用getenv得到数据的长度
    char *len = getenv("CONTENT_LENGTH");
    // 再使用fgets的实际的数据
    char data[128]="";
    fgets(data,atoi(len)+1,stdin);
   

    int data1 = 0, data2=0;
    char ch = 0;
    sscanf(data,"%d%c%d",&data1, &ch, &data2);
    
    (ch=='+')?(printf("%d\n",data1+data2)):(printf("%d\n",data1-data2));
    
    return 0;
}
