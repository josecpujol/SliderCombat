# SliderCombat

## Compilation instructions and execution
### Ubuntu

Install the dependencies:
```
sudo apt-get cmake install libsdl2-dev libglm-dev libglew-dev libsdl2-image-dev
```

Compile: 
```
./build_native.sh
```

Execution
```
./SliderCombat
```

In order for the program to load, it has to be in the root directory of the project to find the resources

### Windows
Open `SliderCombat.sln` in Visual Studio 2017

### Compile to HTML/Javascript
Install emscripten (tested with 1.38.20)
Compilation:
```
./build_html
```

Execution:
* Serve the content inside `build_html` with a HTTP server (like `python -m SimpleHTTPServer`) 
* Open the page `http://127.0.0.1:8000/SliderCombat.html`

## Libraries and resources used
* tinyobjloader, by Syoyo Fujita. https://github.com/syoyo/tinyobjloader
* tinyxml2, by Lee Thomason. https://github.com/leethomason/tinyxml2
* tmxparser, by Adaleigh Martin. https://github.com/sainteos/tmxparser
* Prototype font, by mouse@mickeyavenue.com

