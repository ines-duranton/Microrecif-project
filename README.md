# Microrecif-project

## Overview

Microrécif is a C++ simulation engine modelling the evolution of an artificial ecosystem with corals, algae and scavengers. It was realized for an Object-Oriented programming class at EPFL, in the spring of 2023. in the context of this class, the project was created without AI to understand some key software engineering challenges : 

- modular software architecture
- object-oriented design
- 2D collision detection
- event-driven simulation
- graphical rendering

!!! Explanation of the functionning of the game and its goal
The application includes a GTKmm graphical interface allowing users to visualize and interact with the simulation.

---

## Features

- Object-oriented architecture
- Interactive GUI (GTKmm)
- File import/export
- Simulation loop
- Collision detection
- Segment intersection algorithms
- Dynamic entity management
- Event-driven updates

---

## Software Architecture

The project is organised into different independent modules:

!!!review that tree

```
GUI
 │
 ▼
Simulation
 │
 ├── Lifeforms
 │     ├── Coral
 │     ├── Algae
 │     └── Scavenger
 │
 ├── Shape
 └── Graphic
```

This separation makes each module responsible for a single concern and simplifies maintenance and testing.
!!!explanation of each module, in a separate section ?

Simulation : handles the simulation updates, the visual drawings of the entites, the reading of potential input files and the writing of output files.

---

!!!Explain how to start this project

## Technologies

Language: C++17
GUI: GTKmm
Build system: Make
Paradigms: Object-Oriented Programming, modular design

---

## Skills acquired

This project required implementing:

- software modularity
- simulation architecture
- object interactions
- event handling
- graphical visualization
- testing and debugging of a multi-module application

---

## Build

!!!make better explanations and presentation

```bash
make
```

Run

```bash
./projet example.txt
```

---

## Why this project?

!!!to modify

Although the simulated environment is a coral ecosystem, the software architecture and algorithms are directly applicable to robotics software:

- environment simulation
- geometric reasoning
- collision handling
- modular software development
- event-driven systems
