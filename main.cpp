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

int main()
{

    // game loop

    CheckPoint* firstCheckPoint { nullptr };
    CheckPoint* lastCheckPointInserted { nullptr };
    bool firstLapCompleted = false;

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
            if (!firstCheckPoint){ //Create the checkpoint list head
                firstCheckPoint = new CheckPoint(next_checkpoint_x, next_checkpoint_y);
                lastCheckPointInserted = firstCheckPoint;
            }
            else{
                
                //Try to find the checkpoint on the list
                CheckPoint* c = firstCheckPoint;
                bool isCheckPointRegistered = false; // Avoid to introduce the first twice
                
                while(c->nextCheckPoint){   
                    /*                             
                    if ((*c->nextCheckPoint).x == (*firstCheckPoint).x && (*c->nextCheckPoint).y == (*firstCheckPoint).y){
                        firstLapCompleted = true;
                        c->nextCheckPoint->nextCheckPoint = firstCheckPoint; // Make the list circular to loop through
                    }
                    */                                           
                    if ((*c->nextCheckPoint).x == next_checkpoint_x && (*c->nextCheckPoint).y == next_checkpoint_y){
                        isCheckPointRegistered = true;
                    }                
                    c = c->nextCheckPoint;
                }            
                cerr << "isCheckPointRegistered " << isCheckPointRegistered << endl;
                if (!isCheckPointRegistered){ // Register a new checkPoint
                    lastCheckPointInserted->nextCheckPoint = new CheckPoint(next_checkpoint_x, next_checkpoint_y);
                    lastCheckPointInserted = lastCheckPointInserted->nextCheckPoint;
                }                       
            }            
        }

        CheckPoint* c = firstCheckPoint;        
        while(c->nextCheckPoint){
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
