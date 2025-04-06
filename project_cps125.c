#include <stdio.h>
#include <stdlib.h>

#define MAX_DAYS 365
#define MAX_VALUES 30  // Now we only store 31 values (excluding the day index)

#include <math.h>
#include <string.h> // Include for strlen

//	Source of how to pass on array from function back to main for C - https://www.geeksforgeeks.org/return-an-array-in-c/#return-an-array-in-c-using-pointers

// Converts a day number (1–365) into a month/day format
void dayToDate(int dayNumber) {
    int num = dayNumber;
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    int month = 0;
    while (dayNumber > daysInMonth[month]) {
        dayNumber -= daysInMonth[month];
        month++;
    }

    char nameMonths[12][4] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    printf("Day %d out of 365 is: %s/%d\n", num, nameMonths[month], dayNumber);
}

// Swaps two doubles (used in sorting)
void swap(double *a, double *b) {
    double num = *a;
    *a = *b;
    *b = num;
}

// Bubble sort for sorting temperature data
void bubbleSort(double arr[], int size){
    for(int j = 0; j < size-1; j++){
        for(int k = 0; k < size-1; k++){
            if(arr[k] > arr[k+1]){
                swap(&arr[k], &arr[k+1]);
            }
        }
    }
}

// Calculates median of a portion of an array
double findMedian(double arr[], int start, int end){
    int mid = start + (end - start) / 2;	//Calculating "midpoint" of the array

    if (mid + 1 <= end) {						// If the function does not have a 
        return (arr[mid] + arr[mid + 1]) / 2.0;	//Mediam calculation that handles odd arrays
    } 
    else {
        return arr[mid];
    }
}

// Calculates mean of an array
double mean(double arr[], int size){	
	double sum = 0;
	for(int i = 0; i < size; i++){
		sum = sum + arr[i];
	}
	return sum/size;		
}

// Calculates Q1, Q2, and Q3 (quartiles) of the array
void findQuartiles(double arr[], int size, double *Q1, double *Q2, double *Q3){		 
    bubbleSort(arr, size); 					//	Sort array
    
    /*
    DEBUG
    for (int j = 0; j < MAX_VALUES; j++){
		printf("%.6f ", arr[j]);
	}
	*/
	
    *Q2 = findMedian(arr, 0, size - 1); 			//	Median of the entire array
    if(size%2 == 0){
		*Q1 = findMedian(arr, 0, size/2 - 1); 		//	Median of the lower half (0 to halfsize - 1)
		*Q3 = findMedian(arr, size/2, size - 1);	//	Median of the upper half (halfsize to size - 1)
	}
    else {											//	Odd nubmer of days		Example... (0, 1, 2, 3, 4)	ignore the number 2...
        *Q1 = findMedian(arr, 0, size/2 - 1);		//	Median of the lower half
        *Q3 = findMedian(arr, size/2 + 1, size - 1);//	Median of the upper half
    }
}

// Detects outliers based on IQR method and stores them
void detectOutliers(double arr[], int day, double** outlierArray, int size, double *Q1, double *Q3, double *IQR) {
	*IQR = *Q3 - *Q1;		//	Compute IQR
	
	//	Calculating Threshold for lower and upper bound
    double lowerBound = *Q1 - 1.5 * *IQR;
    double upperBound = *Q3 + 1.5 * *IQR;
    
    //DEBUG 	
    //printf("%.6f ", *IQR);
    //printf("%.6f ", lowerBound);
    //printf("%.6f ", upperBound);
    //printf("Outliers: ");
    
    int foundOutlier = 0;	//	Keep track of Outliers, reseting for each lake

	//	If the value in array is not inbetween lower and upper bound, you add one to outlier
    for (int i = 0; i < size; i++) {
        if (arr[i] < lowerBound || arr[i] > upperBound) {
			if(arr[i] < 40){
				//printf("%.6f ", arr[i]);	//DEBUG
				outlierArray[day][foundOutlier] = arr[i];
				foundOutlier = foundOutlier + 1;
			}
        }    
    }

	//DEBUG
    if (foundOutlier == 0) {
        //printf("None");
    }
    //printf("\n");
}

