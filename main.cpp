#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

constexpr int trajectoryCorrectionFactor = 5;
constexpr int traceLength = 10000;
constexpr float checkPointRadius = 600.f;
constexpr float podCollisionRadius = 1000.f;
constexpr float halfScreenWidth = 8000.f;
constexpr float halfScreenHeight = 4500.f;
constexpr int shieldCoolDown = 5;

struct CheckPoint
{    
    explicit CheckPoint(int x, int y) : x(x) , y(y) {
        static int id = 0;
    }
    
    int x , y;
};

struct Vec{

    Vec() : x(0), y(0)  {}

    Vec(float x, float y) : x(x), y(y){
        length = sqrt(pow(x, 2) + pow(y, 2));
    }

    void Normalize(){
        x /= length;
        y /= length;
    }
    float x, y;
    float length;    
};


enum PodType{
    PLAYER,
    ENEMY
};

struct Pod{
  
    explicit Pod(PodType type) : type(type) {
        static int nextId = 0;
        id = nextId++;
        lastCheckPointId = 0;
        checkPointCount = 0;
    }

    void ParseInput(){
        cin >> x >> y >> vx >> vy >> angle >> nextCheckpointId; cin.ignore();
        if (nextCheckpointId != lastCheckPointId){        
            lastCheckPointId = nextCheckpointId;
            ++checkPointCount;
        }
        cerr << id << "|" << "CP Count: " << checkPointCount << endl;
    }

    void CalculateThrust(){        
        thrust = (nextCheckpointDist < (velocity.length  * 8) || angleToCheckpoint > M_PI_2) ? 50 : 100;
    } 

    //Traces a line in the velocity direction and checks if the next point intersects this line
    //Returns true on collsion false otherwise

    bool TraceLine(int targetX, int targetY, float traceLength, float targetRadius) const {
        float tx = static_cast<float>(x + vx * traceLength);
        float ty = static_cast<float>(y + vy * traceLength);
        float xp = static_cast<float>(x);
        float yp = static_cast<float>(y);
        float ncpx = static_cast<float>(targetX);
        float ncpy = static_cast<float>(targetY);    

        float distToLine = abs((tx - xp) * (yp - ncpy) - (xp - ncpx) * (ty - yp)) / sqrt((tx - xp) * (tx - xp) + (ty - yp) * (ty - yp));
        //cerr << id << "|" << " Trace line lenght: " << traceLength << endl;
        //cerr << id << "|" << " Trace line from(" << x << "," << y << ") to (" << targetX << "," << targetY << ")" << endl;
        if (distToLine < targetRadius) {                
            cerr << id << "|" << "Hit" << endl;
            return true;
        }    
        else{
            cerr << id << "|" << "Miss" << endl;            
        }
        return false;
    }


    void UpdatePodPhysics(const vector<CheckPoint>& checkpoints){
        cerr << id << "|" << "Update pod physics" << endl;  
        const CheckPoint* cp = &checkpoints[nextCheckpointId];        
        velocity = Vec(static_cast<float>(vx), static_cast<float>(vy));
        Vec nextCheckpointVec = Vec(cp->x - x, cp->y - y);
        nextCheckpointVec.Normalize();
        float theta = angle * M_PI / 180.f;
        float dx = cos(theta);
        float dy = sin(theta);
        Vec directionNormalized = Vec(dx, dy);
        directionNormalized.Normalize();
        nextCheckpointDist = nextCheckpointVec.length;      
        cerr << id << "|" << "Direction norm: " << directionNormalized.x << " - " << directionNormalized.y << endl;

        //Velocity and nextcheckpointVec are normalized so don't need to divide
        angleToCheckpoint = acos(directionNormalized.x * nextCheckpointVec.x + directionNormalized.y * nextCheckpointVec.y);//In radians!!
    }
  

