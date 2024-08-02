EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:mcp1407
LIBS:sfh620a
LIBS:leg-12
LIBS:treiber-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date "14 may 2015"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCP1407 U3
U 1 1 55301172
P 6450 4100
F 0 "U3" H 5950 4700 60  0000 C CNN
F 1 "TC429" H 6450 3900 60  0000 C CNN
F 2 "~" H 6450 4100 60  0000 C CNN
F 3 "~" H 6450 4100 60  0000 C CNN
	1    6450 4100
	1    0    0    -1  
$EndComp
$Comp
L SFH620A U2
U 1 1 55301181
P 4100 4150
F 0 "U2" H 3650 4600 60  0000 C CNN
F 1 "SFH620A" H 4100 3800 60  0000 C CNN
F 2 "~" H 4100 4150 60  0000 C CNN
F 3 "~" H 4100 4150 60  0000 C CNN
	1    4100 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7450 3900 7450 4300
Wire Wire Line
	7450 4300 7250 4300
Wire Wire Line
	7450 3900 7250 3900
Connection ~ 7450 4100
$Comp
L GND #PWR01
U 1 1 553011ED
P 8000 4700
F 0 "#PWR01" H 8000 4700 30  0001 C CNN
F 1 "GND" H 8000 4630 30  0001 C CNN
F 2 "" H 8000 4700 60  0000 C CNN
F 3 "" H 8000 4700 60  0000 C CNN
	1    8000 4700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 553011FA
P 6450 5200
F 0 "#PWR02" H 6450 5200 30  0001 C CNN
F 1 "GND" H 6450 5130 30  0001 C CNN
F 2 "" H 6450 5200 60  0000 C CNN
F 3 "" H 6450 5200 60  0000 C CNN
	1    6450 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	8000 4700 8000 4500
Wire Wire Line
	8000 4500 8150 4500
Wire Wire Line
	6250 4900 6250 5050
Wire Wire Line
	6250 5050 6650 5050
Wire Wire Line
	6450 5050 6450 5200
Wire Wire Line
	6650 5050 6650 4900
Connection ~ 6450 5050
$Comp
L R R2
U 1 1 55301227
P 5250 3400
F 0 "R2" V 5330 3400 40  0000 C CNN
F 1 "4k7" V 5257 3401 40  0000 C CNN
F 2 "~" V 5180 3400 30  0000 C CNN
F 3 "~" H 5250 3400 30  0000 C CNN
	1    5250 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 3900 5650 3900
Wire Wire Line
	5250 3900 5250 3650
Connection ~ 5250 3900
$Comp
L GND #PWR03
U 1 1 55301259
P 5000 4700
F 0 "#PWR03" H 5000 4700 30  0001 C CNN
F 1 "GND" H 5000 4630 30  0001 C CNN
F 2 "" H 5000 4700 60  0000 C CNN
F 3 "" H 5000 4700 60  0000 C CNN
	1    5000 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 4400 5000 4400
Wire Wire Line
	5000 4350 5000 4700
$Comp
L GND #PWR04
U 1 1 5530126F
P 3200 4700
F 0 "#PWR04" H 3200 4700 30  0001 C CNN
F 1 "GND" H 3200 4630 30  0001 C CNN
F 2 "" H 3200 4700 60  0000 C CNN
F 3 "" H 3200 4700 60  0000 C CNN
	1    3200 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 4400 3200 4400
Wire Wire Line
	3200 4400 3200 4700
$Comp
L LM7818CT U1
U 1 1 55301289
P 1850 4750
F 0 "U1" H 1650 4950 40  0000 C CNN
F 1 "LM7818CT" H 1850 4950 40  0000 L CNN
F 2 "TO-220" H 1850 4850 30  0000 C CIN
F 3 "~" H 1850 4750 60  0000 C CNN
	1    1850 4750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 553012A0
P 1850 5200
F 0 "#PWR05" H 1850 5200 30  0001 C CNN
F 1 "GND" H 1850 5130 30  0001 C CNN
F 2 "" H 1850 5200 60  0000 C CNN
F 3 "" H 1850 5200 60  0000 C CNN
	1    1850 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 5000 1850 5200
$Comp
L VDD #PWR06
U 1 1 553012BC
P 2450 4600
F 0 "#PWR06" H 2450 4700 30  0001 C CNN
F 1 "VDD" H 2450 4710 30  0000 C CNN
F 2 "" H 2450 4600 60  0000 C CNN
F 3 "" H 2450 4600 60  0000 C CNN
	1    2450 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 4700 2450 4700
Wire Wire Line
	2450 4700 2450 4600
