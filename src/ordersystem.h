void ordersystem_order(int up, int floor); // For bestillinger "utenfor" heisen. up == 0 impliserer ned.
int ordersystem_getOrder(int up, int floor);
void ordersystem_removeOrder(int floor);
void ordersystem_clearAll();
void ordersystem_orderDestination(int floor); // For bestillinger "innenfor" heisen.
int ordersystem_getDestination(int floor);
