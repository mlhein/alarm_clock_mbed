#include "mbed.h"
#include "TextLCD.h"
#include <string>
using namespace std;

TextLCD lcd(p19, p20, p25, p22, p23, p24); //rs, e, d4-d7
Ticker blinker_cursor, blinker_numer, blinker_menu, clock_viewer, check_alarm, sound_alarm, just_alarm;
InterruptIn joycenter(p14);
InterruptIn up(p15);
InterruptIn down(p12);
InterruptIn left(p13);
InterruptIn right(p16);
Serial pc(USBTX, USBRX); // tx, rx
PwmOut buzzer(p26);
Timer idle_state;

int row, col, r_hour, r_min, r_sec, r_apm, epoch, alarm_hour, alarm_min, alarm_sec, alarm_apm;
bool state_init, state_menu, state_alarm, alarm_up, state_set, alarm_ring;
string alarm_string, clock_string;

/*
 cursor {left,right} movement only work in state_init, state_alarm, state_menu, state_set
*/
void cursor_left()
{
    if(state_init) {
        if (col >0 && col <13) {
            if (col == 12)
                col = 10;
            else if (col ==10)
                col = 7;
            else if (col ==7)
                col = 4;
            else if (col == 4) {
                col =3;
                row =1;
            } else if (col ==3 && row == 1) {
                col = 12;
                row = 0;
            } else
                col = 4;
        } else
            col = 4;
    } else if(state_menu) {
        if (col >0 && col <13) {
            if(col == 3) {
                col = 9;
            } else if(col == 9) {
                col = 3;
            } else {
                col=3;
                row=1;
            }
        } else {
            col =3;
            row =1;
        }
    } else if(state_alarm) {
        if (col >0 && col <13) {
            if (col == 12)
                col = 10;
            else if (col ==10)
                col = 7;
            else if (col ==7)
                col = 4;
            else if (col == 4) {
                col =3;
                row =1;
            } else if (col ==3 && row == 1) {
                col = 9;
                row = 1;
            } else if( col == 9 && row ==1) {
                col = 12;
                row = 0;
            } else
                col = 4;
        } else {
            col=3;
            row=1;
        }
    } else if (state_set) {
        if (col >0 && col <13) {
            if (col == 12)
                col = 10;
            else if (col ==10)
                col = 7;
            else if (col ==7)
                col = 4;
            else if (col == 4) {
                col =3;
                row =1;
            } else if (col ==3 && row == 1) {
                col = 9;
                row = 1;
            } else if (col ==9 && row==1) {
                col = 12;
                row = 0;
            } else {
                col = 4;
            }
        } else {
            col = 9;
            row = 1;
        }
    }
    pc.printf("Cursor left: %i,%i\r\n", col, row);
}

void cursor_right()
{
    if(state_init) {
        if (col >0 && col <13) {
            if (col == 12) {
                col =3;
                row =1;
            } else if (col ==10)
                col = 12;
            else if (col ==7)
                col = 10;
            else if (col == 4) {
                col = 7;
            } else if (col ==3 && row == 1) {
                col = 4;
                row = 0;
            } else
                col = 4;
        } else
            col = 4;
    } else if(state_menu) {
        if (col >0 && col <13) {
            if(col == 3) {
                col = 9;
            } else if(col == 9) {
                col = 3;
            }
        } else {
            col = 3;
            row =1;
        }
    } else if(state_alarm) {
        if (col >0 && col <13) {
            if (col == 12) {
                col =9;
                row =1;
            } else if (col ==10)
                col = 12;
            else if (col ==7)
                col = 10;
            else if (col == 4) {
                col = 7;
            } else if (col ==3 && row == 1) {
                col = 4;
                row = 0;
            } else if(col == 9 && row == 1) {
                col = 3;
                row = 1;
            } else
                col = 4;
        } else {
            col = 3;
            row=1;
        }
    } else if (state_set) {
        if (col >0 && col <13) {
            if (col == 12) {
                col =9;
                row =1;
            } else if (col ==10)
                col = 12;
            else if (col ==7)
                col = 10;
            else if (col == 4) {
                col = 7;
            } else if (col ==3 && row == 1) {
                col = 4;
                row = 0;
            } else if (col == 9 && row ==1) {
                col = 3;
                row = 1;
            } else {
                col = 4;
            }
        } else {
            col = 9;
            row = 1;
        }
    }
    pc.printf("Cursor right: %i,%i\r\n", col, row);
}
/*
  chg_time_{up,down} only functing at state_init or state_alarm
*/
void chg_time_up()
{
    if(state_init or state_set) {
        if (col ==10) {
            r_sec = r_sec +1;
            if (r_sec > 59)
                r_sec = 0;
        } else if (col ==7) {
            r_min = r_min + 1;
            if (r_min > 59)
                r_min = 0;
        } else if (col ==4) {
            r_hour = r_hour + 1;
            if (r_hour > 12)
                r_hour = 1;
        } else if (col == 12) {
            if(r_apm==0)
                r_apm=1;
            else
                r_apm=0;
        } else
            pc.printf("lmfao");
        pc.printf("\nUP: %d:%d:%d %d\r\n",  r_hour, r_min, r_sec, r_apm);
    } else if(state_alarm) {
        if (col ==10) {
            alarm_sec = alarm_sec +1;
            if (alarm_sec > 59)
                alarm_sec = 0;
        } else if (col ==7) {
            alarm_min = alarm_min + 1;
            if (alarm_min > 59)
                alarm_min = 0;
        } else if (col ==4) {
            alarm_hour = alarm_hour + 1;
            if (alarm_hour > 12)
                alarm_hour = 1;
        } else if (col == 12) {
            if(alarm_apm==0)
                alarm_apm=1;
            else
                alarm_apm=0;
        } else
            pc.printf("lmfao");
        pc.printf("\nUP: %d:%d:%d %d\r\n",  alarm_hour, alarm_min, alarm_sec, alarm_apm);
    }

}