$Comp
L +24V #PWR07
U 1 1 553012F9
P 1200 4600
F 0 "#PWR07" H 1200 4550 20  0001 C CNN
F 1 "+24V" H 1200 4700 30  0000 C CNN
F 2 "" H 1200 4600 60  0000 C CNN
F 3 "" H 1200 4600 60  0000 C CNN
	1    1200 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 4700 1450 4700
$Comp
L +24V #PWR08
U 1 1 5530131D
P 1450 3200
F 0 "#PWR08" H 1450 3150 20  0001 C CNN
F 1 "+24V" H 1450 3300 30  0000 C CNN
F 2 "" H 1450 3200 60  0000 C CNN
F 3 "" H 1450 3200 60  0000 C CNN
	1    1450 3200
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 5530139B
P 2850 3900
F 0 "R1" V 2930 3900 40  0000 C CNN
F 1 "R" V 2857 3901 40  0000 C CNN
F 2 "~" V 2780 3900 30  0000 C CNN
F 3 "~" H 2850 3900 30  0000 C CNN
	1    2850 3900
	0    1    1    0   
$EndComp
Wire Wire Line
	3100 3900 3350 3900
$Comp
L VDD #PWR09
U 1 1 553013EB
P 5250 3050
F 0 "#PWR09" H 5250 3150 30  0001 C CNN
F 1 "VDD" H 5250 3160 30  0000 C CNN
F 2 "" H 5250 3050 60  0000 C CNN
F 3 "" H 5250 3050 60  0000 C CNN
	1    5250 3050
	1    0    0    -1  
$EndComp
$Comp
L VDD #PWR010
U 1 1 553013F1
P 6450 3050
F 0 "#PWR010" H 6450 3150 30  0001 C CNN
F 1 "VDD" H 6450 3160 30  0000 C CNN
F 2 "" H 6450 3050 60  0000 C CNN
F 3 "" H 6450 3050 60  0000 C CNN
	1    6450 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 3300 6250 3150
Wire Wire Line
	6250 3150 6650 3150
Wire Wire Line
	6650 3150 6650 3300
Wire Wire Line
	5250 3150 5250 3050
Wire Wire Line
	6450 3050 6450 3150
Connection ~ 6450 3150
Text Notes 2650 3700 0    60   ~ 0
an LED\nanpassen
$Comp
L C C3
U 1 1 553014F7
P 7450 3200
F 0 "C3" H 7450 3300 40  0000 L CNN
F 1 "10uF" H 7456 3115 40  0000 L CNN
F 2 "~" H 7488 3050 30  0000 C CNN
F 3 "~" H 7450 3200 60  0000 C CNN
	1    7450 3200
	1    0    0    -1  
$EndComp
$Comp
L VDD #PWR011
U 1 1 55301504
P 7450 2850
F 0 "#PWR011" H 7450 2950 30  0001 C CNN
F 1 "VDD" H 7450 2960 30  0000 C CNN
F 2 "" H 7450 2850 60  0000 C CNN
F 3 "" H 7450 2850 60  0000 C CNN
	1    7450 2850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR012
U 1 1 5530150A
P 7450 3600
F 0 "#PWR012" H 7450 3600 30  0001 C CNN
F 1 "GND" H 7450 3530 30  0001 C CNN
F 2 "" H 7450 3600 60  0000 C CNN
F 3 "" H 7450 3600 60  0000 C CNN
	1    7450 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7450 3000 7450 2850
Wire Wire Line
	7450 3400 7450 3600
$Comp
L C C4
U 1 1 5530155F
P 7700 3200
F 0 "C4" H 7700 3300 40  0000 L CNN
F 1 "10uF" H 7706 3115 40  0000 L CNN
F 2 "~" H 7738 3050 30  0000 C CNN
F 3 "~" H 7700 3200 60  0000 C CNN
	1    7700 3200
	1    0    0    -1  
$EndComp
$Comp
L VDD #PWR013
U 1 1 55301565
P 7700 2850
F 0 "#PWR013" H 7700 2950 30  0001 C CNN
F 1 "VDD" H 7700 2960 30  0000 C CNN
F 2 "" H 7700 2850 60  0000 C CNN
F 3 "" H 7700 2850 60  0000 C CNN
	1    7700 2850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR014
U 1 1 5530156B
P 7700 3600
F 0 "#PWR014" H 7700 3600 30  0001 C CNN
F 1 "GND" H 7700 3530 30  0001 C CNN
F 2 "" H 7700 3600 60  0000 C CNN
F 3 "" H 7700 3600 60  0000 C CNN
	1    7700 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 3000 7700 2850
