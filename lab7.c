#include "mte220.c"  // C functions needed for labs

/*
;**********************************************************************
;
;    Filename:	    lab6.c
;    Date:          Nov 12/05
;    File Version:  3.1
;
;    Author:        C.C.W. Hulls & D.L. Wight
;
;    Copyright (c) C.C.W. Hulls, P.Eng and D.L. Wight, 2005.
;    This software can be used and modified by students, staff and
;    faculty members at the University of Waterloo for teaching or
;    research purposes, provided the authors are acknowledged.
;
;**********************************************************************
;
;    Files required:
;
;**********************************************************************
;
;    History:
;
;    3.1  Nov 12/05  use new functions and macros;
;    3.0  Nov  7/05  main code split into separate file that loads the
;                    functions needed for MTE 220
;
;    2.0  Nov  5/05  ported to C;
;
;    1.1  Oct 25/05  changed the phase of right servo to be midway
;                    through the entire cycle; enable peripheral interrupts
;                    as part of interrupt initialization
;    1.0  Oct 24/05  original release
;
;**********************************************************************
;**********************************************************************
;
;    Description:
;
;   The lab 6 code reads the IR sensors difference amplifier.  The IR
;   sensors are used for line following, so when a noticeable difference
;   is detected, the appropriate servo is turned off.
;
;**********************************************************************
*/

//;**********************************************************************
void main(void)
{
    uns8 analog_value;  // current ADC value
    uns8 analog_valueHall;
    uns8 analog_valueHall_2;
	uns8 analog_valueHall_3;
	uns8 analog_valueHall_4;
    uns16 avg; 

    Initialization();
    WaitForButton();
	
	while(1){
		
		//turn on servos
		SetLeft(3340);
		SetRight(3620);
		Delay(10);
		SetLeft(3040);
		SetRight(3860);
		Delay(10);
		LeftServoOn
		RightServoOn

		LongDelay(2); // get out of magnets way
		
		analog_valueHall = AnalogConvert(ADC_HALL_EFFECT);
		Delay(200);
		analog_valueHall_2 = AnalogConvert(ADC_HALL_EFFECT);
		Delay(200);
		analog_valueHall_3 = AnalogConvert(ADC_HALL_EFFECT);
		Delay(200);
		analog_valueHall_4 = AnalogConvert(ADC_HALL_EFFECT);
		avg = analog_valueHall;
		avg += analog_valueHall_2;
		avg += analog_valueHall_3;
		avg += analog_valueHall_4;
		avg = avg / 4; 
		
		while(avg > 0x6E && avg < 0x99) { // no magnet detected
			OffLED
			analog_value = AnalogConvert(ADC_IR_SENSOR);  // get analog value from IR sensor diff amp
			if (analog_value < 0x66) { // 0x66 is 2V for 10-bit ADC with 2 LSB dropped
				// left servo only
				LeftServoOn
				RightServoOff
			} else if (analog_value > 0x99) { // 0x99 is 3V for 10-bit ADC with 2 LSB dropped
				// right servo only
				RightServoOn
				LeftServoOff
			} else {
				//both servos on
				BothServosOn
			}
            
			analog_valueHall = AnalogConvert(ADC_HALL_EFFECT);
			Delay(50);
			analog_valueHall_2 = AnalogConvert(ADC_HALL_EFFECT);
			Delay(50);
			analog_valueHall_3 = AnalogConvert(ADC_HALL_EFFECT);
			Delay(50);
			analog_valueHall_4 = AnalogConvert(ADC_HALL_EFFECT);
			avg = analog_valueHall;
			avg += analog_valueHall_2;
			avg += analog_valueHall_3;
			avg += analog_valueHall_4;
			avg = avg / 4; 
		}

		//stopping
		SetLeft(2650);
		SetRight(4130);
		Delay(10);
		SetLeft(2950);
		SetRight(3890);
		Delay(10);
		LeftServoOff
		RightServoOff

		if (analog_valueHall>0x99){ // 3V solid
			OnLED
			LongDelay(56);
			OffLED 
		}
		else if (analog_valueHall<0x6E){//2.15v blink
			OnLED
			LongDelay(8);
			OffLED
			LongDelay(8);
			OnLED
			LongDelay(8);
			OffLED
			LongDelay(8);
			OnLED
			LongDelay(8);
			OffLED
			LongDelay(8);
			OnLED
			LongDelay(8);
			OffLED
		
		}
        	
	}
}

