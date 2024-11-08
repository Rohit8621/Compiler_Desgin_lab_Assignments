#include <stdio.h>

int a = 10;
int b = 20;
float c = 10.00;

while(1) {
    if(a > b) {
        c = a - b;
    }
    else {
        c = b - a;
    }
    a = a - 1;
    
}