void chg_time_down()
{
    if(state_init or state_set) {
        if (col ==10) {
            r_sec = r_sec - 1;
            if (r_sec < 0)
                r_sec = 59;
        } else if (col ==7) {
            r_min = r_min - 1;
            if (r_min < 0)
                r_min = 59;
        } else if (col ==4) {
            r_hour = r_hour - 1;
            if (r_hour < 1)
                r_hour = 12;
        } else if (col == 12) {
            if(r_apm==0)
                r_apm=1;
            else
                r_apm=0;
        } else
            pc.printf("lmfao");
        pc.printf("\nDown clock: %d:%d:%d %d\r\n",  r_hour, r_min, r_sec, r_apm);
    } else if(state_alarm) {
        if (col ==10) {
            alarm_sec = alarm_sec - 1;
            if (alarm_sec < 0)
                alarm_sec = 59;
        } else if (col ==7) {
            alarm_min = alarm_min - 1;
            if (alarm_min < 0)
                alarm_min = 59;
        } else if (col ==4) {
            alarm_hour = alarm_hour - 1;
            if (alarm_hour < 1)
                alarm_hour = 12;
        } else if (col == 12) {
            if(alarm_apm==0)
                alarm_apm=1;
            else
                alarm_apm=0;
        } else
            pc.printf("lmfao");
        pc.printf("\nDown alarm: %d:%d:%d %d\r\n",  alarm_hour, alarm_min, alarm_sec, alarm_apm);
    }
}
/*
  Display sections.
*/
void show_menu()
{
    if(alarm_up) {
        lcd.locate(2,1);
        lcd.printf("*");
    } else {
        lcd.locate(2,1);
        lcd.printf(" ");
    }
    lcd.locate(3,1);
    lcd.printf("Alarm");
    lcd.locate(9,1);
    lcd.printf("Set");
}
void view_clock()
{
    lcd.locate(3,0);
    time_t seconds = time(NULL);
    char buffer[32];
    strftime(buffer, 32, "%I:%M:%S %p", localtime(&seconds));
    lcd.printf("%s", buffer);

    //Print main Menu
    show_menu();
    // Update our clock string.
    clock_string = buffer;
}
void blink_cursor()
{
    lcd.locate(col, row);
    lcd.printf("_");
}
void blink_num()
{

    if (r_hour > 9) {
        lcd.locate(3,0);
        lcd.printf("%d:", r_hour);
    } else {
        lcd.locate(3,0);
        lcd.printf("0%d:", r_hour);
    }
    if (r_min > 9) {
        lcd.locate(6,0);
        lcd.printf("%d:", r_min);
    } else {
        lcd.locate(6,0);
        lcd.printf("0%d:", r_min);
    }
    if (r_sec > 9) {
        lcd.locate(9,0);
        lcd.printf("%d", r_sec);
    } else {
        lcd.locate(9,0);
        lcd.printf("0%d", r_sec);
    }
    lcd.locate(12,0);
    if (r_apm ==0)
        lcd.printf("AM");
    else
        lcd.printf("PM");
    lcd.locate(3,1);
    lcd.printf("Done");

    if (state_set) { // Print "Back" if this is state_set
        lcd.locate(9,1);
        lcd.printf("Back");
    }
    return;
}

