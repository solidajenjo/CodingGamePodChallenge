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
I've improved a lot the bot and now it performs way better than before. At this level I had a lot of problems with collisions. I've implemented a safety check on the last laps to figure out if a collision get the bot out of route or miss the checkpoint. As well implemented a trajectory prediction based on the bot velocity, I throw a ray in the velocity direction from the bot and if this ray doesnt collide with the checkopoint I apply a negative force to reorient the bot to the checkpoint.

![Rank1 of bronze](https://user-images.githubusercontent.com/12461772/146348573-0f6caaa8-4b88-4bcd-8360-826e9237a6b0.PNG)


At this moment I'm the first of the bronze league but I think I should wait for the promotion a while.

Silver league
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
I've just been promoted to silver!! I tested my previous bot on this league and got the first place without changing the code.

![Rank1 of silver](https://user-images.githubusercontent.com/12461772/146356903-f3d3fa3f-75bb-4d8a-adaf-5bccd78e3de0.PNG)

I'm gonna wait for the promotion to the next league before starting to add the shield new feature.

Gold league
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Time spent here 6h.

In this league I had a lot of problems with the collisions. I tried to make use of the shield the best I could but sometimes my own pods were colliding and causing out of route 
issues. I've been trying to balance between be aggressive or conservative but the results were different depending on the game board.
In this last version, however, I think I've made a quite good bots and finally got to 1233 position.

This last version is a improvement of the other versions but in this league I had all the information at the begining of the race and then I could start to advance turns at the firs lap. I tried to approach better to the checkpoints aiming at a point displaced based on the next checkpoint instead aiming to the center of the current checkpoint. In this last version as well I implemented a wolf/hare group behavior. If one of the pods is far beyond the others it became a wolf and stops going to the checkpoints to try to hit the best of the enemy pods to stop it. The other is the Hare and tries to win the race. At the begining both pods are hares.

Further improvements
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
It will be cool to explore the genetic algorithm approach but I didn't find out how to do it, for that it will be necessary to get access to the game code and run a lot of races  and store the results to breed better pods by mutation, cross and random generation.
Another cool thing can be trace the optimal route using splines but I've discarded this because in the web it can be hard to debug, I'll need to draw a lot of information on the screen to check if its working as expected.
