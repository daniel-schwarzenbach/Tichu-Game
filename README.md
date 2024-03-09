# Tichu

This is a simple C++ implementation of the game "Tichu" by Fatamorgana. You can read the game's rules [here](https://fatamorgana.ch/fatamorgana/tichu/english-rules). The implementation features a client/server architecture for multiplayer scenarios.
It uses [Qt](https://www.qt.io) for the GUI, [sockpp](https://github.com/fpagliughi/sockpp) for the network interface, [rapidjson](https://rapidjson.org/md_doc_tutorial.html) for object serialization, and [googletest](https://github.com/google/googletest) for the unit tests. 

<img src="./assets/tichu_logo.png?raw=true" width="300" height="300">

## 1. Compile instructions
This project only works on UNIX systems (Linux / MacOS). We recommend using [Ubuntu 20.04](https://ubuntu.com/#download).

**Note:** If you create a virtual machine, we recommend to give the virtual machine **at least 12GB** of (dynamic) harddrive space (CLion and wxWidgets need quite a lot of space).

### 1.1 Prepare OS Environment

#### Ubuntu 20.4
The OS should already have git installed. If not, you can use: 
`sudo apt-get install git`

Then use  `git clone` to fetch this repository.

Execute the following commands in a console:
1. `sudo apt-get update`
2. `sudo apt-get install build-essential` followed by `sudo reboot`
3. if on virtual machine : install guest-additions (https://askubuntu.com/questions/22743/how-do-i-install-guest-additions-in-a-virtualbox-vm) and then `sudo reboot`
4. `sudo snap install clion --classic` this installs the latest stable CLion version
5. `sudo apt-get install libwxgtk3.0-gtk3-dev` this installs wxWidgets (GUI library used in Lama)
6. `sudo apt-get install qt5-default` this installs qt (GUI library used in Tichu)

#### MacOS Ventura
If not already installed, install brew:
`/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"`

If not already installed, install git:
`brew install git`

Build-essential should already be installed in the os check with: 
`xcode-select --install`

Execute the following commands in a console:
1. `brew install cmake`
2. `brew install qt`
3. `export PATH="/opt/homebrew/bin:$PATH" >> ~/.zshrc`

Create a folder and navigate to it.
`cd \path\to\folder`

Clone this repository (you might need to setup ssh connection):
`git clone git@gitlab.ethz.ch:bamertsa/tichu-se.git`

Now download VSCode(https://code.visualstudio.com/download)
Open VSCode and navigate to the Tichu folder

### 1.2 Compile Code
1. Open Clion
2. Click `File > Open...` and there select the **/sockpp** folder of this project
3. Click `Build > Build all in 'Debug'`
4. Wait until sockpp is compiled (from now on you never have to touch sockpp again ;))
5. Click `File > Open...` select the **/tichu-se** folder
6. Click `Build > Build all in 'Debug'`
7. Wait until Lama-server, Lama-client and Lama-tests are compiled

## 2. Run the Game
1. Open a console in the project folder, navigate into "cmake-build-debug" `cd cmake-build-debug`
2. Run server `./Tichu-server`
3. In new consoles run 4 clients to generate 4 players `./Tichu-client`