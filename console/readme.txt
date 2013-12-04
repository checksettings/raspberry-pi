Anleitung für die Nutzung der Shell.

Damit die Shell startet und richtig initialisiert wird müssen folgende zwei Funktionen aufgerufen werden:
uartInit();
shell();

Fünf verschiedene Befehle sind in der Shell fix hardcoded implementiert:
exit,shutdown,restart,load,help.
Was diese Befehle machen kann man  nachlesen wenn man die Shell startet und "help" eingibt.
Der Befehl shutdown funktioniert nur wenn auch unser Raspbootcom verwendet wird da wir dort kleine Änderungen vorgenommen
haben da wir so vom Raspberry aus Raspbootcom beenden können.

Der interessanteset Befehl ist der load-Befehl. Wenn das Programm einmal auf dem Raspberry ausgeführt wird kann man am
PC durch eine zweite Konsole das kernel.img neu erstellen und mit dem Befehle (load) neu auf das Raspberry kopieren und 
das Programm startet neu.

Um die Shell möglichst dynamisch zu halten kann man über die Funktion
void addNewCommand(fcn_ptr function_pointer, char command_name[24], char* help_text)
neue Befehle hinzufügen. Der erste Parameter ist der Pointer auf die Funktion welche beim Aufrufen des Befehls ausgeführt werden soll.
Der zwei Parameter ist der Name des Befehls. Der dritte Parameter ist ein char array im welchen der Text steht welche beim aufrufen der Funktion
help angezeigt werden soll. Falls der User keinen Hilfetext anzeigen möchte dann kann muss dierser Pointer auf NULL zeigen.
Ein Beispiel dieser Funktion kann man in der shell.c Zeile 21 sehen.
