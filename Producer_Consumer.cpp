#include<iostream>
#include<mutex>
#include<condition_variable>
#include<queue>
#include<thread>

std::mutex mtx;
std::condition_variable cv;

std::queue<int> buffer;
const int MAX_BUFFER_SIZE = 5;

void producer(int id) {
    int value =0;
    while(value <10) {
        
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] {return buffer.size() < MAX_BUFFER_SIZE; });
        
        buffer.push(value);
        std::cout<<"producer:"<<id<<"produced:"<<value<<std::endl;
        value++;
        
        cv.notify_all();
        
    }
    
}

void consumer(int id) {
    while(true) {
        
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] {return !buffer.empty(); });
        
        int item = buffer.front();
        buffer.pop();

        std::cout<<"consumer:"<<id<<"consumed:"<<item<<std::endl;
        cv.notify_all();
        
        if(item == 9) break;
        
    }
    
}


int main() {
    
    std::thread p1(producer, 1);
    std::thread c1(consumer, 1);
    
    p1.join();
    c1.join();
    
    return 0;
}
 
