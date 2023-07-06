#include <stdio.h>

int nums[]={1, 2, 4, 6, 7, 9, 7, 11, 13, 17};
int dble(int num) {
    return num + num;
} 
int main(){
    int volatile sum = 0;
    for(int i=0; i<10; i++)
        sum+=dble(nums[i]);
    printf("Sum: %d", sum);
    return 1;
}

