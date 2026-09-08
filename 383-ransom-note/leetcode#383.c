#include <stdio.h>
#include <string.h>
#include <stdbool.h>
char ransomNote[] = "aa";
char magazine[] = "aaab";
bool result=true;
int count[26] = {0};
int main(){
    int magazine_len=strlen(magazine);
    int ransomNote_len=strlen(ransomNote);
    for(int i=0;i<ransomNote_len;i++)
    {
        count[ransomNote[i] - 'a']++;
        printf("count[%d]=%d\n",ransomNote[i] - 'a',count[ransomNote[i] - 'a']);
    }
    for(int j=0;j<magazine_len;j++)
    {
        count[magazine[j] - 'a']--;
        printf("count[%d]=%d\n",magazine[j] - 'a',count[magazine[j] - 'a']);
    }
    for(int k=0;k<26;k++)
    {
        if(count[k]>0)
        {
            result=false;
            break;
        }
    }
    printf("result=%d\n",result);





}