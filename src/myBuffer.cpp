#include <iostream>
#include "MyBuffer.h"


void MyBuffer::validatePush(){
    if (pushPointer == bufferSize){
        reset = !reset;
        pushPointer = 0;
        numberOfElements--;
    }
}

void MyBuffer::validatePop(){
    if (popPointer == bufferSize){
        popPointer = 0;
    }
}

void MyBuffer::push(uint16_t element){
    arr[pushPointer] = element;
    pushPointer++;
    if (!reset){
        elements++;
    }
    validatePush();
}

uint16_t MyBuffer::pop(){
        uint16_t returnV = arr[popPointer];
        popPointer++;
        elements--;
        validatePop();
        return returnV;
}

bool MyBuffer::elementsLeft() const {
    return elements > 0;
}