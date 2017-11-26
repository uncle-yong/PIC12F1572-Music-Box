#include "songdata.h"
#include "tuningwords.h"

// Christmas midi source: https://www.mfiles.co.uk/christmas-music-and-carols.htm
// midi notes to frequency: http://subsynth.sourceforge.net/midinote2freq.html
// Note: Tuning words in this code have nothing to do with the DDS module whatsoever! It is named tuning words because
//       this was ported from another code that uses DDS. Todo: change to PeriodRegister!

unsigned int time_play = 0;
unsigned int songIndex = 0;
unsigned char opcode = 0;
unsigned char chan = 0;
unsigned char cmd = 0;
unsigned char isUpdateNote = 1;
unsigned char isPlaying = 0;
unsigned int time_play_count = 0;

extern const unsigned int tuningWords[128];

void timer1interrupt() iv 0x0004 ics ICS_AUTO {
     if(time_play_count > time_play) {
        time_play_count = 0;
        isUpdateNote = 1;
     }
     else
        time_play_count++;
     
     //PORTA.RA5 ^= 1;
     TMR1 = 64536;
     PIR1.TMR1IF = 0;
}

void init() {

    //OSCCON.SPLLEN = 1;      // PLL turned off.
    //OSCCON.IRCF = 0b1110;   // 8mhz internal oscillator.
    //OSCCON.SCS = 0b10;
    

    OSCCON.SPLLEN = 0; // disable Software PLL.
    OSCCON.IRCF3 = 1;  // 8mhz -> 32mhz if PLLx4 set.
    OSCCON.IRCF2 = 1;
    OSCCON.IRCF1 = 1;
    OSCCON.IRCF0 = 0;
    OSCCON.SCS1  = 0;  // system clock select: internal osc.
    OSCCON.SCS0  = 0;

    APFCON.P1SEL = 0;
    ANSELA = 0x00;
    TRISA = 0x00;
    LATA = 0x00;
    
    INTCON = 0x00;

}

void init_timer1() {
    // Timer1 clock source follows instruction clock!
    T1CON = 0x00;
    T1CON.T1CKPS0 = 1;     // 1:8 prescale value.
    T1CON.T1CKPS1 = 1;
    TMR1 = 64536;
    T1GCON = 0x00;
    T1CON.TMR1ON = 1;


    

}

void init_intr() {
     // Please look at the datasheet for the PIC12F1572 for more info!
     PIR1 = 0x00;
     PIE3 = 0x00;
     PIE1.TMR1IE = 1;
     INTCON.PEIE = 1;
     INTCON.GIE = 1;
}

void init_pwm1() {
    // Enable PWM1 module: output pin enable, output active state low.
    PWM1CON = 0x00;
    PWM1CON.PWM1POL = 0;
    PWM1CON.PWM1OE = 1;
    PWM1CON.PWM1EN = 0;

    // PWM Interrupts: all disable.
    PWM1INTE = 0x00;

    // PWM Clock Controller:
    PWM1CLKCON = 0x00;
    //PWM1CLKCON.PS = 0b011;    // divide clock source by 8.
    //PWM1CLKCON.CS = 0b01;     // source: HF internal osc.
    PWM1CLKCON.PWM1PS0 = 1;
    PWM1CLKCON.PWM1PS1 = 1;
    PWM1CLKCON.PWM1PS2 = 0;
    PWM1CLKCON.PWM1CS0 = 0;
    PWM1CLKCON.PWM1CS1 = 0;

    // PWM Timer Registers:
    PWM1TMR = 0;

    // PWM Phase Register:
    PWM1PH = 0;

    // PWM Period Count Registers:
    PWM1PR = 5000;

    // PWM Duty Cycle registers:
    //PWM1DCH = 0x08;
   // PWM1DCL = 0xe0;

    PWM1DC = 2500;

    PWM1LDCON.PWM1LD = 1;     // Load Buffer.

}

void init_pwm2() {
    // Enable PWM2 module: output pin enable, output active state low.
    PWM2CON = 0x00;
    PWM2CON.PWM2POL = 0;
    PWM2CON.PWM2OE = 1;
    PWM2CON.PWM2EN = 0;

    // PWM Interrupts: all disable.
    PWM2INTE = 0x00;

    // PWM Clock Controller:
    PWM2CLKCON = 0x00;
    //PWM1CLKCON.PS = 0b011;    // divide clock source by 8.
    //PWM1CLKCON.CS = 0b01;     // source: HF internal osc.
    PWM2CLKCON.PWM2PS0 = 1;
    PWM2CLKCON.PWM2PS1 = 1;
    PWM2CLKCON.PWM2PS2 = 0;
    PWM2CLKCON.PWM2CS0 = 0;
    PWM2CLKCON.PWM2CS1 = 0;

    // PWM Timer Registers:
    PWM2TMR = 0;

    // PWM Phase Register:
    PWM2PH = 0;

    // PWM Period Count Registers:
    PWM2PR = 8000;

    // PWM Duty Cycle registers:
    //PWM1DCH = 0x08;
   // PWM1DCL = 0xe0;

    PWM2DC = 4000;

    PWM2LDCON.PWM2LD = 1;     // Load Buffer.

}

