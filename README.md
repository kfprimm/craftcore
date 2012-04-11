### Craftcore
Craftcore is Minecraft-type game engine that aims to be highly customizable and developer friendly.

### Installation/Usage
Craftcore depends on the following libraries:

* GLew   >= 1.5
* LuaJIT =  2.0.0 (beta9)

To install LuaJIT 2.0.0:

```
$ wget http://luajit.org/download/LuaJIT-2.0.0-beta9.tar.gz
$ tar zxf LuaJIT-2.0.0-beta9.tar.gz
$ cd LuaJIT-2.0.0-beta9 && make
$ sudo make install
```

Once that has installed, you can now build and run Craftcore:

```
$ cd craftcore
$ make && ./craftcore
```

### Contributors

* Kevin Primm (@kfprimm)

