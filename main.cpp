#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

constexpr int trajectoryCorrectionFactor = 5;
constexpr int traceLength = 10000;
constexpr float checkPointRadius = 600.f;
constexpr float podRadius = 400.f;

struct CheckPoint
{    
    explicit CheckPoint(int x, int y) : x(x) , y(y) {
        static int id = 0;
    }
    
    int x , y;
};

int CalculateThrust(int velocity, int nextCheckpointDist){
    if (nextCheckpointDist > (velocity * 2))
        return 100;
    else 
        return 10;  
}



enum PodType{
    PLAYER,
    ENEMY
};

struct Pod{
  
    explicit Pod(PodType type) : type(type) {
        static int nextId = 0;
        id = nextId++;
    }

    void ParseInput(){
        cin >> x >> y >> vx >> vy >> angle >> nextCheckpointId; cin.ignore();
    }

    //Traces a line in the velocity direction and checks if the next point intersects this line
    //Returns true on collsion false otherwise

    bool TraceLine(int targetX, int targetY, float traceLength, float targetRadius){
        float tx = static_cast<float>(x + vx * traceLength);
        float ty = static_cast<float>(y + vy * traceLength);
        float xp = static_cast<float>(x);
        float yp = static_cast<float>(y);
        float ncpx = static_cast<float>(targetX);
        float ncpy = static_cast<float>(targetY);    

        float distToLine = abs((tx - xp) * (yp - ncpy) - (xp - ncpx) * (ty - yp)) / sqrt((tx - xp) * (tx - xp) + (ty - yp) * (ty - yp));
        
        if (distToLine < targetRadius) {                
            cerr << id << "|" << "Hit" << endl;
            return true;
        }    
        else{
            cerr << id << "|" << "Miss" << endl;            
        }
        return false;
    }

    void Update(const vector<CheckPoint>& checkpoints, const Pod& e1, const Pod& e2){

        const CheckPoint* cp = &checkpoints[nextCheckpointId];

        int velocity = sqrt(vx * vx + vy * vy);
        int nextCheckpointDist = sqrt(pow(x - checkpoints[nextCheckpointId].x, 2) + pow(y - checkpoints[nextCheckpointId].y, 2));

        int thrust = CalculateThrust(velocity, nextCheckpointDist);   
        
        if (nextCheckpointDist < 2000){
            cerr << id << "|" << "Picking next checkpoint" << endl;
            cp = &checkpoints[(nextCheckpointId + 1) % checkpoints.size()];
        }
        if (TraceLine(e1.x, e1.x, velocity * 2, podRadius) || TraceLine(e2.x, e2.x, velocity * 2, podRadius)){
            //cout << cp->x << " " << cp->y << " SHIELD" << endl;
            cout << cp->x << " " << cp->y << " " << thrust << endl;
        }
        else{        

            if (!TraceLine(cp->x, cp->y, traceLength, checkPointRadius)){
                cerr << id << "|" << "Trajectory correction" << endl;
                cout << cp->x - (vx * trajectoryCorrectionFactor) << " " << cp->y - (vy * trajectoryCorrectionFactor) << " " << thrust << endl;
            }
            else
                cout << cp->x << " " << cp->y << " " << thrust << endl;
            
           
        }
        cerr << id << "|" << "NextCP distance: " << nextCheckpointDist << endl;        
    }

    PodType type;
    int x, y, vx, vy, angle, nextCheckpointId;  

    int lastCheckPointDist = 0; 
    int id;
};



int main()
{

    int laps, checkPointNum;

    cin >> laps; cin.ignore();
    cin >> checkPointNum; cin.ignore();

    vector<CheckPoint> checkpoints;

    for (int i = 0; i < checkPointNum; ++i)
    {
        int nextCpX, nextCpY;
        cin >> nextCpX >> nextCpY; cin.ignore();

        checkpoints.emplace_back(nextCpX, nextCpY);        
    }

    Pod p1(PodType::PLAYER), p2(PodType::PLAYER);
    Pod e1(PodType::ENEMY), e2(PodType::ENEMY);

    while(1){
        p1.ParseInput();
        p2.ParseInput();
        e1.ParseInput();
        e2.ParseInput();

        p1.Update(checkpoints, e1, e2);
        p2.Update(checkpoints, e1, e2);
    }
}
