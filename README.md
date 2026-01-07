# Pacman

###  Marie Van Nuffel
###  Studentennummer: 20240829

### Structuur:
logic/        
- World.h – centraal voor game logic en interacties
- Entity.h – abstracte basisklasse voor alle entiteiten
- PacManModel.h, GhostModel.h, CoinModel.h, FruitModel.h – entity models
- Direction.h – bewegingsrichting
- Subject.h / Observer.h – Observer pattern
- Score.h – scoreberekening & highscores
- Stopwatch.h – deltaTime (Singleton)
- Random.h – random generator (Singleton)
- 
view/           # SFML rendering & states
- Game.h – setup van window, main loop
- StateManager.h – stak van States
- MenuState.h, LevelState.h, PauseState.h – concrete states
- EntityView.h – abstracte view
- PacManView.h, GhostView.h, CoinView.h, FruitView.h – entity rendering
- MazeView.h – maze rendering
- Camera.h – world naar pixel projectie
- ConcreteEntityFactory.h – koppelt logic aan views
- Resource.h – texture caching

### MVC (Model-View-Controller)
| Component      | Rol                                                              |
| -------------- |------------------------------------------------------------------|
| **Model**      | Bevat eigenschappen van entiteiten (positie, richting, snelheid) |
| **View**       | SFML rendering & animaties                                       |
| **Controller** | `World` coördineert interacties en game logica/regels            |

### Wereldcoördinaten en Camera
In de game hou ik posities bij in tegels (tile‑coords). 
De logica werkt dus met waarden zoals (x + 0.5, y + 0.5) voor het midden van een tegel, dus geen pixels en geen SFML. 
De Camera in de view zet die tile‑coords om naar pixels zodat het op het scherm getekend kan worden. 
Technisch doet de Camera dat via een tussenstap naar een genormaliseerd coördinatenstelsel ([-1,1]) en projecteert dat vervolgens naar het deel van het venster waar de maze staat (met centering en schaal). 
Daardoor blijft alles er goed uitzien op verschillende resoluties, en kun je de logica zonder SFML compileren. 
De logic blijft dus grafisch onafhankelijk en de Camera regelt de vertaling naar schermpixels.

### Design patterns
| Design pattern            | Toepassing                            | Doel                                                   |
| ------------------------- | ------------------------------------- |--------------------------------------------------------|
| **Model-View-Controller** | World (Controller), Models en Views   | Scheiding tussen logica en representatie               |
| **Observer**              | Models → Views & Score                | Automatische updates bij state veranderingen (events)  |
| **Abstract Factory**      | ConcreteEntityFactory                 | Aanmaken van entiteiten zonder afhankelijkheid van SFML |
| **Singleton**             | Stopwatch, Random                     | Eén instantie voor timing en randomisatie       |
| **State**                 | StateManager + MenuState / LevelState | Makkelijke state wisselingen met state stack           |

### State Systeem
Het spel maakt gebruik van een state stack om verschillende speltoestanden te beheren zoals:
- MenuState
- LevelState
- PauseState
De StateManager beheert deze stack, waarbij enkel de bovenste state actief is. Hierdoor kan bijvoorbeeld een pauzescherm bovenop een lopend level geplaatst worden zonder de onderliggende state te vernietigen.

### Tijd en score
- Stopwatch berekent deltaTime, waardoor beweging en animaties framerate onafhankelijk zijn.
- Score ontvangt events via het observer pattern en berekent dynamisch de score, inclusief tijdsbonussen en levelbonussen.
- Highscores worden opgeslagen in een bestand.

### Bonus Features
- GhostDoor
De deur die de ghostbox afsluit waar alleen de ghosts doorheen kunnen, en Pac-Man niet.
De World controleert bij elke beweging of pacman of een ghost door de ghostdoor wilt.
pacman kan er niet door, ghosts wel, maar alleen als ze terug naar hun spawn point gaan wanneer ze opgegeten zijn.
Dit gebeurt volledig via collision checks in de world class, dus de logica blijft netjes gescheiden van de rendering.

- Menu Animatie 
Het menuscherm heeft een kleine animatie om het spel visueel aantrekkelijker te maken.
Van links naar rechts op het beeld zie je pacman die achtervolgt wordt door de ghosts.
Dit gebeurt volledig in de view, niet in logic.

- Ghost AI met Breadth-First Search (BFS)
Ghosts gebruiken het BFS algoritme om terug naar hun spawn point te gaan wanneer ze opgegeten zijn.
Deze search is veel slimmer dan de manhatten search.
