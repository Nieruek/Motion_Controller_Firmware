###CANOpen import to STM32CubeIDE

*github.com/CANopenNode/CANOpenSTM32
*clone git repo to project folder
*refresh project -> shows CANOpenSTM32 Folder

###Setup CANOpen

*see github.com/CANopenNode/CANOpenSTM32

###Usage with FreeRTOS

*create Task for CANOpen
*when accessing OD, CAN Send and CMCy variables use lock to prevent race
*CANFD konfigurieren: 
	CANFD aktivieren
	Interrupt aktivieren
	Timing anpassen, entsprechend clock: https://www.kvaser.com/support/calculators/can-fd-bit-timing-calculator/
	-> Nominal+Bit-TIming
	Timer der für CANFD verwendet wird kann entsprechend Clock-Freq mit Prescaler so angepasst werden, dass er nur alle 1 ms triggert
	-> Verarbeitung erfolgt nicht zu oft.
	

#CONFIG
*In der freertos CAN-Fctn, NICHT im loop:
CANopenNodeSTM32 canOpenNodeSTM32;
canOpenNodeSTM32.CANHandle = &hfdcan2;
canOpenNodeSTM32.HWInitFunction = MX_FDCAN2_Init;
canOpenNodeSTM32.timerHandle = &htim17;
canOpenNodeSTM32.desiredNodeID = 21;
canOpenNodeSTM32.baudrate = 500;
canopen_app_init(&canOpenNodeSTM32);

#Im LOOP danach:
*StatusLEDS setzen, an externen GPIOS oder mit der internen
HAL_GPIO_WritePin(extern_LED_2_GPIO_Port, extern_LED_2_Pin,
		canOpenNodeSTM32.outStatusLEDGreen == 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
HAL_GPIO_WritePin(extern_LED_3_GPIO_Port, extern_LED_3_Pin,
		canOpenNodeSTM32.outStatusLEDRed == 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
canopen_app_process();
vTaskDelay(pdMS_TO_TICKS(1)); // sleep for 1 ms

#Im Timer INTERRUPT: 
*Prüfen ob unser vorher zugeordneter Timer getriggert wurde -> canopen-int-Fctn aufrufen
 if (htim == canopenNodeSTM32->timerHandle) {
	  canopen_app_interrupt();
  }


###CANopenNode Editor
Getestet und verwendet unter Linux
https://github.com/CANopenNode/CANopenEditor/   mit visual code oeffnen und starten oder download build 
run unter Linux: mono (or wine) <EDSEditor.exe 
#Verwendung
OPEN: .XPD File im Projekt unter CANopenNode_STM32/ 
ADJUST: anpassen/ändern was man haben will und SAVE
UPDATE_OD: export CANopenNode -> im Projekt unter CANopenNode_STM32/ als OD.h speichern und das alte ersetzen. 
			der Rest wird automatisch generiert etc.
			
#Create/Mod PDO/SDO:
OD-Reiter -> Device Profile Specific... -> click -> add (name etc.)
-> Permissions: Access SDO/PDO/... t = tranmit, r = rec, no=no ;)

TX PDO Mapping-Reiter -> Add  new PDO -> unseren PDO einfügen -> Zyklus(Event-Timer) + valid! konfigurieren 
-> double check alles weil das speichern manchmal probleme macht

UPDATE_OD (siehe oben)

Build and run -> sollte laufen 
