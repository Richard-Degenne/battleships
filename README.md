Battleships
===========

About
-----

Network Battleship game for Linux terminal.

This is an amateur project developed by two French student as a school task.

Developers
----------

* Richard Degenne (`richdeg2 AT gmail DOT com`)
* Adrien Deprez

License
-------

Battleships is distributed under the GNU General Public License. See LICENSE
for further details.

User's Manual
-------------

## Download

You can download Battleships in many ways:

1. Cloning Git repository with `git clone https://www.github.com/Richard-Degenne/battleships.git`.
2. Downloading a zipped file by clicking [here](https://www.github.com/Richard-Degenne/battleships/archive/master.zip) and extract it.

## Build

Move into the project folder and run `make`.


## Run the server

Move to the `server` folder and run `./main`.

## Run the clients

Move to the `client` folder and run `./main`.

You'll be prompted to the server address. Enter `127.0.0.1` if both the client and the server are running on the same machine, ou run `ifconfig` on the server machine to get its address.

You'll then be prompted for a nickname, and a game mode. If you host a game, the program will wait for another player to join you. If you join a game, you'll be showed the available games and prompted again to select one.

## Play!

You first have to place your fleet on the battlefield. Select a coordinate for your first boat. The program will then let you choose from available positions stating from the coordinate you previously entered. Repeat for each boat.

When you're done placing your boats, let the fight start and good luck!

Documentation
-------------

If you want to know more about how the game is done, check the `doc` folder. It contains a full documentation about the code.

Bug tracking
------------

Please note that the game is *still being developed*. It's full of bugs and needs lots of improvements. However, if an error occurs, please look up your problem in the [bugtracker](https://www.github.com/Richard-Degenne/battleships/issues) and, if your problem is not listed there, submit a new issue, explaining what happened and, if possible, how to reproduce your problem.

That will help us improve the game as fast as possible! :)
