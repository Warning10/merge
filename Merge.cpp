#include<iostream>
#include<cstdlib>
#include<omp.h>
#include<time.h>
using namespace std;

void merge(int array[],int low1, int high1,int low2,int high2, int n)
{
    int temp[n];
    int i=low1,j=low2,k=0;
    
    while(i<=high1 && j<=high2)
    {
        if(array[i]<array[j])
            temp[k++]=array[i++];
        else
            temp[k++]=array[j++];
    }
    
    while(i<=high1)
        temp[k++]=array[i++];
    
    while(j<=high2) 
        temp[k++]=array[j++];
        
    for(i=low1,j=0;i<=high2;i++,j++)
        array[i]=temp[j];
}

void mergesort_serial(int array[], int low, int high, int n)
{
    if(low<high)
    {
        int mid=(low+high)/2;
        mergesort_serial(array,low,mid,n);
        mergesort_serial(array,mid+1,high,n);
        merge(array,low,mid,mid+1,high,n);
    }
}

void mergesort_parallel(int array[], int low, int high, int n)
{
    if(low<high)
    {
        int mid=(low+high)/2;
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                mergesort_parallel(array,low,mid,n);
            }
            
            #pragma omp section
            {
                mergesort_parallel(array,mid+1,high,n);
            }
            
        }
        
        merge(array,low,mid,mid+1,high,n);
    }
}

void display(int array[], int n)
{
    for(int i=0;i<n;i++) cout<<array[i]<<" ";
}


int main()
{
    int n;
    cout<<"Enter the number of elements : ";
    cin>>n;
    
    int array[n] = {0};

    srand(time(NULL));
    for(int i=0;i<n;i++)
    {
        array[i]=rand()%32;
    }
    
    cout<<"Original Array: ";
    display(array,n);
    cout<<endl;
    
    // Serial Execution
    clock_t start_serial = clock();
    mergesort_serial(array,0,n-1,n);
    clock_t stop_serial = clock();
    cout<<"Final Array (Serial): ";
    display(array,n);
    cout<<endl;
    cout<<"Time required for serial execution: "<<(double)(stop_serial-start_serial)<<" ms"<<endl;

    // Reset array for parallel execution
    srand(time(NULL));
    for(int i=0;i<n;i++)
    {
        array[i]=rand()%32;
    }

    // Parallel Execution
    clock_t start_parallel = clock();
    mergesort_parallel(array,0,n-1,n);
    clock_t stop_parallel = clock();
    cout<<"Final Array (Parallel): ";
    display(array,n);
    cout<<endl;
    cout<<"Time required for parallel execution: "<<(double)(stop_parallel-start_parallel)<<" ms"<<endl;
    
    return 0;
}
