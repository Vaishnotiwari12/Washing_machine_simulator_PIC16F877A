/*
 * File:   main.c
 * Author: Madhire Kalyan Sathwik
 *
 * Created on 11 June, 2025, 7:57 PM
 */


#include"main.h"
#pragma config WDTE = OFF

unsigned int operational_state=washing_program_screen;
unsigned int reset_flag=reset_screen;
static void init_config(void){
    //to initialize clcd
    init_clcd();
    //to initialize digital keypad
    init_digital_keypad();
    //to initialize timmer
    init_timer2();
    //to enable global interrupt and peripheral interrupt
    GIE=1;
    PEIE=1;
    buzzer_ddr=0;
    fan_ddr=0;
    fan=off;
    buzzer = off;
    
}

void main(void) {
    init_config();
    unsigned char key;
    clcd_print("Press key 5 to",LINE1(1));
    clcd_print("Power ON",LINE2(5));
    clcd_print("Washing Machine",LINE3(1));
    while(read_digital_keypad(LEVEL) != SW5){
        for(unsigned int delay=3000;delay--;);
    }//waiting for switch 5 to be pressed
    clear_clcd();
    power_ON_screen();
    __delay_ms(1000);
    clear_clcd();
    
    while(1){
        key=read_digital_keypad(LEVEL);
        //to avoid bouncing affect
        for(unsigned int delay=3000;delay--;);
        if(key == LPSW4 && (operational_state ==washing_program_screen)){
            operational_state = water_level;
            reset_flag=reset_water_level_screen;
        }
        else if((key == LPSW4) && (operational_state == water_level)){
            operational_state = start_or_stop;
            clear_clcd();
            clcd_print("press switch",LINE1(0));
            clcd_print("SW5: START",LINE2(0));
            clcd_print("SW6: STOP",LINE3(0));
            set_time();
        }
        else if(key == SW5 && operational_state == pause){
            operational_state = start_screen;
            TMR2ON = 1;
            fan = on;
        }
        
        switch(operational_state){
                case washing_program_screen:
                    washing_program_(key);
                    break;
                case water_level:
                    water_level_screen(key);
                    break;
                case start_or_stop:
                    if(SW5==key){
                        operational_state = start_screen;
                        reset_flag = reset_start_screen;
                    }
                    else if(SW6==key){
                        operational_state=washing_program_screen;
                        reset_flag=reset_screen;
                        continue;
                    }
                    break;
                case start_screen:
                    start(key);
                    break;
        }         
        
    }
    return;
}
