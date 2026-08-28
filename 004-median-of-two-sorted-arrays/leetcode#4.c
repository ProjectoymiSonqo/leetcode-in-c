#include <stdio.h>
#include <stdbool.h>

int nums1[] = {20,28,30,50,53 ,59 ,89 ,116,119,120,129,169,174,176,187,188,194};
int nums2[] = {50,60,73,84,115,123,151,153,157,193,195};
int nums1Size = sizeof(nums1)/sizeof(nums1[0]);
int nums2Size = sizeof(nums2)/sizeof(nums2[0]);
typedef struct
{
    bool even;
    int left_rank;
    int right_rank;
} TargetRank;

TargetRank init_target_rank(int total_size)
{
    TargetRank target;
    if (total_size % 2 == 0)
    {
        target.even = true;
        target.left_rank = total_size / 2;
        target.right_rank = total_size / 2 + 1;
    }
    else
    {
        target.even = false;
        target.left_rank = total_size / 2 + 1;
        target.right_rank = target.left_rank;
    }
    return target;
}

typedef struct
{
    int left;
    int right;
} Range;


typedef struct
{
    int left_bound;
    int right_bound;
    int count;
    int min_idx;
    int mid_idx;
    int max_idx;
    int min_val;
    int mid_val;
    int max_val;
} MidPoint;

MidPoint get_midpoint(int nums[],
                      int left_bound,
                      int right_bound)
{
    MidPoint mid;
    int size = right_bound - left_bound + 1;
    mid.left_bound=left_bound;
    mid.right_bound=right_bound;    
    if(size==1)
    {
        mid.count=1;
        mid.mid_idx = left_bound;
        mid.mid_val = nums[mid.mid_idx];
    }
    else if(size%2==0)
    {
        mid.count=2;
        mid.min_idx = left_bound + size / 2 - 1;
        mid.max_idx = left_bound + size / 2;
        mid.min_val = nums[mid.min_idx];
        mid.max_val = nums[mid.max_idx];        
    }
    else
    {
        mid.count=3;
        mid.min_idx = left_bound + size / 2 - 1;
        mid.mid_idx = left_bound + size / 2;
        mid.max_idx = left_bound + size / 2 + 1;
        mid.min_val = nums[mid.min_idx];
        mid.mid_val = nums[mid.mid_idx];
        mid.max_val = nums[mid.max_idx];
    }

    return mid;
}

int get_left_value(MidPoint mid)
{

    if (mid.count == 1)

        return mid.mid_val;

    return mid.min_val;

}

int get_right_value(MidPoint mid)
{

    if (mid.count == 1)

        return mid.mid_val;

    return mid.max_val;

}

typedef enum
{
    OVERLAP,
    NUM1_LEFT,
    NUM2_LEFT
} CompareResult;

CompareResult compare_mid(MidPoint num1,
                          MidPoint num2)
{
    int num1_min = get_left_value(num1);
    int num1_max = get_right_value(num1);
    int num2_min = get_left_value(num2);
    int num2_max = get_right_value(num2);
    if (num1_max < num2_min)
        return NUM1_LEFT;

    if (num2_max < num1_min)
        return NUM2_LEFT;
    return OVERLAP;
}

int count_less_equal(int nums[], int size, int value)

{

    int count = 0;

    for (int i = 0; i < size; i++)

    {

        if (nums[i] <= value)

            count++;

        else

            break;

    }

    return count;

}

int get_rank(int value)
{
    int count1 = count_less_equal(nums1, nums1Size, value);
    int count2 = count_less_equal(nums2, nums2Size, value);

    return count1 + count2;
}

int main() {
    printf("Target setting:\n");
    int total_size = nums1Size + nums2Size;
    TargetRank target = init_target_rank(total_size);
    printf("Left Rank : %d\n", target.left_rank);
    printf("Right Rank: %d\n", target.right_rank);
    printf("Even      : %d\n", target.even);
    MidPoint num1_mid =
    get_midpoint(nums1, 0, nums1Size - 1);
    MidPoint num2_mid =
    get_midpoint(nums2, 0, nums2Size - 1);
    CompareResult result = compare_mid(num1_mid, num2_mid);
    switch(result)
    {
    case OVERLAP:
        printf("OVERLAP\n");
        
        break;
    case NUM1_LEFT:
        printf("NUM1_LEFT\n");
        break;
    case NUM2_LEFT:
        printf("NUM2_LEFT\n");
        break;
    }
    printf("119 rank = %d\n", get_rank(119));
printf("120 rank = %d\n", get_rank(120));
printf("123 rank = %d\n", get_rank(123));
    return 0;
}
