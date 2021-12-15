#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct CheckPoint
{    
    explicit CheckPoint(int x, int y) : x(x) , y(y) {
        static int id = 0;
        checkPointNum = id++;
    }
    
    int x , y;
    int checkPointNum = 0;
    CheckPoint* nextCheckPoint { nullptr };

};

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

constexpr int horizontalCellSize = 16000 / 32;
constexpr int verticalCellSize = 9000 / 32;

int main()
{

    // game loop

    CheckPoint* firstCheckPoint { nullptr };
    CheckPoint* lastCheckPointInserted { nullptr };
    bool firstLapCompleted = false;

    int xCells = 0, yCells = 0;

    while (1) {
        int x;
        int y;
        int next_checkpoint_x; // x position of the next check point
        int next_checkpoint_y; // y position of the next check point
        int next_checkpoint_dist; // distance to the next checkpoint
        int next_checkpoint_angle; // angle between your pod orientation and the direction of the next checkpoint
        cin >> x >> y >> next_checkpoint_x >> next_checkpoint_y >> next_checkpoint_dist >> next_checkpoint_angle; cin.ignore();
        int opponent_x;
        int opponent_y;
        cin >> opponent_x >> opponent_y; cin.ignore();
        
        if (firstCheckPoint != lastCheckPointInserted && !firstLapCompleted){
            firstLapCompleted = (firstCheckPoint->x == next_checkpoint_x && firstCheckPoint->y == next_checkpoint_y);
        }
        if (firstLapCompleted){
            cerr << "First lap completed!!\n";
        }
        else{
            cerr << "First lap\n";
            int xCellIndex = 1 << (next_checkpoint_x / horizontalCellSize);
            int yCellIndex = 1 << (next_checkpoint_y / verticalCellSize);
                                                
            bool isCheckPointRegistered = (xCells & xCellIndex) && (yCells & yCellIndex); //Check if already registered
            
            cerr << "isCheckPointRegistered " << isCheckPointRegistered << endl;
            if (!isCheckPointRegistered){ // Register a new checkPoint
                
                if (lastCheckPointInserted){
                    lastCheckPointInserted->nextCheckPoint = new CheckPoint(next_checkpoint_x, next_checkpoint_y);
                    lastCheckPointInserted = lastCheckPointInserted->nextCheckPoint;
                }
                else{
                    firstCheckPoint = new CheckPoint(next_checkpoint_x, next_checkpoint_y);
                    lastCheckPointInserted = firstCheckPoint;
                }

                xCells = xCells | xCellIndex;
                yCells = yCells | yCellIndex;
            }                       
                       
        }

        CheckPoint* c = firstCheckPoint;        
        while(c){
            cerr <<"CheckPoint: x-> " << c->x << " y-> " << c->y << endl;
            c = c->nextCheckPoint;
        }
        int absCheckpointAngle = abs(next_checkpoint_angle);

        int thrust;
        if (next_checkpoint_dist < 1500)
            thrust = 20;
        else 
            thrust = 100 - clamp(absCheckpointAngle, 0, 180);

        thrust = clamp(thrust, 20, 100);

        cerr << "Thrust: " << thrust << endl;
        cout << next_checkpoint_x << " " << next_checkpoint_y << " " << thrust << endl;
    }
     CheckPoint* c = firstCheckPoint;        
    
    while(c->nextCheckPoint){ //Clean
        CheckPoint* next = c->nextCheckPoint;
        delete c;
        c = next;
    }
}
