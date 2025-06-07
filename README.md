# Tichu

This is a simple C++ implementation of the game "Tichu" by Fatamorgana. You can read the game's rules [here](https://fatamorgana.ch/fatamorgana/tichu/english-rules). The implementation features a client/server architecture for multiplayer scenarios.
It uses [Qt](https://www.qt.io) for the GUI, [sockpp](https://github.com/fpagliughi/sockpp) for the network interface, [rapidjson](https://rapidjson.org/md_doc_tutorial.html) for object serialization, and [googletest](https://github.com/google/googletest) for the unit tests. 

<img src="./assets/tichu_logo.png?raw=true" width="300" height="300">

## 1. Build instructions
This project only works on UNIX systems (Linux / MacOS). We recommend using [Ubuntu](https://ubuntu.com/#download).

**Note:** If you create a virtual machine, we recommend to give the virtual machine **at least 12GB** of (dynamic) harddrive space.

### Ubuntu/Linux

The OS should already have git installed. If not, you can use: 
`sudo apt-get install git`

If not already installed, install build-essential:
```bash
sudo apt-get install build-essential
```
If not already installed, install cmake:
```bash
sudo apt-get install cmake
```
If not already installed, install qt5:
```bash
sudo apt install qtbase5-dev qtbase5-dev-tools qtchooser
```

### MacOS

If not already installed, install brew:
`/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"`

Build-essential should already be installed in the os check with: 
`xcode-select --install`

Execute the following commands in a console:
1. `brew install cmake`
2. `brew install qt`
3. `export PATH="/opt/homebrew/bin:$PATH" >> ~/.zshrc`

Create a folder and navigate to it.
`cd \path\to\folder`

### Build the Project

Once the OS environment is prepared, you can build the project.

```bash
./build.sh
```

## 2. Run the Game

You can test it with the following commands:
```bash
# This will run the server and 4 clients
./run.sh
```

**Run server:** `./build/Tichu-server`

**Run client:** `./build/Tichu-client`