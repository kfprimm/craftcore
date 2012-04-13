### Introduction
Craftcore is a Minecraft inspired game engine that aims to be highly customizable and developer friendly.

### Installation/Usage
Craftcore depends on the following libraries:

* GLew   >= 1.5
* DevIL  >= 1.7.8
* LuaJIT =  2.0.0 (beta9)
* enet   =  1.3.1

On Ubuntu:

```
sudo apt-get install git libdevil-dev libglew1.5-dev
```

To install LuaJIT 2.0.0:

```
$ wget http://luajit.org/download/LuaJIT-2.0.0-beta9.tar.gz
$ tar zxf LuaJIT-2.0.0-beta9.tar.gz && cd LuaJIT-2.0.0-beta9
$ make
$ sudo make install
$ sudo ln -sf luajit-2.0.0-beta9 /usr/local/bin/luajit
```

To install enet:

```
$ wget http://enet.bespin.org/download/enet-1.3.3.tar.gz
$ tar zxf enet-1.3.3.tar.gz && cd enet-1.3.3
$ ./configure && make
$ sudo make install
```

Once that has installed, you can now build and run Craftcore:

```
$ cd craftcore
$ make && ./craftcore
```

### Contributors

* Kevin Primm (@kfprimm)

