#include <Arduino.h>

#ifndef RINGBUFFER_MYBUFFER_H
#define RINGBUFFER_MYBUFFER_H


class MyBuffer {

public:

    MyBuffer(int size){
        this->bufferSize = size;
    }

    void push(uint16_t element);

    uint16_t pop();

    void validatePush();

    void validatePop();

    bool elementsLeft() const;


private:
    bool reset = false;
    int elements = 0;
    int bufferSize;
    int numberOfElements = 0;
    int pushPointer = 0;
    int popPointer = 0;
    uint16_t arr[5000];

};


#endif //RINGBUFFER_MYBUFFER_H