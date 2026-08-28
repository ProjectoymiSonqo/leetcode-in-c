struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
    struct ListNode *head = NULL;
    struct ListNode *tail = NULL;
    int carry=0;
    int digit=0; 
    int value1=0;
    int value2=0;
    while(l1!=NULL||l2!=NULL||carry!=0)
    {
        value1=0;
        value2=0;
        struct ListNode *newNode =
        malloc(sizeof(struct ListNode));
        if(l1!=NULL)
            value1=l1->val;
        if(l2!=NULL)
            value2=l2->val;
        int sum=value1+value2+carry;
        digit=sum%10;
        carry=sum/10;
        newNode->val=digit;
        newNode->next=NULL;
        if(l1!=NULL)
            l1=l1->next;
        if(l2!=NULL)
            l2=l2->next;
        if(head==NULL)
        {
            head=newNode;
            tail=newNode;
        }
        else
        {            
            tail->next=newNode;
            tail=newNode;
        }
        
    }
    
    return head;
}