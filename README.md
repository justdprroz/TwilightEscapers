# TwilightEscapers

## What this game is about?

This will be a RPG game about some guys escaping  \*Twilight\*(Some strange place where you will fight monsters and gather cool stuff)

## Requirements

### General Requirements
- Check if you installed [CMake](https://cmake.org/) __3.17__ or __newer__
- Ensure that you have [sfml](https://www.sfml-dev.org/) installed
- Install C++ compiler(g++ for Linux and Visual Studio for Windows)

### Windows

#### SFML
- Install [SFML for visual studio](https://www.sfml-dev.org/download/sfml/2.5.1/)
- Extract downloaded archive and place its content(Folder containing **bin** **include** etc.) to ```C:\``` renaming ```SFML-[VERSION]``` to just ```SFML```
- Add content of ```C:\SFML\bin``` to PATH by editing enviroment variables or by copying this files to directory that already in PATH

#### Visual Studio
- Install Visual Studio and add folder containig ```MSBuild.exe``` to PATH
- In my case i added this folder ```C:\Program Files\Microsoft Visual Studio\2022\Community\Msbuild\Current\Bin```

## How to build?

### Linux:
- Run ```./build.sh``` or ```./run.sh```
### Windows:
- Run ```.\build.bat``` or ```.\run.bat``` 

### MacOS:
- __MacOS is unsupported right now__
- If you have MacOS you can try adapt Linux build instructions and pull request them

## How to run

### Linux:
1) Run ```./run.sh```, this script will ensure that all necessary files are present before running game
2) Or navigate to ```build``` directory and run ```./twe```

### Windows:
1) Run ```./run.bat```, this script will ensure that all necessary files are present before running game
2) Or navigate to ```build``` directory and run ```.\twe.exe```

## How to play
__Currently game lacks any content and features__
- You can move using ```W```, ```A```, ```S```, ```D```
- Press ```Shift``` to speed up, Press ```Left Control``` to slow down
- Press ```Enter``` to enable \*Flight\* (Very high speed, primarily for debugging)
- Press ```F3``` or ```BackSlash``` to toggle debug info

## How to contribute
- Create issues if needed
- You can suggest improvements or any other features or bugfixes
