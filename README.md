# Graph

*Graph* is a poorly named desktop application for [graph](https://en.wikipedia.org/wiki/Graph_(discrete_mathematics)) operations.

### Available methods:
* [Breadth-first search](https://en.wikipedia.org/wiki/Breadth-first_search)
* [Depth-first search](https://en.wikipedia.org/wiki/Depth-first_search)
* [Bellman–Ford algorithm](https://en.wikipedia.org/wiki/Bellman%E2%80%93Ford_algorithm)
* [Ford–Fulkerson algorithm](https://en.wikipedia.org/wiki/Ford%E2%80%93Fulkerson_algorithm)
* [Topological ordering](https://en.wikipedia.org/wiki/Topological_sorting)
* Vertex [degree](https://en.wikipedia.org/wiki/Degree_(graph_theory)) calculation
* Detecting [cycles](https://en.wikipedia.org/wiki/Cycle_(graph_theory)) in a graph
* Graph [coloring](https://en.wikipedia.org/wiki/Graph_coloring)
* Checking [connectivity](https://en.wikipedia.org/wiki/Connectivity_(graph_theory))
* [Planarity](https://en.wikipedia.org/wiki/Planar_graph) testing

You can also enable force-directed vertex placement method in settings

### Build
#### Requirements
* [Qt library](https://qt.io)
* a C++11-standard-compliant compiler

#### Build from the command line
Setup Qt environment and run the following commands from project root:
```bash
mkdir build_dir
cd build_dir
qmake ../src/Graph.pro
make Makefile
```
For MinGW replace `make` line to:
```bash
mingw32-make.exe -f Makefile
```

#### Build from QtCreator
Setup Qt environment and open `Graph.pro` project file with QtCreator. Build project. So it's easy to build in QtCreator


### Running the unit tests
* will be possible after they are written
