## Tetris Game MVC Flowchart

+------------------------------------+
|           MAIN MODULE              |
| (main.c: Initializes Game Loop)    |
|                                    |
|  1. Initialize GameController      |
|  2. Start Game Loop (runGame)      |
+------------------------------------+
               |
               v
+------------------------------------+
|        GAME CONTROLLER MODULE      |
|   (controller.c: Manages Game Flow)|
|                                    |
|  1. Initialize Game (initGameController) |
|     - Initialize GameBoard         |
|     - Initialize Renderer          |
|                                    |
|  2. Game Loop (runGame)            |
|     - Handle Input (handleInput)   |
|     - Update Game State (updateGame)|
|     - Render Game (drawBoard)      |
+------------------------------------+
               |
               v
+------------------------------------+
|        INPUT HANDLING              |
|                                    |
|  1. Poll SDL Events                |
|  2. Set Movement Flags             |
|  3. Handle Rotation                |
+------------------------------------+
               |
               v
+------------------------------------+
|        GAME LOGIC (MODEL)          |
| (model.c: Manages Game State)      |
|                                    |
|  1. Reset Game Board               |
|  2. Check Collision (checkCollision)|
|  3. Place Shape (placeShape)       |
|  4. Clear Lines (clearLines)       |
|  5. Rotate Shape (rotateShape)     |
+------------------------------------+
               |
               v
+------------------------------------+
|        RENDERING (VIEW)            |
|  (view.c: Handles Display)         |
|                                    |
|  1. Initialize Renderer            |
|     - Create SDL Window            |
|     - Create SDL Renderer          |
|     - Load Font for Text           |
|                                    |
|  2. Draw Game Board (drawBoard)    |
|     - Draw Blocks                  |
|     - Draw Current Shape           |
|     - Display Score (renderText)   |
|                                    |
|  3. Clean Up (cleanupRenderer)     |
|     - Close Font                   |
|     - Destroy Window & Renderer    |
+------------------------------------+


## Detailed Explanation of the Flowchart:

	1.	Main Module (main.c):
	•	The game starts by initializing the GameController, which in turn sets up the game environment, including the game board and renderer.
	•	The main game loop is executed via the runGame function, which continues until the game ends.
	2.	Game Controller (controller.c):
	•	The game controller manages the main game loop. It repeatedly handles input, updates the game state, and triggers the rendering of the game board.
	•	initGameController sets up the initial game state, while handleInput, updateGame, and drawBoard are repeatedly called within the loop.
	3.	Input Handling:
	•	The controller polls SDL events to capture player input, such as moving shapes left or right, rotating them, or speeding up their fall. This input is then processed to update the game state accordingly.
	4.	Game Logic (Model, model.c):
	•	This module handles the core game logic, including checking for collisions, placing shapes on the board, clearing completed lines, and rotating shapes.
	•	When a shape is placed, the game checks if any lines are complete and updates the score accordingly.
	5.	Rendering (View, view.c):
	•	The view module handles all rendering operations. It initializes the SDL window and renderer, loads the necessary fonts, and then renders the game board, current shape, and the score on the screen.
	•	After each frame is updated in the model, the view redraws the entire scene to reflect the current game state.
	6.	Cleanup:
	•	When the game ends or the program is closed, the renderer and other resources are cleaned up, ensuring that memory is properly managed.

## Todo