// Calculates and stores quartiles, min, max, mean, and outliers for a day's data
void displayQuartiles(double* arr, double* q1Array, double* q2Array, double* q3Array, double* q4Array, double* lowArray, double* highArray, double** outlierArray, int day){
	
    double Q1 = 0, Q2 = 0, Q3 = 0, IQR = 0;						//	Calculating Median, Mean, etc...
    findQuartiles(arr, MAX_VALUES, &Q1, &Q2, &Q3);	
    //printf("\n");												//	Displaying Calculations
    
    //printf("Minimum: %.6f\n", arr[0]);
    lowArray[day] = arr[0];
    
    if(arr[MAX_VALUES - 1] > 25){
		//printf("Maximum: %.6f\n", arr[MAX_VALUES - 2]);
		highArray[day] = arr[MAX_VALUES - 2];
	}
	else{
		//printf("Maximum: %.6f\n", arr[MAX_VALUES - 1]);
		highArray[day] = arr[MAX_VALUES - 1];
	}
	
	//Storing each quartile value into tbeir respective array
    //printf("Q1: %.6f\n", Q1);
    q1Array[day] = Q1;
    
    //printf("Q2 (Median): %.6f\n", Q2);
    
    //printf("Calculated Q2 for day %d: %.6f\n", day, Q2);
    q2Array[day] = Q2;
    
    //printf("Q3: %.6f\n", Q3);
    q3Array[day] = Q3;
    
    //printf("Mean: %.6f\n", mean(arr, MAX_VALUES));
    q4Array[day] = mean(arr, MAX_VALUES);
    
    detectOutliers(arr, day, outlierArray, MAX_VALUES, &Q1, &Q3, &IQR);	//Detect any outliers by using quartiles as reference
}

