#include "Map.h"

using namespace std;

int main() {
    // -------------------------------------------------------------------------------------------
    //                                         MAP
    // -------------------------------------------------------------------------------------------

    int width, height;
    cout << "Enter map width: ";
    cin >> width;
    cout << "Enter map height: ";
    cin >> height;

    Map gameMap(width, height);
    gameMap.displayMap();

    char row;
    int col;
    cout << "Set path using coordinates (enter Z -1 to stop):\n";
    while (true) {
        cin >> row >> col;
        if (row == 'Z' && col == -1) break;
        gameMap.setCellToPath(row - 'A', col);
        gameMap.displayMap();
    }
    cout << "Set entry point (Row(A-Z) Column(0-9)): ";
    cin >> row >> col;
    gameMap.setEntryPoint(row - 'A', col);
    gameMap.displayMap();

    cout << "Set exit point (Row(A-Z) Column(0-9)): ";
    cin >> row >> col;
    gameMap.setExitPoint(row - 'A', col);
    gameMap.displayMap();

    if (gameMap.validateMap()) {
        cout << "Map is valid!" << endl;
        gameMap.displayEntityPath();
    } else {
        cout << "Map is invalid!" << endl;
    }
// -------------------------------------------------------------------------------------------







    return 0;
}
