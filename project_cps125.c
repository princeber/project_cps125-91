#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> // Include for strlen

void swap(double *a, double *b) {			//	Ability to swap numbers for bubble sort, using pointers so no need to pass around
    double num = *a;
    *a = *b;
    *b = num;
}

void bubbleSort(double arr[], int size){	//	Bubble Sorting algorythmn , using pointers so no need to pass around
	for(int j = 0; j < size-1; j++){
		for(int k = 0; k < size-1; k++){
			if(arr[k] > arr[k+1]){
				swap(&arr[k], &arr[k+1]);
			}
		}
	}	
}

double findMedian(double arr[], int start, int end){	//	Median function to be reused for lower and upper and entire array
	int size = end - start + 1;
	int halfSize = size / 2;
	if(size%2 == 0)										//	If the array is an even number, you have to find avg of two numbers clossest to middle
		return (arr[start + halfSize - 1] + arr[start + halfSize]) / 2.0;
	else{												//	Else you can directly take the middle number
		return arr[start + size / 2];
	}
}

double mean(double arr[], int size){	//Average function
	double sum = 0;
	for(int i = 0; i < size - 1; i++){
		sum = sum + arr[i];
	}
	return sum/size;		
}

void findQuartiles(double arr[], int size, double *Q1, double *Q2, double *Q3){		//	Process Median for entire, upper and lower of array, using pointers so no need to pass around
    bubbleSort(arr, size); 									//	Sort array
    int halfSize = size / 2;
    *Q2 = findMedian(arr, 0, size); 						//	Median of the entire array
    if(size%2 == 0){										//	If the entire array is even do the following to calculate upper and lower median
		*Q1 = findMedian(arr, 0, halfSize - 1); 			//	Median of the lower half (0 to halfsize - 1)
		*Q3 = findMedian(arr, halfSize, size - 1);			//	Median of the upper half (halfsize to size - 1)
	}															
    else {													//	Else the entire array is odd do the following to calculate upper and lower median
        *Q1 = findMedian(arr, 0, halfSize - 1);				//	Median of the lower half
        *Q3 = findMedian(arr, halfSize + 1, size - 1);		//	Median of the upper half
    }

}

int sizeOfArray(char* filename){
	FILE* in = fopen(filename, "r"); 		// Open the file
    char input[100];						//	Handles maximum number of chacters for fgets function
    double temp1, temp2;					//	Store temperature for eachline of file, since each line can have maximum two temperature lines
    int id, count = 0;
    
    while(fgets(input, sizeof(input), in) != NULL){				
		//printf("Read line: %s", input);
		int result = sscanf(input, "%d,%lf,%lf", &id, &temp1, &temp2);		//	sscan scans the numbers and saves the into specific format
		
		if (result == 3) {
            count = count + 1;		//	If there are two files in a line
            count = count + 1;
        } 
        else if (result == 2) {
            count = count + 1;		//	If there is only files in a line
        } 
        else {

        }
				
    }
    
    return count; // Return the total number of temperatures
    fclose(in);		
	
}

double* readTempArray(char* filename, int size){
	FILE* in = fopen(filename, "r"); 	// Open the file
    char input[100];					//	Handles maximum number of chacters for fgets function
    double temp1, temp2;				//	Store temperature for eachline of file, since each line can have maximum two temperature lines
    double unsortedTemp[732];			//	Maxiumum possible ammount of temeprature of any given file, since there can be a maximum of 366 days and each day have 2 temmps
    int id, count = 0;
    
    double* array = (double*)malloc(size * sizeof(double));		//	Source of how to pass on array from function back to main for C - https://www.geeksforgeeks.org/return-an-array-in-c/#return-an-array-in-c-using-pointers
    
    while(fgets(input, sizeof(input), in) != NULL){		
		
		int result = sscanf(input, "%d,%lf,%lf", &id, &temp1, &temp2);		//	sscan scans the numbers and saves the into specific format
		
		if (result == 3) {
            //	Two numbers were read
            unsortedTemp[count] = temp1;
            count = count + 1;
            unsortedTemp[count] = temp2;
            count = count + 1;
        } 
        else if (result == 2) {
			//	One numbers were read
            unsortedTemp[count] = temp1;
            count = count + 1;									

        } 
        else {
        }
				
    }
    fclose(in);
    
    
    double oneDtemp[count];				//Storing the unsortedarray with trailing zeros into a custom array that fits the size perfectly
    for (int i = 0; i < count; i++) {
		oneDtemp[i] = unsortedTemp[i];
		array[i] = oneDtemp[i];
        //printf("%.6f\n", oneDtemp[i]);
    }
    return array;	//Returning array back to the function that called it (displayQuartiles)
    
}

void displayQuartiles(char* filename, int size){
	double* temperature = readTempArray(filename, size);		//	Creating array of all temp with right size
	
    double Q1, Q2, Q3;											//	Calculating Median, Mean, etc...
    findQuartiles(temperature, size, &Q1, &Q2, &Q3);	
    printf("\n");												//	Displaying Calculations
    printf("Minimum: %.6f\n", temperature[0]);
    printf("Maximum: %.6f\n", temperature[size - 1]);
    printf("Q1: %.6f\n", Q1);
    printf("Q2 (Median): %.6f\n", Q2);
    printf("Q3: %.6f\n", Q3);
    printf("Mean: %.6f\n", mean(temperature, size));
	
}

int main() {
	printf("Lake Superior Data:\n");							//	Pass to calculations and display function...
	char* filename = "avgtemps-s_1995-2025_C.csv"; 				//	Opening Temperature File
	int size = sizeOfArray(filename);							//	Calculating Size of Temperature File
	displayQuartiles(filename, size);							//	Pass to calculations and display function...
	
	printf("\n\nLake Michigan Data:\n");
	filename = "avgtemps-m_1995-2025_C.csv"; 				
	size = sizeOfArray(filename);							
	displayQuartiles(filename, size);
	
	printf("\n\nLake Huron Data:\n");
	filename = "avgtemps-h_1995-2025_C.csv"; 				
	size = sizeOfArray(filename);							
	displayQuartiles(filename, size);
	
	printf("\n\nLake Ontario Data:\n");
	filename = "avgtemps-o_1995-2025_C.csv"; 				
	size = sizeOfArray(filename);							
	displayQuartiles(filename, size);
	
	printf("\n\nLake Erie Data:\n");
	filename = "avgtemps-e_1995-2025_C.csv"; 				
	size = sizeOfArray(filename);							
	displayQuartiles(filename, size);
	
	printf("\n\nLake St. Clair Data:\n");					
	filename = "avgtemps-c_1995-2025_C.csv"; 				
	size = sizeOfArray(filename);							
	displayQuartiles(filename, size);						
	
	
	
	/*															//	Printing out Elements of array
	printf("Array Elements: ");
    for (int i = 0; i < size; i++) {
        printf("%.6lf ", temperature[i]);
    }
    */  
	
    return 0;
    
}
