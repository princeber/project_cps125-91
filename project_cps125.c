#include<stdio.h>
int main() {
    FILE* in;
    double testArray[366], test;
    printf("Hello World");
    
	in = fopen("avgtemps-s_1995-2025_C.csv", "r");
    while(fscanf (in, "%lf", &test) != EOF){
        printf("%lf", test);
    }
    
    return 0;
}