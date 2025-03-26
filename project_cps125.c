#include <stdio.h>
#include <stdlib.h>

#define MAX_DAYS 356
#define MAX_VALUES 31  // Now we only store 31 values (excluding the day index)


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
	printf("%d\n", size);
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
    /*
    for (int j = 0; j < MAX_VALUES; j++){
		printf("%.6f ", arr[j]);
	}
	*/
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

void detectOutliers(double arr[], int size, double *Q1, double *Q3, double *IQR) {
	*IQR = *Q3 - *Q1;		//	Compute IQR
	
	//	Calculating Threshold for lower and upper bound
    double lowerBound = *Q1 - 1.5 * *IQR;
    double upperBound = *Q3 + 1.5 * *IQR;
    
    //printf("%.6f ", *IQR);
    //printf("%.6f ", lowerBound);
    //printf("%.6f ", upperBound);
	
	
    printf("Outliers: ");
    int foundOutlier = 0;	//	Keep track of Outliers, reseting for each lake

	//	If the value in array is not inbetween lower and upper bound, you add one to outlier
    for (int i = 0; i < size; i++) {
        if (arr[i] < lowerBound || arr[i] > upperBound) {
			if(arr[i] < 40){
				printf("%.6f ", arr[i]);
				foundOutlier = foundOutlier + 1;
			}
        }
    }

    if (foundOutlier == 0) {
        printf("None");
    }
    printf("\n");
}

void displayQuartiles(double* arr){
	
    double Q1, Q2, Q3, IQR = 0;									//	Calculating Median, Mean, etc...
    findQuartiles(arr, MAX_VALUES, &Q1, &Q2, &Q3);	
    printf("\n");												//	Displaying Calculations
    printf("Minimum: %.6f\n", arr[0]);
    if(arr[MAX_VALUES - 1] > 25){
		printf("Maximum: %.6f\n", arr[MAX_VALUES - 2]);
	}
	else{
		printf("Maximum: %.6f\n", arr[MAX_VALUES - 1]);
	}
    printf("Q1: %.6f\n", Q1);
    printf("Q2 (Median): %.6f\n", Q2);
    printf("Q3: %.6f\n", Q3);
    printf("Mean: %.6f\n", mean(arr, MAX_VALUES));
    detectOutliers(arr, MAX_VALUES, &Q1, &Q3, &IQR);
	
}


void readTempArray(char* filename) {
    FILE *file = fopen(filename, "r");

    double data[MAX_DAYS][MAX_VALUES] = {0};
    double summerDayArray[MAX_DAYS][MAX_VALUES];
    int day_count = 0, totalTempCount = 0;

    while (day_count < MAX_DAYS) {
        double dummy;
        // Read and ignore the first number (day index)
        if (fscanf(file, "%lf,", &dummy) != 1) {
            break;  // Stop if no more data
        }

        // Read the next 31 values (actual temperature data)
        for (int i = 0; i < MAX_VALUES; i++) {
            if (fscanf(file, "%lf,", &data[day_count][i]) != 1) {
                break;  // Stop if incomplete line
            }
        }

        day_count++;
    }

    fclose(file);
    
	double* totalTemp = (double*)malloc(10680 * sizeof(double));
	
	// Process each day's data (now correctly excluding the day index)
    for (int i = 0; i < day_count; i++) {
        printf("\nDay %d's data (31 temperature values):\n", i+1);
		double* eachDayArray = (double*)malloc(MAX_VALUES * sizeof(double));		//	Source of how to pass on array from function back to main for C - https://www.geeksforgeeks.org/return-an-array-in-c/#return-an-array-in-c-using-pointers
		
		
		for (int j = 0; j < MAX_VALUES; j++){
			eachDayArray[j] = data[i][j];
			//printf("%.6f ", eachDayArray[j]);
			if(i >= 172 && i <= 266){
				summerDayArray[i][j] = data[i][j];
				
				//printf("Summer Stuff: %.6f ", summerDayArray[i][j]);
				
			}
			
			totalTemp[totalTempCount] = data[i][j];
			totalTempCount = totalTempCount + 1;
		}
		displayQuartiles(eachDayArray);
		free(eachDayArray);
    }
    
    printf("\n");
    printf("Calculations of Warmest and Coldest Temperature of Lake Over 30 years\n");
    bubbleSort(totalTemp, 10680);
    for (int i = 0; i < (10680-2); i++) {
		//printf("%.6f ", totalTemp[i]);
    }
    printf("\n");

    for (int i = (10680-1); i > 0; i--) {
		//printf("%.6f ", totalTemp[i]);
		if (totalTemp[i] < 30){
			printf("Hottest Temperature: %.6f\n", totalTemp[i]);;
			break;
			
		}
    }
    for (int i = 0; i < 10680; i++) {
		if (totalTemp[i] > 0){
			printf("Coldest Temperature: %.2f\n", totalTemp[i]);
			break;
			
		}
    }
    
    //Summer Calc
    
    //Summer Avg per year per lake
    printf("\n");
    double summerTotalSum = 0;
    double summerSum = 0;
    double avg = 0;
    for (int j = 0; j < 31; j++){
		printf("Summer Average of Year %d\n", 1995 + j);
		for(int i = 172; i < 266; i++){
			summerSum = summerSum + summerDayArray[i][j];
		}
		avg = summerSum / (266-172);
		summerSum = 0;	
		
		printf("%.2lf\n", avg);
		summerTotalSum = summerTotalSum + avg;
			 
	}
	
	//Summer Avg Total
	printf("\n");
	printf("Summer Average of Lake Over 30 Years...\n");
	printf("%lf", summerTotalSum / 30);
	summerTotalSum = 0;
	
}


int main() {
	printf("Lake Superior Data:\n");							//	Pass to calculations and display function...
	char* filename = "all_year_glsea_avg_s_C.csv"; 				//	Opening Temperature File
	readTempArray(filename);
	/*
	printf("\n\nLake Michigan Data:\n");
	filename = "all_year_glsea_avg_m_C.csv"; 				
	readTempArray(filename);
	
	printf("\n\nLake Huron Data:\n");
	filename = "all_year_glsea_avg_h_C.csv"; 				
	readTempArray(filename);
	
	printf("\n\nLake Ontario Data:\n");
	filename = "all_year_glsea_avg_o_C.csv"; 				
	readTempArray(filename);
	
	printf("\n\nLake Erie Data:\n");
	filename = "all_year_glsea_avg_e_C.csv"; 				
	readTempArray(filename);
	
	printf("\n\nLake St. Clair Data:\n");					
	filename = "all_year_glsea_avg_c_C.csv"; 				
	readTempArray(filename);	
	*/
	
    return 0;
}