    void Update(const vector<CheckPoint>& checkpoints, const Pod& palPod, const Pod& e1, const Pod& e2){

        const CheckPoint* cp = &checkpoints[nextCheckpointId];
        //If is the last pod and his pal is not a wolf  turns into wolf to try to disturb the enemy pods
        isWolf = palPod.nextCheckpointId > nextCheckpointId && e1.nextCheckpointId > nextCheckpointId && e2.nextCheckpointId > nextCheckpointId;
        if (isWolf) //Is a wolf
        {    
            cerr << id << "|" << " is a Wolf" << endl;        
            float theta = angle * M_PI / 180.f;
            float dx = cos(theta);
            float dy = sin(theta);
            Vec directionNormalized = Vec(dx, dy);
            directionNormalized.Normalize();
            Vec targetVec;
            Vec targetNextLoc;
            if (e1.checkPointCount > e2.checkPointCount){
                targetVec = Vec(x - e1.x, y - e1.y);
                targetNextLoc = Vec(e1.x + e1.vx * 8, e1.y + e1.vy * 8);
            }
            else if (e1.checkPointCount < e2.checkPointCount){
                targetVec = Vec(x - e2.x, y - e2.y);
                targetNextLoc = Vec(e2.x + e2.vx * 8, e2.y + e2.vy * 8);
            }
            else if (e1.nextCheckpointDist < e2.nextCheckpointDist){
                targetVec = Vec(x - e1.x, y - e1.y);
                targetNextLoc = Vec(e1.x + e1.vx * 8, e1.y + e1.vy * 8);
            }
            else{
                targetVec = Vec(x - e2.x, y - e2.y);
                targetNextLoc = Vec(e2.x + e2.vx * 8, e2.y + e2.vy * 8);
            }
            targetVec.Normalize();
            float angleToTarget = acos(directionNormalized.x * targetVec.x + directionNormalized.y * targetVec.y);//In radians!!

            cerr << id << "|" << " Target distance: " << targetVec.length;
            
            if (targetVec.length > 400)
                cout << targetNextLoc.x << " " << targetNextLoc.y << " " << (angleToTarget < M_PI_2 ? 80 : 100) << endl;
            else
                cout << targetNextLoc.x << " " << targetNextLoc.y << " SHIELD" << endl;
            return;
        }
        
        //Is a hare
        if (coolDown > 0)
            --coolDown;
        
        UpdatePodPhysics(checkpoints);

        cerr << id << "|" << "Next checkpoint angle: " << angleToCheckpoint << endl; 

        CalculateThrust();  
        
        if (nextCheckpointDist < velocity.length * 4){
            cerr << id << "|" << "Picking next checkpoint" << endl;
            cp = &checkpoints[(nextCheckpointId + 1) % checkpoints.size()];
        }
        cerr << id << "|" << " e1 length: " << Vec(e1.x - x, e1.y - y).length << " e2 length: " << Vec(e2.x - x, e2.y - y).length << " cd: " << coolDown << endl;
        if (!coolDown && (Vec(e1.x - x, e1.y - y).length < podCollisionRadius || Vec(e2.x - x, e2.y - y).length < podCollisionRadius)){
            cerr << id << "|" << "Potential collision detected" << endl;
            cout << cp->x << " " << cp->y << " SHIELD" << endl;            
            coolDown = shieldCoolDown; // Use a cooldown to avoid be with the shield on for a long time
        }
        else{
            if (!boostUsed && nextCheckpointDist > 2000.f && angleToCheckpoint < 0.2f)
            { 
                cout << cp->x << " " << cp->y << " BOOST" << endl;
                boostUsed = true;
            }
            else{        

                if (!TraceLine(cp->x, cp->y, traceLength, checkPointRadius)){
                    cerr << id << "|" << "Trajectory correction" << endl;
                    cout << cp->x - (vx * trajectoryCorrectionFactor) << " " << cp->y - (vy * trajectoryCorrectionFactor) << " " << thrust << endl;
                }
                else{ //Aim to the optimal point of the checkpoint according its next checkpoint
                    cerr << id << "|" << "Trajectory anticipation" << endl;                
                    const CheckPoint* nextCheckpoint = &checkpoints[(nextCheckpointId + 1) % checkpoints.size()];
                    int xC = cp->x > nextCheckpoint->x ? -300 : 300;
                    int yC = cp->y > nextCheckpoint->y ? -300 : 300;
                    cout << cp->x + xC << " " << cp->y + yC << " " << thrust << endl;
                }                            
            }
        }
        cerr << id << "|" << "NextCP distance: " << nextCheckpointDist << endl;        
        cerr << id << "|" << "Velocity: " << velocity.x << " - " << velocity.y << endl; 
        cerr << id << "|" << "Angle:" << angle << endl;
    }

    PodType type;
    int x, y, vx, vy, angle, nextCheckpointId;  
    int lastCheckPointId;
    int checkPointCount;

    int lastCheckPointDist = 0; 
    int id;
    int thrust;
    Vec velocity;
    float nextCheckpointDist;
    int coolDown = 20;
    bool boostUsed = false;
    bool isWolf = false;
    float angleToCheckpoint;
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
    
        e1.UpdatePodPhysics(checkpoints);
        e2.UpdatePodPhysics(checkpoints);
        p1.Update(checkpoints, p2, e1, e2);
        p2.Update(checkpoints, p1, e1, e2);
    }
}


