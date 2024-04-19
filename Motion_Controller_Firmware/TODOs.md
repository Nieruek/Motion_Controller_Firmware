###TODOS for Project: 

-Move trajektoriengenerator  in separate class, 
not needed only in DCMotor -> same for all, verschiedene Methoden etc., je nach param/init andere Funktion/Funktionen nutzen 
wie verpackt mans/ kommts mit in die achse rein

CANOpennode 
-Er sagt uns bis zum nächsten meeting was/welche ids über canopen kommen -> könnten wir dann schonmal als default/Besipiel für die spätere Verwendung einbauen
-DIe Sache mit den LEDs und dem Oszi-Test in den Bericht übernehmen.

ADC aiuslesen nur wenn unterer durchgeschalten ist -> vom PWM-Zyklus abhänging machen, nicht stump mitm interrupt

Stromregelung: 
PID nehmen und D-Anteil auf 0 setzen -> easy going wenn der PID steht
wenns dann ernst wird lieber eigenen Regler nehmen statt die Fallunterscheidung auf die Dämpfung machen: kostet RAM aber bring Rechenzeitvorteil
alle 10ys nur wenn wirs schaffen, zur not auch langsamer wenn ers nicht schafft
Stromregelung macht das ganze dynamisch. 

50% der Zeit in Echtzeitanwedung sein -> rest für die nicht kritischen sachen -> genug Puffer + kein doppelter Interruppt, NOGO

Am Anfang alles mit 0 initialisieren.

Fürs holen vom ADC wert eine eigene Klasse, Den bitwert umrechen etc. Auch Differenz im ADC intern verrechnen vll.

Klassenaufbau sein Vorschlag: 
Trajektorie: von PC, Canopen, S-Curve -> bidirektional. 		
Motor Lage+Strom zusammen, ein system zusammen
PWM 
Matrix Klasse, drüber nachdenken wie wir das machen! ADC Matrix mit Output Matrix koppeln etsprechend was/wie geschalten wird
evtl Datenrekorder, der zeugs aufnimmt + in jeder Klasse

Achse wäre dann Trij + Motor + PWM

Input/OutputMatrix direkt am Output Motor/Input Sesnor: 
	Sesnorumschaltung evtl über die InputMatrix von den Sesnoren