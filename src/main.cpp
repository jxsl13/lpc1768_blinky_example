/**
 * @brief These are being implemented in _example_impl.cpp
 *      for each target specifically.
 * @param int 
 */
extern void delay_ms(unsigned int);
extern void ToggleLED();
extern void DisableLED();

// clears external interrupt pending flag
extern void ClearIRQCondition();


void Blinking(unsigned int times = 1, unsigned int ms = 300)
{
    for (unsigned int i = 0; i < times * 2; i++)
    {
        delay_ms(ms);
        ToggleLED();
    } 
}

void PushButton_Handler()
{
    ClearIRQCondition();
}


int main()
{   
    
    DisableLED();

    while(1)
    {
        Blinking(2);
    } 
}


