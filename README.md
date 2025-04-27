# Microcosm
The project is writtend on C++ and can be used with or without Unreal.
The example uses SDL3 for visual representation. The SDL static library already linked in the project.
Make sure you run it in win32 mode (I did only x86 sdl static library)
For more information about SDL library you can check here https://www.libsdl.org

Instruction:
All test classes lay in folder Core, all visual representation done in main.cpp

in main.cpp you can find configuration varibales:</br>
WINDOW_WIDTH = 800;</br>
WINDOW_HEIGHT = 600;</br>
TARGET_FPS = 30;</br>
FRAME_DELAY = 1000 / TARGET_FPS; // milliseconds</br>
UNIT_SIZE = 20; // Size of each unit in pixels</br>
GRID_WIDTH = 40;</br>
GRID_HEIGHT = 30;</br>
UNIT_COUNT = 5; // Number of units per color</br>

What is shown:
There are two teams blue and red,  the amount of players you can set in UNIT_COUNT.
Each of them looks for the closest enemy (opposite color) and go towards it, when its in range its starts attack.
As less health it has then more light color it has. Black color means its dead and soon will be deleted.
When unit is dead the shooter looks for another target.
