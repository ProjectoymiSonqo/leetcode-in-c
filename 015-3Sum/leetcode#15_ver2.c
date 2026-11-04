#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

struct Node{
    int val;
    int cnt;
    struct Node *next;
}; 
static int hash_index(int value,int bucket){
    int index=value%bucket;
    if(index<0)
        index=index+bucket;
    return index;
}
static struct Node *hascontain(struct Node **table, int bucket, int value)
{
    int index=hash_index(value,bucket);
    struct Node *current=table[index];
    while(current!=NULL)
    {
        if(current->val==value)
            return current;
        current=current->next;
    }
    return NULL;
}
int** threeSum(int* nums, int numsSize,
               int* returnSize,
               int** returnColumnSizes)
{
    *returnSize = 0;
    *returnColumnSizes = NULL;

    if (numsSize < 3)
        return NULL;
    int bucket=2*numsSize+1;
    struct Node **table=calloc(bucket,sizeof(struct Node *));
    struct Node *pool=malloc(numsSize*sizeof(struct Node));
    if(table==NULL||pool==NULL)
    {
        free(table);
        free(pool);
        return 0;
    }
    int used=0;

    for(int i=0;i<numsSize;i++)
    {
        struct Node *found=hascontain(table,bucket,nums[i]);
        if(found!=NULL)     //找到已存在的數 更改cnt+1
        {
            found->cnt++;
            continue;
        }
        int index=hash_index(nums[i],bucket);   //沒有已存在的數 計算要放哪
        struct Node *new_node=&pool[used++];    //pool已經分配位置 而new_node放在分配的位置+1
        new_node->val=nums[i];
        new_node->cnt=1;
        new_node->next=table[index];
        table[index]=new_node;   
    }

    int count=0;
    int capacity=1;
    int **result=malloc(capacity*sizeof(int *));
    if (result == NULL)
    {
        free(table);
        free(pool);
        return NULL;
    }
    for(int i=0;i<used;i++)
    {
        int ans=pool[i].val;
        for(int j=i;j<used;j++)
        {

            int know1=pool[j].val;
            int legal=false;
            long long target=-(long long)ans-know1;
            if(target<INT_MIN||target>INT_MAX)
                continue;
            struct Node *found=hascontain(table,bucket,(int)target);
            if(found!=NULL)
            {
                int k=found-pool;
                if(k<j)
                    continue;
                if(i==j&&j==k)
                {
                    if(pool[j].cnt>=3)
                    {
                        legal=true;
                    }
                }
                else if(i==j||j==k)
                {
                    if(pool[j].cnt>=2)
                    {
                        legal=true;
                    }
                }
                else
                {
                    legal=true;
                }
                if(legal==true&&count<capacity)
                {
                    result[count]=malloc(3*sizeof(int));
                    if (result[count] == NULL)
                    {
                        for (int a = 0; a < count; a++)
                        {
                            free(result[a]);
                        }
                        free(result);
                        free(table);
                        free(pool);
                        *returnSize = 0;
                        *returnColumnSizes = NULL;
                        return NULL;
                    }
                    result[count][0]=ans;
                    result[count][1]=know1;
                    result[count][2]=found->val;
                    count++;
                }
                else if(legal==true&&count==capacity)
                {
                    int new_capacity = capacity * 2;
                    int **temp=realloc(result,new_capacity*sizeof(int *));
                    if(temp==NULL)
                    {
                        for (int i = 0; i < count; i++)
                        {
                           free(result[i]);
                        }
                        free(result);
                        free(table);
                        free(pool);
                        *returnSize = 0;
                        *returnColumnSizes = NULL;
                        return NULL;
                    }
                    else
                    {
                        result=temp;
                        capacity=new_capacity;
                        result[count]=malloc(3*sizeof(int));
                        if (result[count] == NULL)
                        {
                            for (int a = 0; a < count; a++)
                            {
                                free(result[a]);
                            }
                            free(result);
                            free(table);
                            free(pool);
                            *returnSize = 0;
                            *returnColumnSizes = NULL;
                            return NULL;
                        }


                        result[count][0]=ans;
                        result[count][1]=know1;
                        result[count][2]=found->val;
                        count++;
                    }   
                }
            }
            
        }
    }
    if (count == 0)
    {
        free(result);
        free(pool);
        free(table);
        return NULL;
    }
    *returnSize=count;
    *returnColumnSizes = malloc(count * sizeof(**returnColumnSizes));
    if (*returnColumnSizes == NULL)
    {
        for (int k = 0; k < count; k++)
        {
            free(result[k]);
        }

        free(result);
        free(table);
        free(pool);

        *returnSize = 0;
        return NULL;
    }
    for(int i=0;i<count;i++)
    {
        (*returnColumnSizes)[i]=3;
    }
    
    free(pool);
    free(table);
    return result;

}

/* ===== Test Environment ===== */

typedef struct {
    int values[3];
} Triplet;

static int compareInt(const void *a, const void *b)
{
    int x = *(const int*)a;
    int y = *(const int*)b;
    return (x > y) - (x < y);
}

static int compareTriplet(const void *a, const void *b)
{
    const Triplet *x = a;
    const Triplet *y = b;

    for (int i = 0; i < 3; i++) {
        if (x->values[i] != y->values[i])
            return (x->values[i] > y->values[i]) -
                   (x->values[i] < y->values[i]);
    }
    return 0;
}

