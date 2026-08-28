int lengthOfLongestSubstring(char* s) {
    int len=strlen(s);
    int count=0;
    int count_max=0;
    int checkpoint=0;
    for(int i=0;i<len;i++)
    {  
        for(int j=checkpoint;j<i;j++)
        {
            if(s[i]==s[j])
            {
                checkpoint=j+1;
                break;
            }
        }
        count=i-checkpoint+1;
        if(count>count_max)
        {
            count_max=count;
        }
    }

    return count_max;

}