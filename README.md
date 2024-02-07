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

Players control the duck's movement through mazes using the **WASD**
keys, navigating obstacles and strategically placing explosive eggs with
the **SPACE** key. It is also possible to rotate the duck by pressing the
**R** key.

### 💣 Explosive Combat

Deploy a diverse range of explosive eggs strategically to eliminate
enemies and open pathways. Pay attention to blast radius, as explosions
can also harm the player. Every explosive egg will have a color showing
its trait. The selection of these eggs will be random. 

| Egg Color        | Description                                                          |
|------------------|--------------------------------------------------------------------- |
| <kbd>Red</kbd>   | The explosion will be one unit in every direction                    |
| <kbd>Blue</kbd>  | The explosion will be the whole row                                  |
| <kbd>Green</kbd> | The explosion will be the whole column                               |
| <kbd>Yellow</kbd>| The explosion will be the whole row + column                         |

### 🦆 Enemy Intelligence

There will be intelligent enemy ducks with A* navigation that pursue the
player dynamically, requiring cautious planning and swift reflexes to
evade them. Upon contact, a player loses. If an enemy is hit by the bomb,
they will perish. Explosive eggs follow realistic behaviour which depend
on factors like initial velocity, blast force, and environmental barriers.
When explosions occur, the affected ducks experience tangible effects that
influence their movements in a lifelike way, due to the physics engine.
