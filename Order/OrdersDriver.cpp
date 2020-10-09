#include "Orders.h"

int main() {
    //list of Order
    OrdersList orders;
    
    std::cout << "Creating Orders and adding them to the list of Orders:" << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;
   
    Deploy deploy=Deploy("deploy");
    orders.addOrder(deploy);
    Advance advance=Advance("advance");
    orders.addOrder(advance);
    Bomb bomb= Bomb("bomb");
    orders.addOrder(bomb);
    Blockade blockade= Blockade("blockade");
    orders.addOrder(blockade);
    Airlift airlift=Airlift("airlift");
    orders.addOrder(airlift);
    Negotiate negotiate=Negotiate("negotiate");
    orders.addOrder(negotiate);
    
    std::cout << "\nOrderList contains " << orders.getsize() << " objects." << std::endl;
    
    std::cout << "\nRemoving Orders from the list:" << std::endl;
    std::cout << "------------------------------" << std::endl;
    orders.Delete(0);
    
     std::cout << "\nOrderList contains "<<orders.getsize() << " objects." << std::endl;
    
    std::cout << "\nMoving Orders from the list:" << std::endl;
    std::cout << "----------------------------" << std::endl;
    orders.move(1, 0);
    
    std::cout << "\nExecuting and Validating Orders:" << std::endl;
    std::cout << "--------------------------------" << std::endl;
       
    orders.executeOrders();
    
}
