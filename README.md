# Parking Lot System

## Overview
The Parking Lot System is a command-line based application that allows managing parking spaces across multiple floors for different types of vehicles. It supports parking, removing, and querying vehicle locations efficiently.

## Features
- Supports multiple floors with a fixed number of parking spots.
- Vehicles can be of different types:
  - **Bike**: Requires 1 spot
  - **Car**: Requires 1 spot
  - **Truck**: Requires 2 consecutive spots
- Assigns the nearest available spot for parking.
- Handles vehicle removal and frees up occupied spots.
- Queries:
  - Available spots per floor
  - Parking lot full status
  - Current location of a vehicle
- Uses an **object-oriented approach** and **clean coding principles**.
- Ensures **thread safety** with `mutex` for concurrent operations.

## Technologies Used
- C++
- Standard Template Library (STL)
- Multi-threading (`mutex` for concurrency management)

## How to Use
### Compilation
To compile the program, use the following command:
```sh
 g++ -o parking_lot parking_lot.cpp -std=c++11 -lpthread
```

### Running the Application
Run the compiled executable:
```sh
 ./parking_lot
```

### Interactive Menu
The program provides a command-line menu with options:
1. **Park a Vehicle**
2. **Remove a Vehicle**
3. **Check Available Spots**
4. **Find Vehicle Location**
5. **Exit**

### Example Usage
```sh
Enter the number of floors: 3
Enter the number of parking slots on each floor: 10

----- Parking Lot Menu -----
1. Park a Vehicle
2. Remove a Vehicle
3. Check Available Spots
4. Find Vehicle Location
5. Exit
Enter your choice: 1
Enter Vehicle Plate Number: KA-01-1234
Select Vehicle Type (1: Bike, 2: Car, 3: Truck): 2
Vehicle KA-01-1234 parked at Floor 1, Spot 1
```




