#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

constexpr int horizontalCellSize = 16000 / 32;
constexpr int verticalCellSize = 9000 / 32;
constexpr int trajectoryCorrectionFactor = 5;
constexpr int traceLength = 10000;
constexpr float checkPointRadius = 600.f;

struct CheckPoint
{    
    explicit CheckPoint(int x, int y) : x(x) , y(y) {
        static int id = 0;
        checkPointNum = id++;
    }
    
    int x , y;
    int checkPointNum = 0;
    CheckPoint* nextCheckPoint { nullptr };
    bool isTheLast = false;
};

int CalculateThrust(int velocitySquared, int nextCheckpointDistSquared){
    if (nextCheckpointDistSquared > (velocitySquared * 8))
        return 100;
    //else if (velocitySquared < nextCheckpointDistSquared)
        //return 50;
    else 
        return 10;  
}

//Traces a line in the velocity direction and checks if the next point intersects this line
//If not returns true to correct the trajectory of the pod to cross the checkpoint

bool ShouldCorrectTrajectory(int x, int y, int deltaX, int deltaY, int next_checkpoint_x, int next_checkpoint_y){
    float tx = static_cast<float>(x + deltaX * traceLength);
    float ty = static_cast<float>(y + deltaY * traceLength);
    float xp = static_cast<float>(x);
    float yp = static_cast<float>(y);
    float ncpx = static_cast<float>(next_checkpoint_x);
    float ncpy = static_cast<float>(next_checkpoint_y);    

    float distToLine = abs((tx - xp) * (yp - ncpy) - (xp - ncpx) * (ty - yp)) / sqrt((tx - xp) * (tx - xp) + (ty - yp) * (ty - yp));
    cerr << "Dist to line: " << distToLine << endl;
    
    if (distToLine < checkPointRadius) {                
        cerr << "Hit" << endl;
    }    
    else{
        return true;
        cerr << "Miss" << endl;
    }
    return false;
}
/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/



int main()
{

    // game loop

    CheckPoint* firstCheckPoint { nullptr };
    CheckPoint* lastCheckPointInserted { nullptr };
    CheckPoint* currentCheckPointTarget { nullptr };
    CheckPoint* lastCheckPointTarget { nullptr };

    bool firstLapCompleted = false;

    int xCells = 0, yCells = 0;
    int lastDist = 0;
    int lastX = 0, lastY = 0;
    int velocitySquared = 0;
    bool boostUsed = false;

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
        
        int absCheckpointAngle = abs(next_checkpoint_angle);
        int nextCheckpointDistSquared = next_checkpoint_dist * next_checkpoint_dist;
        int deltaX = x - lastX;
        int deltaY = y - lastY;
        bool trajectoryCorrection = false;
        int thrust = 0;
        velocitySquared = (deltaX * deltaX) + (deltaY * deltaY);

        if (firstCheckPoint != lastCheckPointInserted && !firstLapCompleted){
            if ((firstLapCompleted = (firstCheckPoint->x == next_checkpoint_x && firstCheckPoint->y == next_checkpoint_y)))
            {
                lastCheckPointInserted->isTheLast = firstLapCompleted;
                currentCheckPointTarget = firstCheckPoint;
                lastCheckPointInserted->nextCheckPoint = firstCheckPoint;
                lastCheckPointTarget = lastCheckPointInserted;
            }
        }
        if (firstLapCompleted){
            cerr << "First lap completed!!\n";
            if (!boostUsed && next_checkpoint_dist > 5500 && absCheckpointAngle < 10)
            { 
                cout << next_checkpoint_x << " " << next_checkpoint_y << " BOOST" << endl;
                boostUsed = true;                
            }
            else{            

                thrust = CalculateThrust(velocitySquared, nextCheckpointDistSquared);
                
                if (next_checkpoint_dist < 2000 && lastCheckPointTarget->x != next_checkpoint_x && lastCheckPointTarget->y != next_checkpoint_y)
                {        
                    cerr << "Close to checkpoint" << endl;      
                    lastCheckPointTarget = currentCheckPointTarget; 
                    currentCheckPointTarget = currentCheckPointTarget->nextCheckPoint; 
                }
                
                if (next_checkpoint_dist > lastDist){ // Possible collision or out of route detected try to figure out the actual next checkpoint
                    cerr << "Rotue correction" << endl;
                    CheckPoint* c = firstCheckPoint;     
                    bool found = false;                       
                    while(c->nextCheckPoint && !found){ 
                        if (c->x == next_checkpoint_x && c->y == next_checkpoint_y){
                            currentCheckPointTarget = c;
                            found = true;
                        }
                        c = c->nextCheckPoint;
                    }                    
                }
                
                trajectoryCorrection = ShouldCorrectTrajectory(x, y, deltaX, deltaY, next_checkpoint_x, next_checkpoint_y);
                cerr << "CheckPoint: x-> " << currentCheckPointTarget->x << " y-> " << currentCheckPointTarget->y << endl;
                
                //If needs to correct the trajectory it moves the target point in the inverse direction of the velocity to compensate
                if (trajectoryCorrection)
                    cout << currentCheckPointTarget->x - (deltaX * trajectoryCorrectionFactor) << " " << currentCheckPointTarget->y - (deltaY * trajectoryCorrectionFactor) << " " << thrust << endl;
                else
                    cout << currentCheckPointTarget->x << " " << currentCheckPointTarget->y << " " << thrust << endl;
            
            }
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
                //Mark the checkpoint as registered
                xCells = xCells | xCellIndex;
                yCells = yCells | yCellIndex;
            }                       
                     
            CheckPoint* c = firstCheckPoint;        
            while(c){
                cerr <<"CheckPoint: x-> " << c->x << " y-> " << c->y << endl;
                c = c->nextCheckPoint;
            }
            
            thrust = CalculateThrust(velocitySquared, nextCheckpointDistSquared);   
            
            trajectoryCorrection = ShouldCorrectTrajectory(x, y, deltaX, deltaY, next_checkpoint_x, next_checkpoint_y);
            if (trajectoryCorrection)
                cout << next_checkpoint_x - (deltaX * trajectoryCorrectionFactor) << " " << next_checkpoint_y - (deltaY * trajectoryCorrectionFactor) << " " << thrust << endl;
            else
                cout << next_checkpoint_x << " " << next_checkpoint_y << " " << thrust << endl;

        }
        
        cerr << "Thrust: " << thrust << endl;
        cerr << "Next checkpoint dist: " << next_checkpoint_dist << endl;
        cerr << "Velocity sq: " << velocitySquared << endl;
        cerr << "Dist sq: " << nextCheckpointDistSquared << endl;
        cerr << "Abs Angle:" << absCheckpointAngle << endl;
        lastDist = next_checkpoint_dist;
        lastX = x;
        lastY = y;
    }
    CheckPoint* c = firstCheckPoint;        
    
    while(c && c->nextCheckPoint){ //Clean
        CheckPoint* next = c->isTheLast ? nullptr : c->nextCheckPoint;
        delete c;
        c = next;
    }
}
