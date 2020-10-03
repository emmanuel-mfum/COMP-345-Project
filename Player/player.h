#include <vector>

class Player{
    private:
        int armies;
        
       
    
        
    public:
        Player();
        int getArmies(){return armies;};
        int setArmies(int sA){armies=sA;};
        void toDefend(){}
        void toAttack(Player *pl){}
        void issueOrder(){}
        

};
