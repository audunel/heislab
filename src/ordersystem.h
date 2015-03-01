void order(int up, int floor); // For bestillinger "utenfor" heisen. up == 0 impliserer ned.
int getOrder(int up, int floor);
void removeOrder(int floor);
void clearAll();
void orderDestination(int floor); // For bestillinger "innenfor" heisen.
int getDestination(int floor);
void receiveOrder();
