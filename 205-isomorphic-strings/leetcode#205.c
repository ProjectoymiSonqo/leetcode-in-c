#include <stdio.h>
#include <string.h>
#include <stdbool.h>
char s[]="paper";
char t[]="title";
int map_s[128]={0};
int map_t[128]={0};
int index_s=1;
int index_t=1;
bool result=true;
int main(){
    int s_len=strlen(s);
    int t_len=strlen(t);
    for(int i=0;i<strlen(s);i++)
    {
        char c = s[i];
        char d = t[i];
//        printf("%d\n",(unsigned char)c);
        if(map_s[(unsigned char)c]==0)
        {
            map_s[(unsigned char)c]=index_s;
            index_s++;
        }

        if(map_t[(unsigned char)d]==0)
        {
            map_t[(unsigned char)d]=index_t;
            index_t++;
        }
        printf("index_s=%d,",map_s[(unsigned char)c]);
        printf("index_t=%d\n",map_t[(unsigned char)d]);        
        if(map_s[(unsigned char)c]!=map_t[(unsigned char)d])
        {
            result=false;
            break;
        }
    }
    printf("result=%d",result);
    
    return 0;
}