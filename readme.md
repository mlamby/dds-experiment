# Cyclone DDS Experiments

Repo used for experimenting with the [Cyclone DDS library](https://github.com/eclipse-cyclonedds/cyclonedds) and the [python bindings](https://github.com/eclipse-cyclonedds/cyclonedds-python).

The cyclone python bindings need to be installed by following the instructions in the cyclonedds-python repo.

The c library is included as a sub-module in the 3rdparty directory.

There is plenty more to do, and this sample only just bounces a single entity up and down on the screen.

## Structure

### Publisher

The publisher is a c executable built using cmake. It publishes an entity as described in the `idl` folder that basically bounces up and down.

### Client

The client is a python client that subscribes to entity updates and moves the ball on the screen to the entity position.

The `pyidl` folder contains the python IDL bindings generated using `"idlc -lpy"`
