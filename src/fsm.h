/*
orderButtonPressed er for knappene "utenfor" heisen. up == 0 impliserer at ned knappen har blitt trykket.
*/
void fsm_orderButtonPressed(int up, int floor);
/*
destinationButtonPressed er for knappene "innenfor" heisen. Denne funksjonen er for øvrig identisk med orderButtonPressed med unntak av
at orderDestination kalles istedenfor order
*/
void fsm_destinationButtonPressed(int floor);
void fsm_floorSignal(int floor);
void fsm_stopButtonPressed(void);
void fsm_initialize(void);
void fsm_timeout(void);
void fsm_doorOpen(int floor);