Wire Wire Line
	7700 3400 7700 3600
$Comp
L DIODE D1
U 1 1 5530BE66
P 5000 4150
F 0 "D1" H 5000 4250 40  0000 C CNN
F 1 "DIODE" H 5000 4050 40  0000 C CNN
F 2 "~" H 5000 4150 60  0000 C CNN
F 3 "~" H 5000 4150 60  0000 C CNN
	1    5000 4150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5000 3900 5000 3950
Connection ~ 5000 3900
Connection ~ 5000 4400
$Comp
L R R3
U 1 1 5530BEDA
P 7950 4100
F 0 "R3" V 8030 4100 40  0000 C CNN
F 1 "0" V 7957 4101 40  0000 C CNN
F 2 "~" V 7880 4100 30  0000 C CNN
F 3 "~" H 7950 4100 30  0000 C CNN
	1    7950 4100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7700 4100 7450 4100
$Comp
L C C1
U 1 1 554C6DB1
P 1300 4950
F 0 "C1" H 1300 5050 40  0000 L CNN
F 1 "330nF" H 1306 4865 40  0000 L CNN
F 2 "~" H 1338 4800 30  0000 C CNN
F 3 "~" H 1300 4950 60  0000 C CNN
	1    1300 4950
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 554C6DB7
P 2350 4950
F 0 "C2" H 2350 5050 40  0000 L CNN
F 1 "100nF" H 2356 4865 40  0000 L CNN
F 2 "~" H 2388 4800 30  0000 C CNN
F 3 "~" H 2350 4950 60  0000 C CNN
	1    2350 4950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR015
U 1 1 554C6DBD
P 1300 5250
F 0 "#PWR015" H 1300 5250 30  0001 C CNN
F 1 "GND" H 1300 5180 30  0001 C CNN
F 2 "" H 1300 5250 60  0000 C CNN
F 3 "" H 1300 5250 60  0000 C CNN
	1    1300 5250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR016
U 1 1 554C6DC3
P 2350 5250
F 0 "#PWR016" H 2350 5250 30  0001 C CNN
F 1 "GND" H 2350 5180 30  0001 C CNN
F 2 "" H 2350 5250 60  0000 C CNN
F 3 "" H 2350 5250 60  0000 C CNN
	1    2350 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 4700 2350 4750
Connection ~ 2350 4700
Wire Wire Line
	2350 5150 2350 5250
Wire Wire Line
	1300 5250 1300 5150
Wire Wire Line
	1300 4750 1300 4700
Connection ~ 1300 4700
$Comp
L CONN_1 P1
U 1 1 554C6EDF
P 8550 3950
F 0 "P1" H 8630 3950 40  0000 L CNN
F 1 "Gate" H 8550 4005 30  0001 C CNN
F 2 "~" H 8550 3950 60  0000 C CNN
F 3 "~" H 8550 3950 60  0000 C CNN
	1    8550 3950
	-1   0    0    1   
$EndComp
$Comp
L CONN_1 P2
U 1 1 554C6EF6
P 8300 4500
F 0 "P2" H 8380 4500 40  0000 L CNN
F 1 "Emmitter" H 8300 4555 30  0001 C CNN
F 2 "~" H 8300 4500 60  0000 C CNN
F 3 "~" H 8300 4500 60  0000 C CNN
	1    8300 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 4000 2250 4000
Wire Wire Line
	2250 4000 2250 3900
Wire Wire Line
	2250 3900 2600 3900
$Comp
L LEG-12 U4
U 1 1 554CA0E3
P 9500 3950
F 0 "U4" H 9150 4250 60  0000 C CNN
F 1 "LEG-12" H 9500 3650 60  0000 C CNN
F 2 "~" H 9500 3950 60  0000 C CNN
F 3 "~" H 9500 3950 60  0000 C CNN
	1    9500 3950
	-1   0    0    1   
$EndComp
Wire Wire Line
	8550 4150 8850 4150
$Comp
L R R4
U 1 1 554CA20E
P 10350 3400
F 0 "R4" V 10430 3400 40  0000 C CNN
F 1 "400" V 10357 3401 40  0000 C CNN
F 2 "~" V 10280 3400 30  0000 C CNN
F 3 "~" H 10350 3400 30  0000 C CNN
	1    10350 3400
	-1   0    0    1   
$EndComp
Wire Wire Line
	10150 3750 10550 3750
Wire Wire Line
	10350 3750 10350 3650
