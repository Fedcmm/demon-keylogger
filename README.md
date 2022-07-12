# Demon Keylogger
This is a simple keylogger which runs as a daemon developed for the
O.S. exam at [UNICAM](http://www.unicam.it/)

---
### Building the project
To build the project, open a terminal inside the main folder, then run 
the following commands:
```bash
cmake . -B build
cd build
make
```

### Usage
After building, the executable will be located in the *build* directory, 
so position the terminal inside it and run
```bash
sudo ./demon_keylogger path-to-output-file.txt
```
The keystrokes will be logged to the specified file.

To stop the daemon, you can run
```bash
pidof demon_keylogger
```
which will return the IDs of the two processes, then use the *kill* 
command as following (with "pid" being one of the two IDs)
```bash
sudo kill -s INT pid
```
The other process will stop automatically.

### Note
If the path to the output file is just the name of the file itself, 
it will be placed in the root directory ("/") of the filesystem.