void init_pwm3() {
    // Enable PWM3 module: output pin enable, output active state low.
    PWM3CON = 0x00;
    PWM3CON.PWM3POL = 0;
    PWM3CON.PWM3OE = 1;
    PWM3CON.PWM3EN = 0;

    // PWM Interrupts: all disable.
    PWM3INTE = 0x00;

    // PWM Clock Controller:
    PWM3CLKCON = 0x00;
    //PWM1CLKCON.PS = 0b011;    // divide clock source by 8.
    //PWM1CLKCON.CS = 0b01;     // source: HF internal osc.
    PWM3CLKCON.PWM3PS0 = 1;
    PWM3CLKCON.PWM3PS1 = 1;
    PWM3CLKCON.PWM3PS2 = 0;
    PWM3CLKCON.PWM3CS0 = 0;
    PWM3CLKCON.PWM3CS1 = 0;

    // PWM Timer Registers:
    PWM3TMR = 0;

    // PWM Phase Register:
    PWM3PH = 0;

    // PWM Period Count Registers:
    PWM3PR = 2500;

    // PWM Duty Cycle registers:
    //PWM1DCH = 0x08;
   // PWM1DCL = 0xe0;

    PWM3DC = 1250;

    PWM3LDCON.PWM3LD = 1;     // Load Buffer.

}

// UpdateNote - updates the note when it is finished playing. The function
//              parses the array which is generated by Len Shustek's Miditones.
void updateNote() {
    while(1) {
           cmd = songData1[songIndex];

           if(cmd < 0x80) {
              time_play = ( (songData1[songIndex] << 8) | songData1[songIndex+1] );
              songIndex += 2;
              break;
           }

           opcode = cmd & 0xf0;
           chan   = cmd & 0x0f;

           if(opcode == 0x80) {// stop note here!
               switch(chan) {
                  // when note stopped, reset the particular channel value here!
                  case 0: PWM1CON.PWM1EN = 0;
                          break;
                  case 1: PWM2CON.PWM2EN = 0;
                          break;
                  case 2: PWM3CON.PWM3EN = 0;
                          break;
                  default:
                          break;
              }
              songIndex += 1;

           }
           else if(opcode == 0x90) { // play note here!
               isUpdateNote = 1;
               switch(chan) {
                  // when updating note, place the note values into the channel!
                  case 0: PWM1PR = tuningWords[songData1[songIndex+1]];
                          PWM1DC = tuningWords[songData1[songIndex+1]] >> 1;
                          PWM1LDCON.PWM1LD = 1;     // Load Buffer.
                          PWM1CON.PWM1EN = 1;
                         break;
                  case 1: PWM2PR = tuningWords[songData1[songIndex+1]];
                          PWM2DC = tuningWords[songData1[songIndex+1]] >> 1;
                          PWM2LDCON.PWM2LD = 1;     // Load Buffer.
                          PWM2CON.PWM2EN = 1;
                         break;
                  case 2: PWM3PR = tuningWords[songData1[songIndex+1]];
                          PWM3DC = tuningWords[songData1[songIndex+1]] >> 1;
                          PWM3LDCON.PWM3LD = 1;     // Load Buffer.
                          PWM3CON.PWM3EN = 1;
                         break;
                  default:
                         break;
               }
               songIndex += 2;

           }

         else if(opcode == 0xf0) {  // stop playing score!
                isPlaying = 0;
                break;
         }

         else if(opcode == 0xe0) {  // start playing from beginning!
               songIndex = 0;
               time_play_count = 0;
               time_play = 0;
               break;
         }
       }
}

void main() {

    init();

    delay_ms(50);

    init_pwm1();
    init_pwm2();
    init_pwm3();
    
    init_intr();

    isPlaying = 1;
    
    init_timer1();

    while(1) {
       //PORTA.RA2 ^= 1;
       //delay_ms(500);
       if (isPlaying == 1)
        {
          if (isUpdateNote)
            {
        	updateNote();
        	isUpdateNote = 0;
            }
        }
      else
        {
          PWM1CON.PWM1EN = 0;
          PWM2CON.PWM2EN = 0;
          PWM3CON.PWM3EN = 0;
        }
    }
}