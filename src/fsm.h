// orderButtonPressed er for knappene "utenfor" heisen. up == 0 impliserer at ned knappen har blitt trykket.

void orderButtonPressed(int up, int floor);


/*
destinationButtonPressed er for knappene "innenfor" heisen. Denne funksjonen er for øvrig identisk med orderButtonPressed med unntak av
at orderDestination kalles istedenfor order */

void destinationButtonPressed(int floor);


void floorSignal(int floor);
void stopButtonPressed(void);
void initialize(void);
void timeout(void);
int stoptime;
