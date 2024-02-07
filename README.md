# 🦆 QuackBlast
QuackBlast is a thrilling 2D single-player action game that draws from
the classic Bomberman concept while adding a delightful duck theme and
strategic complexity. Players assume the role of a charismatic duck
wielding a powerful arsenal of explosive eggs, maneuvering through
challenging labyrinths filled with enemies.

<img src="https://github.com/fini03/QuackBlast/blob/main/game.png">

### 👾 Gameplay Mechanics

QuackBlast features gameplay mechanics similar to those found in the
classic game Bomberman. Players guide their duck character through
maze-like levels, utilizing explosive eggs to destroy barriers and
outwit opponents. If an enemy is hit by the bomb, they will perish,
and if the duck collides with an opponent, the player loses. The ultimate
goal is to reach the exit of each level.

### 🎮 Movement

| Key                | Description                                                          |
|--------------------|--------------------------------------------------------------------- |
| <kbd>W</kbd>       | Move the player forward                                              |
| <kbd>A</kbd>       | Move the player to the left                                          |
| <kbd>S</kbd>       | Move the player backwards                                            |
| <kbd>D</kbd>       | Move the player to the right                                         |
| <kbd>R</kbd>       | Rotate the player                                                    |
| <kbd>Space</kbd>   | Place colored explosive egg                                          |

### 💣 Explosive Combat

Deploy a diverse range of explosive eggs strategically to eliminate
enemies and open pathways. Pay attention to blast radius, as explosions
can also harm the player. Every explosive egg will have a color showing
its trait. The selection of these eggs will be random. 

| Egg Color        | Description                                                          |
|------------------|--------------------------------------------------------------------- |
| Red              | The explosion will be one unit in every direction                    |
| Blue             | The explosion will be the whole row                                  |
| Green            | The explosion will be the whole column                               |
| Yellow           | The explosion will be the whole row + column                         |

### 🦆 Enemy Intelligence

There will be intelligent enemy ducks with A* navigation that pursue the
player dynamically, requiring cautious planning and swift reflexes to
evade them. Upon contact, a player loses. If an enemy is hit by the bomb,
they will perish. Explosive eggs follow realistic behaviour which depend
on factors like initial velocity, blast force, and environmental barriers.
When explosions occur, the affected ducks experience tangible effects that
influence their movements in a lifelike way, due to the physics engine.


### Game demo

Here a short gameplay:

https://github.com/fini03/QuackBlast/assets/112564342/78a74482-4bbe-4b39-aaa7-d2461315babf
