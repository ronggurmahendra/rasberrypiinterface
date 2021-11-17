#include "../lib/vimana.h"


int main(){
    //initialize
    Vimana myVimana = Vimana();
    
    myVimana.commandGoTo(-69813748, 1075702605,500); //give order to vimana to go to -69813748, 1075702605(shelter lanud) with altitude of 50 m
    while(myVimana.distanceToCoor( -69813748, 1075702605 ) < 10){ // when vimana within 10 m of coordinate
        myVimana.commandLoiter(-69813748, 1075702605,500);//give order to vimana to loiter at -69813748, 1075702605(shelter lanud) with altitude of 50 m
    }
}