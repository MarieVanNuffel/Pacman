# Pacman

###  Marie Van Nuffel

### Structuur:
logic/          # Models, observer pattern, score, stopwatch
- Entity.h
- PacManModel.h
- GhostModel.h
- CoinModel.h
- FruitModel.h
- Subject.h / Observer.h
- Score.h
- Stopwatch.h
view/           # SFML rendering & states
- EntityView.h
- PacManView.h
- GhostView.h
- CoinView.h
- FruitView.h
- MazeView.h
- Camera.h
- LevelState.h
- MenuState.h
- ConcreteEntityFactory.h
- Game.h
- StateManager.h

### Design patterns
| Pattern              | Waar toegepast                                                             | Opmerking                                                                |
|----------------------| -------------------------------------------------------------------------- | ------------------------------------------------------------------------ |
| **Observer**         | CoinModel, FruitModel → Score & Views                                      | Voor automatische updates van UI bij verzamelen van items                |
| **Abstract Factory** | ConcreteEntityFactory                                                      | Creëert view-objecten voor elk type entity (Pac-Man, Ghost, Coin, Fruit) |
| **State**            | StateManager + MenuState / LevelState                                      | Maakt een flexibele game state machine mogelijk                          |
| **Singleton**        | Stopwatch                                                                  | Voor consistent bijhouden van delta-time                                 |


