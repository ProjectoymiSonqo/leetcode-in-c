#include <stdio.h>
#include <stdbool.h>
#include <string.h>
char s[] = "anagram";
char t[] = "nagaram";
int map_s[26]={0};
int map_t[26]={0};
int main(){
    bool result=true;
    if(strlen(s)!=strlen(t))
    {
        result=false;
    }
    for(int i=0;i<strlen(s);i++)
    {
        char c_s=s[i];
        char c_t=t[i];
        map_s[c_s-'a']++;
        map_t[c_t-'a']++;
    }
    for(int j=0;j<26;j++)
    {
        printf("%d,%d\n",map_s[j],map_t[j]);
        // if(map_s[j]!=map_t[j])
        // {
        //     result=false;
        // }
    }
    
    return 0;
}