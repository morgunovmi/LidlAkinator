# LidlAkinator 

![snap](https://github.com/morgunovmi/LidlAkinator/blob/master/resources/snap.png)

## Introduction

This command line program allows you to play around with your personal Akinator (Lidl version).
It will blow your mind with its ability to guess what you have in mind (provided that what you have in mind
also exists in its database (and also that you answer the questions in the same way as the person who populated the database
(yeah...))) 

## Summary

* [Introduction](#introduction)
* [Game modes](#game-modes)
* [Prerequisites](#prerequisites)
* [Usage](#usage)

## Game modes

1. Guess the object

In this mode my great value Akinator will try to guess what you have in mind
by asking you several yes/no questions. If your object is not in the database
it will ask you to name your object and give it a defining feature.

2. Dump the database

In this mode the Akinator dumps the current state of the database in a tree form
and display it to you.

3. Define the object

In this mode the allmighty Akinator will define an object you give it 
(if it exists in the database).

4. Compare two objects

In this mode your machine overlord will compare two objects you give it
(if they are in the db)

## Prerequisites

This project was built on an Ubuntu-based Linux distro, but any Unix-based system
will probably do

To run this program you will also need __make__, __gnustep-gui-runtime__(for text-to-speech
generation), __fim__ (for image output) and __dot__ (for graphical dumps with graphviz).

You can install them with:
```
sudo apt install make fim gnustep-gui-runtime dot
```
(Or alternative commands depending on your distro's package manager)

## Usage

First build the binary by running the following:
```
git clone https://github.com/morgunovmi/LidlAkinator.git && cd ./LidlAkinator
make
```

You can then run it with
```
./akinator
```

You can also pass some options to the program:
```
Usage: akinator [OPTION...]

  -d, --debug                Makes tree dump debug friendly
  -f, --file=FILE_PATH       Use a custom file for the database
  -s, --say                  Use text to speech
  -?, --help                 Give this help list
      --usage                Give a short usage message

```
(Runs with an empty database by default)

### GLHF! @me with errors
