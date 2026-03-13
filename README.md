## TSiren

### Compile (Linux released done, u can downloand directly)
```
cd build && cmake . && make
```

### Installation

#### Linux/Macos
```
sudo mv siren /usr/local/bin
```

#### Win
1 - Create a folder for tool. e.g ```C:\siren```

2 - Add that folder to PATH:
  - Search for "Environment Variables" in Windows Start.
  - Edit the Path variable and add ```C:\siren```

3 - Move siren.exe into that folder.

4 - Restart your terminal and run ```siren [MIN] [SEC]```

### Usage
```siren 5``` : 5 minutes timer.

```siren 0 30``` : 30 seconds timer.

```siren 1 30``` : 1 minutes 30 seconds timers.
