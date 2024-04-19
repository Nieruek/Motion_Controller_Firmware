### RTOS README

#SETUP
setup FREERTOS in .ioc -> Utilities -> FREERTOS
create Tasks with prio, set them to dynamic allocation and default

#USE
Start your task out of freertos startTask-Routine -> call ur own while(1) or whatecter here
Benefit: return/break in ur task-Function with init in begginging -> freertos func restarts your task func after short delay
		-> you restart after initialising again. 
use OS_DELAY(...) instead default delay
default created main() -> while(1) wird in der Regel nicht erreicht und wird nicht verwendet