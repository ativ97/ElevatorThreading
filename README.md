# ElevatorThreading

A M -story highrise has N elevators in a single "elevator bank", each able to serve every floor of the building. Contrary from most elevator designs, there are no buttons to choose a destination floor inside the elevator. Instead, the elevator lobby at each floor has M individual "destination floor" buttons, and passengers choose their destination by pushing the appropriate button. Once a button is pushed, a display near the buttons tells the passenger which elevator door to wait by.
Through thread barriers and mutexes and other concurrent programming practices, multiple elevators are able to serve multiple people at the same time.