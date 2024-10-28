# B-CPP-500-COT-5-1-rtype-fitzgerald.adechian

# R-TYPE

This Advanced C++ knowledge unit project enables the development of networked video games and provides an opportunity to explore advanced development techniques and learn good software engineering practices.
The aim is to implement a multithreaded server and a graphical client for a well-known existing video game called R-Type, using a game engine of your own design.

## PROJECT FEATURES

The project requires the development.... (a completer)

### Server Usage

The customer has the:

1. customer login and logout
2. serialize and deserialize data sent and received
3. compression of sent and received data
4. integration of ECS with server
   
### Client Usage

The customer has the:

1. reception of different information sent by the server
2. display of various asserts
3. retrieve various user inputs and send them to the server
4. set up an attractive client interface (GUI)
5. integration of ECS with client

To launch the server:

> ./r_type_server  <address> <port> <lobby_status>

Now, the clients could connect to the server

To launch the client:

> ./r_type_client  <address> <port> <sound> <lobby_status>

Enjoy the gameplay, and tell your friends to join you !

### ECS Usage

We choosed to use an ECS architecture for our engine.
Entity Component System (ECS) is a software architectural pattern mostly used in video game development for the representation of game world objects. An ECS comprises entities composed from components of data, with systems which operate on entities' components.

ECS follows the principle of composition over inheritance, meaning that every entity is defined not by a type hierarchy, but by the components that are associated with it. Systems act globally over all entities which have the required components.

### Game Logics Usage

## Dependencies

### Dependencies Windows

1. > Installing CMAKE
2. > Instaling CONAN and his package
3. > Installing Microsft Visual Studio C++ and MSBuidl tools

### Dependencies Linux (Ubuntu)

1. > Installing CMAKE
2. > Instaling CONAN his package

## Building R-type

R-type compiles on Windows and Linux.

### Compilation using CMake

CMake is a build file generation tool used to facilitate the process of compiling and building a software project. It generates configuration files adapted to specific operating systems and development environments.
CMake '#3.27' is required.

### Build steps for Linux (Ubuntu)

  1. cmake .. -DCMAKE_BUILD_TYPE=Release
  2. ./r-type_server, ./r-type_client

### Build steps for Windows

  1. cmake .. -DCMAKE_BUILD_TYPE=Release
  2. msbuild solution.sln /p:Configuration=Release
  3. ./r-type_server, ./r-type_client

## Running R-type

1. Create file build
2. Execute commands of each build(linux and Windows) for its system

### Libraries

You must use the SFML on the client side and any rendering must be done using the SFML.
You are allowed to use Boost::ASIO for your server.


## Developer documentation

https://yoh5.github.io/

## GROUP MEMBERS
The team that worked on this project is made up of:

[Aurel Azon](https://github.com/ka3n1x)

[Fidélia Sowakoude](https://github.com/sowakoude)

[Fitzgerald ADECHIAN](https://github.com/Fitzgerald1234)

[Milohoue AHO](https://github.com/Yoh5)

[Sedjro HOUNDONOUGBO](https://github.com/Yoanncrack)


## CONTRIBUTING

Feel free to contribute to this project by adding new features, improving existing ones, or fixing bugs. Please create a new branch for each contribution and submit a pull request when ready.

![photo of a sample r-type or r-type game menu](https://myoctocat.com/assets/images/base-octocat.svg)