$Comp
L GND #PWR017
U 1 1 554CA259
P 10350 4250
F 0 "#PWR017" H 10350 4250 30  0001 C CNN
F 1 "GND" H 10350 4180 30  0001 C CNN
F 2 "" H 10350 4250 60  0000 C CNN
F 3 "" H 10350 4250 60  0000 C CNN
	1    10350 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	10150 4150 10350 4150
Wire Wire Line
	10350 4150 10350 4250
Wire Wire Line
	8700 3950 8850 3950
Wire Wire Line
	8200 4100 8550 4100
Wire Wire Line
	8550 4100 8550 4150
$Comp
L GND #PWR018
U 1 1 554CAB07
P 8700 3800
F 0 "#PWR018" H 8700 3800 30  0001 C CNN
F 1 "GND" H 8700 3730 30  0001 C CNN
F 2 "" H 8700 3800 60  0000 C CNN
F 3 "" H 8700 3800 60  0000 C CNN
	1    8700 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8850 3750 8700 3750
Wire Wire Line
	8700 3750 8700 3800
$Comp
L R R5
U 1 1 554CE234
P 10550 3400
F 0 "R5" V 10630 3400 40  0000 C CNN
F 1 "400" V 10557 3401 40  0000 C CNN
F 2 "~" V 10480 3400 30  0000 C CNN
F 3 "~" H 10550 3400 30  0000 C CNN
	1    10550 3400
	-1   0    0    1   
$EndComp
Wire Wire Line
	10350 3050 10550 3050
Wire Wire Line
	10550 3050 10550 3150
Connection ~ 10350 3050
Wire Wire Line
	10550 3750 10550 3650
Connection ~ 10350 3750
$Comp
L CONN_2 P3
U 1 1 55544DE8
P 850 3400
F 0 "P3" V 800 3400 40  0000 C CNN
F 1 "notaus" V 900 3400 40  0000 C CNN
F 2 "~" H 850 3400 60  0000 C CNN
F 3 "~" H 850 3400 60  0000 C CNN
	1    850  3400
	-1   0    0    1   
$EndComp
Wire Wire Line
	1200 3300 1450 3300
Wire Wire Line
	1450 3300 1450 3200
Wire Wire Line
	1200 3500 1550 3500
$Comp
L CONN_2 P4
U 1 1 55544E99
P 850 3900
F 0 "P4" V 800 3900 40  0000 C CNN
F 1 "signal" V 900 3900 40  0000 C CNN
F 2 "~" H 850 3900 60  0000 C CNN
F 3 "~" H 850 3900 60  0000 C CNN
	1    850  3900
	-1   0    0    1   
$EndComp
Text Label 10500 2700 0    60   ~ 0
notaus
Wire Wire Line
	10500 2700 10350 2700
Wire Wire Line
	10350 2700 10350 3150
Text Label 1550 3800 2    60   ~ 0
notaus
Wire Wire Line
	1550 3800 1200 3800
$Comp
L +24V #PWR019
U 1 1 555456ED
P 1550 3450
F 0 "#PWR019" H 1550 3400 20  0001 C CNN
F 1 "+24V" H 1550 3550 30  0000 C CNN
F 2 "" H 1550 3450 60  0000 C CNN
F 3 "" H 1550 3450 60  0000 C CNN
	1    1550 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 3500 1550 3450
$Comp
L CONN_2 P5
U 1 1 55545B24
P 2500 3100
F 0 "P5" V 2450 3100 40  0000 C CNN
F 1 "gnd" V 2550 3100 40  0000 C CNN
F 2 "~" H 2500 3100 60  0000 C CNN
F 3 "~" H 2500 3100 60  0000 C CNN
	1    2500 3100
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR020
U 1 1 55545B2A
P 2950 3050
F 0 "#PWR020" H 2950 3050 30  0001 C CNN
F 1 "GND" H 2950 2980 30  0001 C CNN
F 2 "" H 2950 3050 60  0000 C CNN
F 3 "" H 2950 3050 60  0000 C CNN
	1    2950 3050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR021
U 1 1 55545B30
P 2950 3250
F 0 "#PWR021" H 2950 3250 30  0001 C CNN
F 1 "GND" H 2950 3180 30  0001 C CNN
F 2 "" H 2950 3250 60  0000 C CNN
F 3 "" H 2950 3250 60  0000 C CNN
	1    2950 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 3200 2950 3200
Wire Wire Line
	2950 3200 2950 3250
Wire Wire Line
	2950 3050 2950 3000
Wire Wire Line
	2950 3000 2850 3000
Connection ~ 1200 4700
Wire Wire Line
	1200 4600 1200 4700
$EndSCHEMATC
