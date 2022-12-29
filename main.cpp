#include <iostream>
#include <random>
#include <chrono>
#include <pthread.h>

using namespace std::chrono;


 struct sub_array_t
 {
    int* array;
    int start;
    int end;
};


void* sum_subarray(void* args)
{
    sub_array_t* sub_array = (sub_array_t*) args;

    int* result = new int{0};
    for(int i = sub_array->start; i < sub_array->end; ++i)
    {
        *result += sub_array->array[i];
    }

    delete sub_array;

    return result;
}


int main(int argc,const char** argv)
{
   
    if(argc < 3)
    {
        std::cerr << "Less arguments" << std::endl;
        return -1;
    }



   
    const char* N_size = argv[1];
    const char* M_size = argv[2];

   
    int N = atoi(N_size);
    int M = atoi(M_size);

   
    int* array = new int[N];

   
    for(int i = 0; i < N; ++i)
    {
        array[i] = rand()%1000;
    }
    
    int threads_sum = 0;

   
    pthread_t* threads = new pthread_t[M];

    for(int i = 0; i < M; ++i)
    {
       
        sub_array_t* sub_array = new sub_array_t;
        sub_array->array = array;
        sub_array->start = i * (N / M);
        sub_array->end = sub_array->start + (N / M);

       
        int result = pthread_create(&threads[i], NULL, sum_subarray, sub_array);
        
        
        if(result < 0)
        {
            exit(result);
        }

    }
    
   
    auto start_threads = high_resolution_clock::now();

    for(int i = 0; i < M; ++i)
    {
       
        void* result;
        int join_result = pthread_join(threads[i], &result);
        int* sub_sum = (int*) result;
        threads_sum += *sub_sum;

       
        if(join_result < 0)
        {
            exit(join_result);
        }

        delete sub_sum;
    }

    
    auto stop_threads = high_resolution_clock::now();

   
    auto duration_threads = duration_cast<nanoseconds>(stop_threads - start_threads);

    std::cout << "Time spent with threads: " << duration_threads.count() << std::endl;
    
    
    int sum = 0;
    auto begin = high_resolution_clock::now();
    for(int i = 0; i < N; ++i){
        sum += array[i];
    }
    auto end = high_resolution_clock::now();

    int total_sum = 0;

   
    auto start = high_resolution_clock::now();

    
    for(int i = 0; i < N; ++i)
    {
        total_sum += array[i];
    }

   
    auto stop = high_resolution_clock::now();

   
    auto duration = duration_cast<nanoseconds>(stop - start);

    std::cout << "Time spent without threads: " << duration.count() << std::endl;
    

   
    delete [] array;
    delete [] threads;

    return 0;
}
