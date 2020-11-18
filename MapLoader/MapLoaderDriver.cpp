#include <iostream>
#include <stdexcept>
#include "MapLoader.h"
using namespace std;

int main() {
    try {
        MapLoader loader("C:\\Users\\flyin\\Downloads\\Europe\\europe.map");
        cout << "Map was successfully loaded." << endl;
    }
    catch (invalid_argument ex) {
        cout << ex.what() << endl;
    }
}
