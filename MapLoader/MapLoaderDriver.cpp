#include <iostream>
#include <stdexcept>
#include "MapLoader.h"
using namespace std;

int main() {
    try {
        MapLoader loader("./MapLoader/maps/TestMap.txt");
        cout << "Map was successfully loaded." << endl;
    }
    catch (invalid_argument ex) {
        cout << ex.what() << endl;
    }
}
