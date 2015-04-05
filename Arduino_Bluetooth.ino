/*******************************************************************************************************************************************
 * Arduino_Bluetooth.c
 * Arthor: Nathan Rauch
 * Data  : 4/5/2015
 * Description: This program is for my MSU Senior Design Class. The program reads values coming in to the analog pin A0 on the arduino
 * 				      and then transmites it through bluetooth to an Android application. The main users of this program will be the EELE101 
 * 	  			    Students at MSU Bozeman to see the data coming out of there cars Freescale Linear Camera. 
 *******************************************************************************************************************************************/
 
 /* Bluetooth Libraries */
#include <Nordic_nRF8001.h>
#include <RBL_nRF8001.h>
#include <SPI.h>

void sending(int values);

void setup() {
	ADCSRA &= ~(bit (ADPS0) | bit (ADPS1) | bit (ADPS2)); /* clear prescaler bits */
	ADCSRA |= bit (ADPS1);                                /* set prescaler to  4, this increases the rate in which the analog pin can be read */
	ble_set_name("EE101(1)");                             /* set what name wil show up on the bluetooth list */
	ble_begin();                                          /* setup bluetooth connection */
}

void loop() {
	int data =map(analogRead(A0),0,1023,0,256);           /* get camera data convert it to pixel grayscale range 0-255 */
	if(data != 0){                                        /* only read camera when its output data */
		if(data < 25){                                      /* threshhold to better see the line on the graph */ 
			data = 0;                                         /* if data is less than 25 make data 0 to better see the black line */  
		}
		sending(data);								                      /* send int value through bluetooth */
	}
}

void sending(int values){                               /* bluetooth sending function */
	char data[3];									                        /* character array which is used because bluetooth can only send chars */ 
	sprintf (data, "%03i",values);					              /* convert interger to char array of the size 3 */
    	if( ble_connected() ){						                /* check if arduino is connect to app */
    		ble_write(data[0]);							                /* get ready to send first byte */ 
    		ble_write(data[1]);							                /* get ready to send second byte */
    		ble_write(data[2]);						                  /* get ready to send third byte */
    	}
    ble_do_events();									                  /* transmite bytes to app */
}
