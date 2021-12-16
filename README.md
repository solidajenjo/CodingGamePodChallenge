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
Bronze league
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

I've spent like 8h here testing a lot of approaches until I found something to be happy with.
I've improved a lot the bot and now it performs way better than before. At this level I had a lot of problems with collisions. I've implemented a safety check on the last laps to figure out if a collision get the bot out of route or miss the checkpoint. As well implemented a trajectory prediction based on the bot velocity, I throw a ray in the velocity direction from the bot and if this ray doesnt collide with the checkopoint I apply a negative force to reorient the bot to the checkpoint.![Rank1 of bronze](https://user-images.githubusercontent.com/12461772/146347959-0da94f74-cbf6-4bf7-8e4b-a6f4541ebd33.PNG)

At this moment I'm the first of the bronze league but I think I should wait for the promotion a while.