void alarm_disp()
{
    lcd.cls();
    if (alarm_hour > 9) {
        lcd.locate(3,0);
        lcd.printf("%d:", alarm_hour);
    } else {
        lcd.locate(3,0);
        lcd.printf("0%d:", alarm_hour);
    }
    if (alarm_min > 9) {
        lcd.locate(6,0);
        lcd.printf("%d:", alarm_min);
    } else {
        lcd.locate(6,0);
        lcd.printf("0%d:", alarm_min);
    }
    if (alarm_sec > 9) {
        lcd.locate(9,0);
        lcd.printf("%d", alarm_sec);
    } else {
        lcd.locate(9,0);
        lcd.printf("0%d", alarm_sec);
    }
    lcd.locate(12,0);
    if (alarm_apm ==0)
        lcd.printf("AM");
    else
        lcd.printf("PM");
    lcd.locate(3,1);
    lcd.printf("Off");
    lcd.locate(9,1);
    lcd.printf("On");
    return;
}

void alarm_sound()
{
    float frequency[]= {659,554,659,554,440,494,554,587,494,659,554,440}; //frequency array
    float beat[]= {1,1,1,1,1,0.5,0.5,1,1,1,1,2};             //beat array
    lcd.cls();
    lcd.printf("Press button to off alarm");
    while(1) {
        for(int i=0; i<=11; i++) {
            buzzer.period(1/(2*frequency[i]));// set PWM period
            buzzer=0.8;// set duty cycle
            wait(0.4*beat[i]);// hold for beat period          
        }
        if(joycenter) { //Off the alarm
            buzzer.period(0);
            buzzer=0;
            col=0;
            row=0;
            lcd.cls();
            break;
        }
    }
}


//Function alarm_chk is in progress
void alarm_chk()
{
    //we basically, comparing 2 strings. alarm string and the clock strings if match we ring it
    //we want to disable cursor movement.
    if(clock_string == alarm_string) {
        pc.printf("%s %s : same\r\n", alarm_string, clock_string);
        //Disable cursor movement.
        state_menu = false;
        state_set = false;
        state_alarm = false;
        blinker_cursor.detach();
        alarm_sound();
    }
    blinker_cursor.attach(&blink_cursor, 0.1);
}

void __set_alarm_string()
{
    char buffer_alarm[32];
    alarm_string.clear();
    if(alarm_hour <9) {
        sprintf (buffer_alarm, "0%d:", alarm_hour);
    } else {
        sprintf (buffer_alarm, "%d:", alarm_hour);
    }
    alarm_string += buffer_alarm;
    if (alarm_min <9) {
        sprintf (buffer_alarm, "0%d:", alarm_min);
    } else {
        sprintf (buffer_alarm, "%d:", alarm_min);
    }
    alarm_string += buffer_alarm;
    if (alarm_sec<9) {
        sprintf (buffer_alarm, "0%d", alarm_sec);
    } else {
        sprintf (buffer_alarm, "%d", alarm_sec);
    }
    alarm_string += buffer_alarm;
    if(alarm_apm ==0) {
        sprintf (buffer_alarm, " %s", "AM");
    } else {
        sprintf (buffer_alarm, " %s", "PM");
    }
    alarm_string += buffer_alarm;
}

/*
  state section.
*/
void setup_alarm()
{
    state_alarm = true;
    state_menu = false;
    just_alarm.attach(&alarm_disp,0.5);
    while(1) {
        wait(1);
        if(col == 3 && row ==1) {
            if(joycenter) { //Turn off Alarm and back to main menu
                just_alarm.detach();//stop alarm_disp
                state_alarm = false;
                state_menu = true;
                alarm_up = false;
                check_alarm.detach();
                alarm_string.clear();
                col=0;
                row=0;
                break;
            }
        } else if(col == 9 && row == 1) {//Turn on Alarm and back to main menu
            if(joycenter) {
                just_alarm.detach();
                state_alarm = false;
                state_menu = true;
                alarm_up = true;
                // Turn on the alarm.
                check_alarm.attach(&alarm_chk,1);
                // Set alarm string
                __set_alarm_string();
                col=0;
                row=0;
                break;
            }
        }
        state_alarm = true;
        state_menu = false;
    }
}

