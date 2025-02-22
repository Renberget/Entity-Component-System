# Entity Component System
An archetype-based [ECS](https://en.wikipedia.org/wiki/Entity_component_system) in C++ 20.\
Entities and components can be iterated through queries that include or exclude some component types.\
Components are accessed through the CompPtr or CompOpt class so we can't remove a component if it is referenced.

### Planned
- Unit tests to safely integrate new features
- SIMD and multithreading support
- Faster queries with an archetype graph