// Reads temperature data from a CSV, processes and analyzes it
double readTempArray(char* filename) {
    FILE *file = fopen(filename, "r");

    double data[MAX_DAYS][MAX_VALUES] = {0};
    double summerDayArray[MAX_DAYS][MAX_VALUES];
    int day_count = 0, totalTempCount = 0;

	//Reading data and storing it in array data
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
        //printf("%d", day_count);
    }

    fclose(file);
    
    //Setting up arrays for each day   
	double* totalTemp = (double*)malloc(MAX_DAYS*MAX_VALUES  * sizeof(double));	
	
	//Index represent days, value represent respective quartile data
	double* q1Array = (double*)malloc(MAX_DAYS  * sizeof(double));
	double* q2Array = (double*)malloc(MAX_DAYS  * sizeof(double));
	double* q3Array = (double*)malloc(MAX_DAYS  * sizeof(double));
	double* q4Array = (double*)malloc(MAX_DAYS  * sizeof(double));
	double* lowArray = (double*)malloc(MAX_DAYS  * sizeof(double));
	double* highArray = (double*)malloc(MAX_DAYS  * sizeof(double));
	
	//Rows represent days cols represent number of outliers of that day
	int rows = MAX_DAYS, cols = 10;
	// Allocate memory for row pointers
    double** outlierArray = (double **)malloc(rows * sizeof(double *));
    
    // Allocate memory for each row
    for (int i = 0; i < rows; i++) {
        outlierArray[i] = (double *)malloc(cols * sizeof(double));
    }
	
	
	// Process each day's data (now correctly excluding the day index)
    for (int i = 0; i < day_count; i++) {
        //printf("\nDay %d's data (31 temperature values):\n", i+1); DEBUG
		double* eachDayArray = (double*)malloc(MAX_VALUES * sizeof(double));		
		
		//Converting 2D array into 1D array of each day for easier handelling and passing of data..
		for (int j = 0; j < MAX_VALUES; j++){
			eachDayArray[j] = data[i][j];
			//printf("%.6f ", eachDayArray[j]); DEBUG
			if(i >= 172 && i <= 266){	//Storing values of summer temperatures for later calculations
				summerDayArray[i][j] = data[i][j];
				//printf("Summer Stuff: %.6f ", summerDayArray[i][j]); DEBUG			
			}
			
			//Saving all values into on array
			totalTemp[totalTempCount] = data[i][j];
			totalTempCount = totalTempCount + 1;
		}
		displayQuartiles(eachDayArray, q1Array, q2Array, q3Array, q4Array, lowArray, highArray, outlierArray, i);
		
		/* DEBUGGING CANT FIX!!!	
		for (int j = 0; j <= i; j++) { // Print only up to processed days
			printf("q2Array[%d]: %.6f", j, q2Array[j]);
		}
		*/
		 
		free(eachDayArray);
    }
    
    // Replace unreasonable quartile values
    /*
    for (int i = 0; i < day_count; i++) {
		//printf("Stored Q2[%d]: %.6f\n", i, q2Array[i]);
		if(q1Array[i] > 100){
			q1Array[i] = q1Array[i-1];
		}
		if(q2Array[i] > 100){
			q2Array[i] = q2Array[i-1];
		}
		if(q3Array[i] > 100){
			q3Array[i] = q3Array[i-1];
		}
		if(q4Array[i] > 100){
			q4Array[i] = q4Array[i-1];
		}	
	}
	*/ 
    
    // Output QUARTILE data per day
    printf("\nDisplay Data:\n\n");
    printf("Day: Minimum:      Maximum:      Q1(Medium Lower Bound):      Q2(Medium):      Q3(Medium Upper Bound):      Q4(Mean):      Outliers:\n");
    for (int i = 0; i < MAX_DAYS; i++) {
		printf("%3d %6.3lf        %6.3lf        %6.3lf                       %6.3lf           %6.3lf                       %6.3lf      ", i+1, lowArray[i], highArray[i], q1Array[i], q2Array[i], q3Array[i], q4Array[i]);
		
		//Handle printing of outliers, exluding unreasonable outliers
		for(int j = 0; j <= 10; j++){
			if(outlierArray[i][j] < 0.0001 /*|| outlierArray[i][j] > 40*/){
				break;
			}
			else{
				if(j > 0 && j%3 == 0){
					printf("\n                                                                                                                       ");	
				}
				printf("   %6.3lf ", outlierArray[i][j]);	
				//printf("%d", j);	DEBUG
				//printf("%d", j%3); DEBUG
			}
		}
		printf("\n");			
	}
 
    printf("\n");
    printf("\n");
    printf("Calculations of Warmest and Coldest Temperature of Lake Over 30 years:\n");
    bubbleSort(totalTemp, 10680);
    
    /*
    DEBUG
    for (int i = 0; i < (10680-2); i++) {
		//printf("%.6f ", totalTemp[i]); 
    }
    */ 
    
    double avgTemp[365];
    
    for (int j = 0; j < MAX_DAYS; j++){
		double sumTemp = 0;
		for(int i = 0; i < MAX_VALUES; i++){
			sumTemp = data[j][i] + sumTemp;	
		}
		avgTemp[j] = sumTemp/MAX_VALUES;
	}
	
	//for (int j = 0; j < MAX_DAYS; j++){
		//printf("%f\n", avgTemp[j]);
	//}
    
    
    //Process of finding which day contains hottest temperature
    printf("\n");
    int NOIday = 0;
    double NOItemp = 0;
    for (int j = 0; j < MAX_DAYS; j++){
		if(NOItemp < avgTemp[j]){
			NOIday = j;
			NOItemp = avgTemp[j];
		}
	}
	
	//Process of displaying 
	//printf("Day: %d\n", NOIday); 
	//printf("%f\n", avgTemp[NOIday]);
	
	printf("Hottest Temperature: %.6f\n", avgTemp[NOIday]);
	dayToDate(NOIday);	//Convert day to date 
    
    
    //Process of finding which day contains coldest temperature
    NOIday = 0, NOItemp = 1000;
    for (int i = 0; i < day_count; i++) {
		if(NOItemp > avgTemp[i]){
			NOItemp = avgTemp[i];
			NOIday = i;
			//printf("Day: %d\n", NOIday); 
		}
    }
    //printf("Day: %d\n", NOIday); 
	//printf("%f\n", avgTemp[NOIday]);
    
    //Process of displaying 
    printf("Coldest Temperature: %.2f\n", avgTemp[NOIday]);
	dayToDate(NOIday);	//Convert day to date format
    
    
    
    //Summer Calc
    
    //Summer Calculation of Avg per year
    printf("\n\n\n");
    double summerTotalSum = 0;
    double summerSum = 0;
    double avg = 0;
    printf("Summer Average of Year:\n");
    for (int j = 0; j < MAX_VALUES; j++){
		for(int i = 172; i < 266; i++){
			if(summerDayArray[i][j] > 40){
				summerDayArray[i][j] = summerDayArray[i-1][j-1];
			}
			summerSum = summerSum + summerDayArray[i][j];
		}
		avg = summerSum / (266-172);
		summerSum = 0;	
		
		printf("%d  -  %.2lf\n", 1995 + j, avg);
		summerTotalSum = summerTotalSum + avg;
			 
	}
	
	//Summer Avg Total Calculation
	double sumAvg = summerTotalSum / 30;
	printf("\n");
	printf("\n");
	printf("Summer Average of Lake Over 30 Years...\n");
	printf("%.2f", sumAvg);
	summerTotalSum = 0;
	
	
	printf("\n\n\n");
	
	return sumAvg;	//returning summer average of 30 years to main
	
}


