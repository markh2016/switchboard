# switchboard
switch bourd with timer for raspberry pi   inclusive clock timer 
I have yet more work to do on this  Fully functional up to now  though 



Setting  of the switch broad itself 

1: make a directory on your SD car on the Raspberry  and call this  switchboard 
2: CD into this folder an either download all via  clone  or use  git clone https://github.com/markh2016/switchboard.git Alternatively  down load in zip format and  unzip contents to that folder 

3 : Although the executable is  pre compiled  I would run  the command “ make clean  from terminal in that directory    then run make  This will produce the required excitable 

4:  From here its a matter of connect  some led to start with with R470R resistors between ports and ground 

The  BCM Pins for all led’s are marked and commented in full  in the C file 

These are marked under this section as listed below  in the SRC  folder 

#define PIN0 RPI_GPIO_P1_11		// gpio_0 pin 11


#define PIN1 RPI_GPIO_P1_12		// gpio_1 pin 12

#define PIN2 RPI_GPIO_P1_13		// gpio_2 this is on pin21 

#define PIN3 RPI_GPIO_P1_15		// gpio_3 pin 15

#define PIN4 RPI_GPIO_P1_16		// gpio_4 pin 16

#define PIN5 RPI_GPIO_P1_18		// gpio_5 pin 18


The related directly to the break out board which Im using correctly for prototyping


This  you can find at this location 

 
https://shop.cyntech.co.uk/collections/raspberry-pi/products/40-way-raspberry-pi-b-split



In the meantime enjoy  and have fun 


Mark Harrington 01:59Hrs  02 May 2020
