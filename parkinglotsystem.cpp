#include<bits/stdc++.h>

using namespace std;

// Enum for vehicle types
enum class VehicleType { Bike, Car, Truck };

// Vehicle Class
class Vehicle {
public:
    string plateNumber;
    VehicleType type;
    int requiredSpots;

    // Constructor to initialize vehicle details
    Vehicle(string plate, VehicleType vType) : plateNumber(plate), type(vType) {
        requiredSpots = (vType == VehicleType::Truck) ? 2 : 1; // Trucks require 2 spots, others require 1
    }
};

// Parking Spot Class
class ParkingSpot {
public:
    int spotID;
    bool isOccupied;
    shared_ptr<Vehicle> vehicle; // Shared pointer for automatic memory management

    ParkingSpot(int id) : spotID(id), isOccupied(false), vehicle(nullptr) {}

    void assignVehicle(shared_ptr<Vehicle> v) {
        vehicle = v;
        isOccupied = true;
    }

    void removeVehicle() {
        vehicle = nullptr;
        isOccupied = false;
    }
};

// Parking Floor Class
class ParkingFloor {
public:
    int floorNumber;
    vector<ParkingSpot> spots;

    ParkingFloor(int number, int spotsPerFloor) : floorNumber(number + 1) { // Start floor numbering from 1
        for (int i = 1; i <= spotsPerFloor; ++i) { // Start spot numbering from 1
            spots.emplace_back(i);
        }
    }

    // Find available spot(s) for a vehicle
    int findAvailableSpot(shared_ptr<Vehicle> vehicle) {
        int needed = vehicle->requiredSpots;
        for (int i = 0; i <= spots.size() - needed; ++i) {
            bool canPark = true;
            for (int j = 0; j < needed; ++j) {
                if (spots[i + j].isOccupied) {
                    canPark = false;
                    break;
                }
            }
            if (canPark) return i;
        }
        return -1;
    }

    // Park vehicle
    bool parkVehicle(shared_ptr<Vehicle> vehicle) {
        int spotIndex = findAvailableSpot(vehicle);
        if (spotIndex == -1) return false;

        for (int i = 0; i < vehicle->requiredSpots; ++i) {
            spots[spotIndex + i].assignVehicle(vehicle);
        }
        return true;
    }

    // Remove vehicle
    void removeVehicle(const string& plateNumber) {
        for (auto& spot : spots) {
            if (spot.isOccupied && spot.vehicle && spot.vehicle->plateNumber == plateNumber) {
                spot.removeVehicle();
            }
        }
    }

    // Get available spots
    int availableSpots() const {
        int count = 0;
        for (const auto& spot : spots) {
            if (!spot.isOccupied) count++;
        }
        return count;
    }
};

// Parking Lot Class
class ParkingLot {
private:
    vector<ParkingFloor> floors;
    unordered_map<string, pair<int, int>> parkedVehicles; // Map: plate -> (floor, spot)
    set<string> parkedPlates; // Set to track parked plate numbers
    mutex mtx;

public:
    ParkingLot(int numFloors, int spotsPerFloor) {
        for (int i = 0; i < numFloors; ++i) {
            floors.emplace_back(i, spotsPerFloor);
        }
    }

    // Park a vehicle
    bool parkVehicle(const string& plateNumber, VehicleType type) {
        lock_guard<mutex> lock(mtx);

        // Check if the vehicle is already parked
        if (parkedPlates.find(plateNumber) != parkedPlates.end()) {
            cout << "Vehicle with plate number " << plateNumber << " is already parked." << endl;
            return false;
        }

        auto vehicle = make_shared<Vehicle>(plateNumber, type);

        for (auto& floor : floors) {
            int spotIndex = floor.findAvailableSpot(vehicle);
            if (spotIndex != -1) {
                floor.parkVehicle(vehicle);
                parkedVehicles[plateNumber] = {floor.floorNumber, spotIndex + 1};
                parkedPlates.insert(plateNumber);
                cout << "Vehicle " << plateNumber << " parked at Floor "
                     << floor.floorNumber << ", Spot " << (spotIndex + 1) << endl;
                return true;
            }
        }
        cout << "Parking lot is full!" << endl;
        return false;
    }

    // Remove a vehicle
    bool removeVehicle(const string& plateNumber) {
        lock_guard<mutex> lock(mtx);

        if (parkedPlates.find(plateNumber) == parkedPlates.end()) {
            cout << "Vehicle with plate number " << plateNumber << " is not parked." << endl;
            return false;
        }

        int floorNum = parkedVehicles[plateNumber].first;
        floors[floorNum - 1].removeVehicle(plateNumber);
        parkedVehicles.erase(plateNumber);
        parkedPlates.erase(plateNumber);
        cout << "Vehicle " << plateNumber << " has left the parking lot." << endl;
        return true;
    }

    // Check available spots per floor
    void availableSpotsPerFloor() const {
        for (const auto& floor : floors) {
            cout << "Floor " << floor.floorNumber << ": "
                 << floor.availableSpots() << " spots available" << endl;
        }
    }

    // Find vehicle location
    void vehicleLocation(const string& plateNumber) const {
        if (parkedVehicles.find(plateNumber) != parkedVehicles.end()) {
            cout << "Vehicle " << plateNumber << " is parked at Floor "
                 << parkedVehicles.at(plateNumber).first << ", Spot "
                 << parkedVehicles.at(plateNumber).second << endl;
        } else {
            cout << "Vehicle not found in the parking lot." << endl;
        }
    }
};

// Command-line interface for user input
void ParkingLotMenu(ParkingLot& parkingLot) {
    while (true) {
        cout << "\n----- Parking Lot Menu -----" << endl;
        cout << "1. Park a Vehicle" << endl;
        cout << "2. Remove a Vehicle" << endl;
        cout << "3. Check Available Spots" << endl;
        cout << "4. Find Vehicle Location" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (choice == 5) break;

        string plateNumber;
        int vehicleType;
        switch (choice) {
            case 1:
                cout << "Enter Vehicle Plate Number: ";
                cin >> plateNumber;
                cout << "Select Vehicle Type (1: Bike, 2: Car, 3: Truck): ";
                cin >> vehicleType;
                parkingLot.parkVehicle(plateNumber, static_cast<VehicleType>(vehicleType - 1));
                break;
            case 2:
                cout << "Enter Vehicle Plate Number to Remove: ";
                cin >> plateNumber;
                parkingLot.removeVehicle(plateNumber);
                break;
            case 3:
                parkingLot.availableSpotsPerFloor();
                break;
            case 4:
                cout << "Enter Vehicle Plate Number to Find: ";
                cin >> plateNumber;
                parkingLot.vehicleLocation(plateNumber);
                break;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 5." << endl;
        }
    }
}

int main() {
    int n, m;
    cout << "Enter the number of floors: ";
     if (!(cin >> n) || n <= 0) {
        cout << "Please enter a valid positive integer for floors.";
        return 0;
    }

    cout << "Enter the number of parking slots on each floor: ";
    if (!(cin >> m) || m <= 0) {
        cout << "Please enter a valid positive integer for slots.\n";
        return 0;
    }
    
    
    ParkingLot parkingLot(n, m);
    ParkingLotMenu(parkingLot);
    return 0;
}