int main() {
	char* filename;
	
	//Array of names of lakes
	char name[6][20];
	strcpy(name[0], "Lake Superior");
	strcpy(name[1], "Lake Michigan");
	strcpy(name[2], "Lake Huron");
	strcpy(name[3], "Lake Ontario");
	strcpy(name[4], "Lake Erie");
	strcpy(name[5], "Lake St Clair");
	
	
	double lakeSummerAvg[5]; //Array of lake summer averages of past 30 years
	
	
	printf("Lake Superior Data:\n");							
	filename = "all_year_glsea_avg_s_C.csv"; 				//	Setting Temperature File to Be open
	lakeSummerAvg[0] = readTempArray(filename);				//	Storing value of summer average of past 30 years into an array

	printf("\n\nLake Michigan Data:\n");
	filename = "all_year_glsea_avg_m_C.csv"; 				
	lakeSummerAvg[1] = readTempArray(filename);
	
	printf("\n\nLake Huron Data:\n");
	filename = "all_year_glsea_avg_h_C.csv"; 				
	lakeSummerAvg[2] = readTempArray(filename);
	
	printf("\n\nLake Ontario Data:\n");
	filename = "all_year_glsea_avg_o_C.csv"; 				
	lakeSummerAvg[3] = readTempArray(filename);
	
	printf("\n\nLake Erie Data:\n");
	filename = "all_year_glsea_avg_e_C.csv"; 				
	lakeSummerAvg[4] = readTempArray(filename);
	
	printf("\n\nLake St. Clair Data:\n");					
	filename = "all_year_glsea_avg_c_C.csv"; 				
	lakeSummerAvg[5] = readTempArray(filename);	

	//Sorting summer averages coldest to hottest and also sorting the names too
	for(int j = 0; j < 6; j++){
        for(int k = 0; k < 6; k++){
            if(lakeSummerAvg[k] > lakeSummerAvg[k+1]){
				double num = lakeSummerAvg[k];
				char temp = *name[k];
                lakeSummerAvg[k] = lakeSummerAvg[k+1];
                lakeSummerAvg[k+1] = num;
                
                *name[k] = *name[k+1];
                *name[k+1] = temp;
            }
        }
    }
    
    //Display Summer Averages hottest to coldest with respective lake
    printf("\n\n");
    printf("Hotest to Coldest Summer Average over 30 years of the Lakes\n\n");
    for(int i = 5; i> -1; i--){
		printf("%14s - %lf\n", name[i], lakeSummerAvg[i]);
	}
	
    return 0;
}