static void runTest(int id, const int *input, int n,
                    const Triplet *expected, int expectedCount)
{
    int *nums = malloc((size_t)(n > 0 ? n : 1) * sizeof(int));

    if (!nums) {
        printf("Allocation failed\n");
        return;
    }

    if (n > 0)
        memcpy(nums, input, (size_t)n * sizeof(int));

    int returnSize = 0;
    int *columnSizes = NULL;

    int **result = threeSum(nums, n, &returnSize, &columnSizes);

    bool pass = (returnSize == expectedCount);
    Triplet *actual = NULL;

    if (returnSize < 0 || returnSize > 1000000) {
        printf("Test %2d | Invalid returnSize\n", id);
        free(nums);
        return;
    }

    if (returnSize > 0) {
        if (!result || !columnSizes) {
            pass = false;
        } else {
            actual = calloc((size_t)returnSize, sizeof(Triplet));
            if (!actual) {
                printf("Test %2d | Allocation failed\n", id);
                free(nums);
                return;
            }

            for (int i = 0; i < returnSize; i++) {
                if (!result[i] || columnSizes[i] != 3) {
                    pass = false;
                    continue;
                }

                for (int j = 0; j < 3; j++)
                    actual[i].values[j] = result[i][j];

                qsort(actual[i].values, 3, sizeof(int), compareInt);
            }

            if (pass) {
                qsort(actual, returnSize,
                      sizeof(Triplet), compareTriplet);

                Triplet *sortedExpected =
                    malloc((size_t)expectedCount * sizeof(Triplet));

                if (!sortedExpected) {
                    pass = false;
                } else {
                    memcpy(sortedExpected, expected,
                           (size_t)expectedCount * sizeof(Triplet));

                    for (int i = 0; i < expectedCount; i++)
                        qsort(sortedExpected[i].values, 3,
                              sizeof(int), compareInt);

                    qsort(sortedExpected, expectedCount,
                          sizeof(Triplet), compareTriplet);

                    for (int i = 0; i < expectedCount; i++) {
                        if (compareTriplet(&actual[i],
                                           &sortedExpected[i]) != 0) {
                            pass = false;
                            break;
                        }
                    }
                    free(sortedExpected);
                }
            }
        }
    }

    printf("Test %2d | Expected: %2d | Result: %2d | %s\n",
           id, expectedCount, returnSize,
           pass ? "PASS" : "FAIL");

    if (!pass && actual) {
        printf("  Actual: ");
        for (int i = 0; i < returnSize; i++)
            printf("[%d,%d,%d] ",
                   actual[i].values[0],
                   actual[i].values[1],
                   actual[i].values[2]);
        printf("\n");
    }

    for (int i = 0; result && i < returnSize; i++)
        free(result[i]);

    free(result);
    free(columnSizes);
    free(actual);
    free(nums);
}

#define TEST(ID, INPUT, EXPECTED) \
    runTest(ID, INPUT, sizeof(INPUT)/sizeof((INPUT)[0]), \
            EXPECTED, sizeof(EXPECTED)/sizeof((EXPECTED)[0]))

int main(void)
{
    int a1[] = {-1,0,1,2,-1,-4};
    Triplet e1[] = {{{-1,-1,2}},{{-1,0,1}}};
    TEST(1,a1,e1);

    int a2[] = {0,1,1};
    runTest(2,a2,3,NULL,0);

    int a3[] = {0,0,0};
    Triplet e3[] = {{{0,0,0}}};
    TEST(3,a3,e3);

    int a4[] = {0,0,0,0,0};
    Triplet e4[] = {{{0,0,0}}};
    TEST(4,a4,e4);

    int a5[] = {-2,0,1,1,2};
    Triplet e5[] = {{{-2,0,2}},{{-2,1,1}}};
    TEST(5,a5,e5);

    int a6[] = {-4,-2,-2,-2,0,1,2,2,2};
    Triplet e6[] = {
        {{-4,2,2}},{{-2,0,2}}
    };
    TEST(6,a6,e6);

    int a7[] = {1,2,-2,-1};
    runTest(7,a7,4,NULL,0);

    int a8[] = {-1,0,1};
    Triplet e8[] = {{{-1,0,1}}};
    TEST(8,a8,e8);

    int a9[] = {1,1,1,1};
    runTest(9,a9,4,NULL,0);

    int a10[] = {-1,-1,0,0,1,1};
    Triplet e10[] = {{{-1,0,1}}};
    TEST(10,a10,e10);

    int a11[] = {-3,-2,-1,0,1,2,3};
    Triplet e11[] = {
        {{-3,0,3}},{{-3,1,2}},
        {{-2,-1,3}},{{-2,0,2}},
        {{-1,0,1}}
    };
    TEST(11,a11,e11);

    int a12[] = {INT_MIN,INT_MAX,1,0,-1};
    Triplet e12[] = {
        {{INT_MIN,1,INT_MAX}},
        {{-1,0,1}}
    };
    TEST(12,a12,e12);

    int a13[] = {5,5,5,5,5};
    runTest(13,a13,5,NULL,0);

    int a14[] = {-5,-3,-2,0,2,3,5};
    Triplet e14[] = {
        {{-5,0,5}},{{-5,2,3}},
        {{-3,-2,5}},{{-3,0,3}},
        {{-2,0,2}}
    };
    TEST(14,a14,e14);

    int a15[] = {0,0};
    runTest(15,a15,2,NULL,0);

    runTest(16,NULL,0,NULL,0);

    return 0;
}
