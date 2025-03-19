#include<stdio.h>
#include <math.h>
#include <string.h> // Include for strlen

void swap(double *a, double *b) {			//	Ability to swap numbers for bubble sort
    double num = *a;
    *a = *b;
    *b = num;
}

void bubbleSort(double arr[], int size){	//	Bubble Sorting algorythmn 
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
	if(size%2 == 0)					//	If the array is an even number, you have to find avg of two numbers clossest to middle
		return (arr[start + halfSize - 1] + arr[start + halfSize]) / 2.0;
	else{								//	Else you can directly take the middle number
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

void findQuartiles(double arr[], int size, double *Q1, double *Q2, double *Q3){		//	Process Median for 
    bubbleSort(arr, size); 					//	Sort array
    int halfSize = size / 2;
    *Q2 = findMedian(arr, 0, size); 			//	Median of the entire array
    if(size%2 == 0){
		*Q1 = findMedian(arr, 0, halfSize - 1); 		//	Median of the lower half (0 to halfsize - 1)
		*Q3 = findMedian(arr, halfSize, size - 1);		//	Median of the upper half (halfsize to size - 1)
	}
    else {											//	Odd nubmer of days		Example... (0, 1, 2, 3, 4)	ignore the number 2...
        *Q1 = findMedian(arr, 0, halfSize - 1);		//	Median of the lower half
        *Q3 = findMedian(arr, halfSize + 1, size - 1);		//	Median of the upper half
    }

}
 

int main() {
    FILE* in;
    char input[100];						//	Handles input of file
    double temp[366][2], temp1, temp2;		//	Storing Day and Temperature pre-handdling of leap year
    double unsortedTemp[732];
    int id, count = 0;
    
	in = fopen("avgtemps-s_1995-2025_C.csv", "r");		//	Opening Temperature File
    while(fgets(input, sizeof(input), in) != NULL){		
		//	Reading using fgets(*variable, *length. *means of input), loop until end of file
		//printf("%s\n", input);							//	Printing out what the C program is reading (1 line)						
		
		//input[strlen(input)-1] = '\0';					//	\n kept at end if input smaller than array size, replace it with \0
		//printf("Length: %zu\n", strlen(input));			//	print out length of input "%zu" special format for length in C
		
		int result = sscanf(input, "%d,%lf,%lf", &id, &temp1, &temp2);		//	sscan scans the numbers and saves the into specific format
		//printf("%d\n", result);												//	result represents how many numbers were detected
		
		if (result == 3) {
            // Two numbers were read
            //printf("ID: %d, Number 1: %.6f, Number 2: %.6f\n", id, temp1, temp2);		//	Displaying the day and the two temeprature	
            temp[id-1][0] = temp1;
            unsortedTemp[count] = temp1;
            count = count + 1;
            temp[id-1][1] = temp2;
            unsortedTemp[count] = temp2;
            count = count + 1;
        } 
        else if (result == 2) {
            // Only one number was read
            //printf("ID: %d, Number 1: %.6f\n", id, temp1);	//	Displaying the day and only one temeprature	(cause the spreadsheet somtimes has one measurement per day instead of the usual)
            temp[id-1][0] = temp1;
            unsortedTemp[count] = temp1;
            count = count + 1;
            temp[id-1][1] = 0; 									// Default value for missing temperature

        } 
        else {
            // No numbers were read (invalid line)
            //printf("Skipping invalid line: %s\n", input);
        }
				
    }
    fclose(in);
    
    // Print the stored temperatures (for verification)
    for (int i = 0; i < 366; i++) {
        //printf("Day %d: Temp1 = %.6f, Temp2 = %.6f\n", i + 1, temp[i][0], temp[i][1]);
    }
    
    //printf("Unsorted Temp: \n");		//Printing out all temperatures from the unsorted(trailing 0s) 1d array (for verificiation)
    //printf("%d", count);
    double oneDtemp[count];				//Storing the unsortedarray with trailing zeros into a custom array that fits the size perfectly
    for (int i = 0; i < count; i++) {
		//printf("%d\n", i);
		oneDtemp[i] = unsortedTemp[i];
        //printf("%.6f\n", oneDtemp[i]);
        //printf("%.6f\n", unsortedTemp[i]);
    }
    
    /*									//	Testing if bubble sorting works
    bubbleSort(oneDtemp, count);
    printf("\n");
    for (int i = 0; i < count; i++) {
        printf("%.6f\n", oneDtemp[i]);
    }
    */
    
    printf("\n");
    double Q1, Q2, Q3;
	findQuartiles(oneDtemp, count, &Q1, &Q2, &Q3);
    printf("Minimum: %.6f\n", oneDtemp[0]);
    printf("Maximum: %.6f\n", oneDtemp[count - 1]);
    printf("Q1: %.6f\n", Q1);
    printf("Q2 (Median): %.6f\n", Q2);
    printf("Q3: %.6f\n", Q3);
    printf("Mean: %.6f\n", mean(oneDtemp, count));
	
    return 0;
}
