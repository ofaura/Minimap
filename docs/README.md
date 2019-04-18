“I am [Òscar Faura](https://www.linkedin.com/in/ofaura21/ "LinkedIn"), student of the [Bachelor’s Degree in Video Games by UPC at CITM](https://www.citm.upc.edu/ing/estudis/graus-videojocs/ "Link to the Degree"). This content is generated for the second year’s subject Project 2, under supervision of lecturer [Ricard Pillosu](https://es.linkedin.com/in/ricardpillosu "LinkedIn").”

# What is a Minimap?
A minimap is an element of the HUD usually placed at a screen corner that represents a map, but its size is much smaller. The function of a minimap is to help the player get orineted.

The common elements usually included in minimaps are the position of the player, the allies and enemies, buildings, terrain, and points of interests. Elements that are not visible for the player, should not appear in the minimap.   

![Minimap](Images/minimaps.png "Minimaps")


# Where should it be located?
Depending on the videogame genre, there are some conventions on where to locate the minimap. But there are always exceptions.


* **MOBA games** place the minimap at the bottom.
![MOBA](Images/moba.png "LoL - Heroes of the Storm - Dota 2")


* **Strategy games** place the minimap at the bottom left.
![Strategy games](Images/strategy.png "civ VI - Starcraft 2 - WarCraft 3")


* **MMORPG's** place the minimap at the right and mostly at the top.
![MMORPG's games](Images/mmorpgs.png "World of Warcraft - Guild Wars 2 - Final Fantasy XIV")


* **Racing games** place the minimap at the left.
![Racing games](Images/racing.png "F1 2018 - Forza Horizon 4 - Need for Speed Rivals")


* **FPS** mainly place the minimap at the left.
![FPS games](Images/fps.png "Battlefield V - CS: GO - Black Ops IV")


# Old vs Modern Minimaps. Pros and Cons
The old minimaps where more effective, they show you where the player is so he can get to acknowledge of surrounding to get somewhere new. Minimaps like this are simple but they work, they do not tell you where do you have to go, so they do not make feel things linear and encourages the player to explore.
With modern games, the minimap shows where every enemy is and also provides the layout of an area without letting you explore it first. This removes the element of surprise when the enemies attack you and also a part of discovery. You can basically play the game looking to the minimap, and that is the major problem in games where exploartion is rewarded or encouraged. The player spends a lot of the time looking to the minimap and not to the whole screen.
 ![Old vs Modern Minimaps](Images/old_modern.png "Assassin's Creed IV: Black Flag - Legend of Zelda")


# Does my game need a Minimap?
It depends on the kind of game you are developing and the use you will give to the minimap. You have to take to account if the minimap conflicts with some of your game pillars, and if it does, evaluate how much does it affect and try to find possible solutions. For instance, if one of your games pillars is to fight all the enemies, do not put on the minimap the area where they are looking, because it gives the possibility to the player to avoid fighting. Only the necessary features should be included.
As a general rule, a minimap should be used:
* When the level or world is large.
* When there are multiple ways to reach the same destination.
* When there are lots of different points of interest.
* If you are doing an RTS, it is a must.

You can always give the possibility to the player to remove or not the minimap if it does not affect to the core of the game.


# Implementation
To create a minimap we will follow a series of TODO's to know the steps to make a basic minimap module. Our goal is to blit to a screen corner the minimap, which it will be the whole map and it will show the position of the player and the enemies, a rectangle of the part you are seeing, and the possibility to move the camera through the minimap. 
 ![TODO's Goal](Images/todo_goal.gif "Goal")

