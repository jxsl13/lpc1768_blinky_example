
#include <LPC1768/LPC17xx.h>
#include <map>
#include <iostream>

extern "C" int main(void);

void Setup()
{

}

int main() {
    Setup();

    std::map<int, int> PinMap;

    for (size_t i = 0; i < 512; i++)
    {
        PinMap.insert({i, i * i});
    }
    
}