void setup_init()
{
    col=0;
    row=0;
    if (state_set) {
        state_init = false;
        state_menu = false;
    } else {
        state_init = true;
        state_menu = false;
    }

    lcd.cls();
    blinker_cursor.attach(&blink_cursor, 0.1);
    blinker_numer.attach(&blink_num, 0.5);
    while (1) {
        wait(1);
        // This is where, we select "Done", and we press joycenter to end the initial setup.
        if(col==3 && row==1) {
            if(joycenter) {
                blinker_numer.detach(); // Stop viewing number.
                blinker_cursor.detach(); // Stop blinking the cursor.

                //Calculate the unix timestamp
                //1h : 3600, 1m: 60, 1s: 1
                if(r_apm==0) { // AM
                    if(r_hour ==12)
                        epoch = (0 *3600) + (r_min *60) +(r_sec);
                    else
                        epoch = (r_hour *3600) + (r_min *60) +(r_sec);
                } else if (r_apm ==1) { //PM
                    if(r_hour ==12)
                        epoch = (r_hour *3600) + (r_min *60) +(r_sec);
                    else
                        epoch = 43200+(r_hour *3600) + (r_min *60) +(r_sec);
                }

                pc.printf("%d\r\n", epoch);
                // Set time base on epoch value.
                set_time(epoch);

                // When setup_init() done, we move the cursor to default state,
                row=0;
                col=0;

                state_init = false;
                state_menu = true;
                state_set = false;
                break;
            }
        } else if (col ==9 && row ==1) {//back button is pressed
            if(joycenter) { // When setup_init() done, we move the cursor to default state,
                blinker_numer.detach(); // Stop viewing number.
                blinker_cursor.detach(); // Stop blinking the cursor.
                row=0;
                col=0;
                state_init =false;
                state_menu = true;
                state_set = false;
                break;
            }
        }
    }
}

/*
  Always back to main_menu state.
*/
void main_menu()
{
    state_menu = true;
    // Start viewing clock!
    clock_viewer.attach(&view_clock,1);
    blinker_cursor.attach(&blink_cursor, 0.1);
    lcd.cls();
    idle_state.start();
    int timer_keeper;
    while(1) {
        wait(1);
        timer_keeper = idle_state.read();
        if(col == 3 && row == 1) {
            if(joycenter) {
                clock_viewer.detach();
                setup_alarm();
                clock_viewer.attach(&view_clock,1);
            }
        } else if(col == 9 && row ==1) {
            if(joycenter) {
                clock_viewer.detach();
                state_set = true;
                setup_init();
                blinker_cursor.attach(&blink_cursor, 0.1);
                lcd.cls();
                clock_viewer.attach(&view_clock,1);
            }
        }
        if(up or down or right or left) { // if have movement,
            idle_state.reset();
            blinker_cursor.attach(&blink_cursor, 0.1);
        } else { //no movement
            if (timer_keeper >5) {
                blinker_cursor.detach();
            } else
                blinker_cursor.attach(&blink_cursor, 0.1);
        }

        //}
        state_menu = true;
    }
}


/*
  main function, duh.
*/
int main()
{
    lcd.locate(0,0);
    lcd.printf("Press button to start");
    while(1) {
        if(joycenter) {
            break;
        }
    }
    // Initial value for our cursor
    row =0 ;
    col=0 ;

    // Initial value for alarm
    alarm_hour=12;
    alarm_min=0;
    alarm_sec=0;
    alarm_apm=0;
    // Initial value for clock
    r_hour=12 ;
    r_min=0 ;
    r_sec = 0;
    r_apm = 0; //0-AM, 1-PM
    epoch = 0; // 12:00:00 AM

    // Set joystick to interrupt and call specific function the cursor value.
    left.rise(&cursor_left);
    right.rise(&cursor_right);
    up.rise(&chg_time_up);
    down.rise(&chg_time_down);

    // Initialize all the state to false.
    state_init = false;
    state_menu = false;
    state_alarm = false;
    alarm_up = false;
    state_set = false;
    alarm_ring = false;

    /*Run setup init -> show main menu
                        |_ setup Alarm (state_alarm)
                        |   |_Off alarm
                        |   |_On alarm
                        |_ Change time (state_init)

      The up/down only work in state_init and state_alarm
      The left/right should work on every state except in state which alarm is ringging.

      Every time we choose the option in any state, the default value for cursor is
        col=0 and row=0. This to avoid accidently go to another state.
    */

    setup_init();
    main_menu();
}
