# **Resources Manager**

Parallelizing the loading of resources avoiding "dead-locks" and "data-races". Go through synchronizing operations and uderstanding memory alignment.

**/!\ This engine is not the most optimized one nor the most efficient one. It supports only .obj models, .mtl materials and a custom scene format. /!\\**

# Informations
The program runs in Debug or Release, platform x64.

A default scene is loaded, you can download the associated resources folder at this link :
https://drive.google.com/file/d/1VOl7ENR7cPMmUbznONpCpKUGxFjzRMf9/view?usp=sharing

For obj's, textures needed to be put on the same folder as the .obj and .mtl

Note : A bug appears when loading the scene for the first time. Multiple objects are not loaded due to a problem with the working directory. You can simply reload the scene from the ImGui menu to fix the problem.

# Controls
- **WASD/ZQSD keys** : Move foward, backward, left and right.
- **Left Shift** : Move faster (sprint).
- **Escape** : Edit mode.

# Edit Mode Features
Edit Mode  is separate in two main headers. Graph that allow different actions on the global scene, Inspector that allow actions on a selected Game Object.

More precisely, the Graph Header is also seperated in 4 headers :

## The scene header
The scene header is used to manage the different scenes. You will see the current scene name follow by a checkbox to activate or deactivate the multithreaded loading. After that, you have 4 different Tabs that will allow you to load an existing scene, reload the current scene, save the current scene or create a new scene from the template scene (Action confirm by clicking linked button). Scenes are saved at ProjectDir/resources/scenes/

 BE CAREFUL : for the loading and creating parts, give a name and finish by .scn. If not respected, engine will assert and crash, eg. myScene.scn

## The benchmark header
The benchmark header is used to benchmark the loading of the current scene. More precisely, to compare the loading scene speed in monothread and multithread.

The red part allow to launch a benchmark with a selected count and save the current benchmark (file save at ProjectDir/logs/benchmark/). Note that a benchmark is automatically saved when the application stopped (ProjectDir/logs/benchmark.txt).

The orange part is the last benchmark informations. You will see the objects, triangles and textures count. And you have the different loading times for each modes and a ratio between them.

When the editor is back, benchmark is over.

## The model header
This header allows you to load a .obj model on the current scene (objs needed to be stored at ProjectDir/resources/obj/ and .mtl + textures on the same folder as the .obj).

You can also select the loading mode with the checkbox : Mono/Multi.

## The hierarchy header
The hierarchy header allows you to modify the gameObjects. When a gameObjects is selected, you can change parameters on the "Inspector" window.

***Annexes***
===
- Link to the UML:
https://miro.com/app/board/o9J_lu8fy1g=/

***Third-party programs and libraries***
===

# Download links

irrKlang
---
https://www.ambiera.com/irrklang/

stb
---
https://github.com/nothings/stb

GLFW
---
https://www.glfw.org/

Glad
---
https://glad.dav1d.de/

***References***
===
OpenGL:
---
- Gives the different functions of OpenGL:
http://docs.gl/
- Ditto:
https://www.khronos.org/opengl/

C++:
---
- C++ references: https://en.cppreference.com/