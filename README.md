## About
![Alt text](https://github.com/artmalini/lem-in/res/ant.jpg "lem")

The goal of this project is find quiqest path for ant swarm. All ants start in the first room according to map and start running
to find exit. There should be manage traffic james and avoid that one room cannot contain more that one ant per move. All
swarm must follow on the one position in room per round. More clear description at file "lem-in.en.pdf".

## Build
This will be work on Linux or MacOs terminal:

   Release build:

```
$ make
```

   Than you can start typing keybord to see usage like this:

```
$ ./lem-in gdg
```
![Alt text](https://github.com/artmalini/lem-in/res/lem_usage.jpg "lem usage")

Then execute command
```
$ ./lem-in -v < maps/map
```

Program start working and end shortly after reading and executihg file at maps/map. There 65 ants atop in file, so all ants start
at room 1. There to possible ways and only two ants follow to room 3 and 2, (L1-3 L2-2 in a top). On next turn these two
ants goes to next rooms according to the rules this will be room 4 and 5, (L1-4 L2-5). And room 3 and 2 now free to enter, than
on this iteration we let to enter new ants L3 and L4 and they went to room 3 and 2. Thats why our output longer on that time. On
each iterations continues same process and all ants stay at room 0.
