# CodingGamePodChallenge

Wood 3
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
The first level took me 5 minutes. Pretty straightforward, only needed to increase the thrust to win.

Wood 2
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

This level took me about 10 minutes. I tried to go slow when next to the checkpoint or the absolute value of the angle was more than 30 deg.

Wood 1
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
In this level I've spent like 2 hours aprox. I tried some different approaches and strategies. Finally I decided to learn the route on the first lap, due the lack of information provided by the game, and use it on the next laps to start turning earlier. I've beat the level without even using the boost.
To learn the route I've created a Checkpoint linked list and once I had all the checkpoints I've closed the list to make it circular and iterate through. To avoid insert the same checkpoint more than one time, I divided the map on cells. The biggest cell size was 500 (16000/32) and this way I could use 2 32 bit ints to store the already inserted checkpoints, the max cell size was smaller than the checkpoint radiurs and there wasn't possibility of collision. The combination of xCellIndex and yCellIndex was unique for every checkpoint and that assured the correctness of the created route.
Once I learnt the route I started using it, when the pod is close to the checkpoint I stoped thrusting and used the inertia to arrive to it while I started turning to face the next checkpoint in the list.
For the next iterations I want to improve the first lap code, I'm still using the Wood 2 simple approach, and make a good use of the boost if the circunstances to use it are good enough
