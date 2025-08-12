#include "avr_utils.h"

// Declare your global variables here
unsigned int read_adc(unsigned char adc_input)
{
      ADMUX = adc_input | ADC_VREF_TYPE;
      delay_us(1);
      
      ADCSRA|=0x40;
      
      while ((ADCSRA & 0x10)==0);
      ADCSRA|=0x10;
      return ADCW;
}

void get_input(InputDoControle *input)
{
      input->x_axis = read_adc(MUX_ADC0_XAXIS);
      input->y_axis = read_adc(MUX_ADC1_YAXIS);
      input->stick_down = STICK_DOWN;
}

void inicia_registradores()
{

    // SPI init 
    // MOSI (PB3), SCK (PB5) e SS (CS) (PB2) como saídas; 
    // MISO (PB4) como entrada;
    PORTB=0x00;
    DDRB=0b00010110;

    PORTC=0x00;
    DDRC=0x00;

    // Port D initialization
    // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
    // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
    PORTD=0x00;
    DDRD=0x00;

    // Timer/Counter 0 initialization
    // Clock source: System Clock
    // Clock value: Timer 0 Stopped
    TCCR0=0x00;
    TCNT0=0x00;

    // Timer/Counter 1 initialization
    // Clock source: System Clock
    // Clock value: Timer1 Stopped
    // Mode: Normal top=0xFFFF
    // OC1A output: Discon.
    // OC1B output: Discon.
    // Noise Canceler: Off
    // Input Capture on Falling Edge
    // Timer1 Overflow Interrupt: Off
    // Input Capture Interrupt: Off
    // Compare A Match Interrupt: Off
    // Compare B Match Interrupt: Off
    TCCR1A=0x00;
    TCCR1B=0x00;
    TCNT1H=0x00;
    TCNT1L=0x00;
    ICR1H=0x00;
    ICR1L=0x00;
    OCR1AH=0x00;
    OCR1AL=0x00;
    OCR1BH=0x00;
    OCR1BL=0x00;

    // Timer/Counter 2 initialization
    // Clock source: System Clock
    // Clock value: Timer2 Stopped
    // Mode: Normal top=0xFF
    // OC2 output: Disconnected
    ASSR=0x00;
    TCCR2=0x00;
    TCNT2=0x00;
    OCR2=0x00;

    // External Interrupt(s) initialization
    // INT0: Off
    // INT1: Off
    MCUCR=0x00;

    // Timer(s)/Counter(s) Interrupt(s) initialization
    TIMSK=0x00;

    // USART initialization
    // Communication Parameters: 8 Data, 1 Stop, No Parity
    // USART Receiver: Off
    // USART Transmitter: On
    // USART Mode: Asynchronous
    // USART Baud Rate: 19200
    UCSRA=0x00;
    UCSRB=0x08;
    UCSRC=0x86;
    UBRRH=0x00;
    UBRRL=0x2F;

    // Analog Comparator initialization
    // Analog Comparator: Off
    // Analog Comparator Input Capture by Timer/Counter 1: Off
    ACSR=0x80;
    SFIOR=0x00;

    // ADC initialization
    // ADC Clock frequency: 921,600 kHz
    // ADC Voltage Reference: Int., cap. on AREF
    // ADC Auto Trigger Source: ADC Stopped
    ADMUX = MUX_ADC0_XAXIS;
    ADCSRA = 0x84;

    // SPI initialization
    // SPI enable, master, clk/16
    SPCR=0b01010001;

    // TWI initialization
    // TWI disabled
    TWCR=0x00;
}