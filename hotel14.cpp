#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <iomanip>

struct Room {
    std::string roomNumber;
    std::string roomType;
    double price;
    bool isAssigned;
};

struct Customer {
    std::string name;
    std::string address;
    std::string phoneNumber;
    std::string roomNumber;
    int numDays;
    double totalPrice;
};

struct Food {
    std::string customerName;
    std::string roomNumber;
    std::string foodItem;
    double price;
};

void addRoom(std::unordered_map<std::string, Room>& rooms) {
    Room room;
    std::cout << "Enter room number: ";
    std::cin >> room.roomNumber;
    std::cout << "Enter room type: ";
    std::cin.ignore();
    std::getline(std::cin, room.roomType);
    std::cout << "Enter room price: ";
    std::cin >> room.price;
    room.isAssigned = false;

    rooms[room.roomNumber] = room;

    std::ofstream roomFile("rooms.txt", std::ios::app);
    roomFile << room.roomNumber << ',' << room.roomType << ',' << room.price << ',' << room.isAssigned << '\n';
    roomFile.close();

    std::cout << "Room added successfully!" << std::endl;
}

void addCustomer(std::unordered_map<std::string, Customer>& customers, std::unordered_map<std::string, Room>& rooms) {
    Customer customer;
    std::cout << "Enter customer name: ";
    std::cin.ignore();
    std::getline(std::cin, customer.name);
    std::cout << "Enter customer address: ";
    std::getline(std::cin, customer.address);
    std::cout << "Enter customer phone number: ";
    std::getline(std::cin, customer.phoneNumber);
    std::cout << "Enter room number: ";
    std::getline(std::cin, customer.roomNumber);
    std::cout << "Enter number of days: ";
    std::cin >> customer.numDays;

    auto roomIt = rooms.find(customer.roomNumber);
    if (roomIt != rooms.end()) {
        Room& room = roomIt->second;
        if (!room.isAssigned) {
            room.isAssigned = true;

            customer.totalPrice = room.price * customer.numDays;

            customers[customer.phoneNumber] = customer;

            std::ofstream customerFile("customers.txt", std::ios::app);
            customerFile << customer.name << ',' << customer.address << ',' << customer.phoneNumber << ',' << customer.roomNumber << ',' << customer.numDays << ',' << customer.totalPrice << '\n';
            customerFile.close();

            std::ofstream roomFile("rooms.txt");
            for (const auto& pair : rooms) {
                const Room& room = pair.second;
                roomFile << room.roomNumber << ',' << room.roomType << ',' << room.price << ',' << room.isAssigned << '\n';
            }
            roomFile.close();

            std::cout << "Customer added successfully!" << std::endl;
        } else {
            std::cout << "Room " << customer.roomNumber << " is already assigned." << std::endl;
        }
    } else {
        std::cout << "Room " << customer.roomNumber << " does not exist." << std::endl;
    }
}

void addFood(std::unordered_map<std::string, Customer>& customers) {
    std::string customerPhoneNumber;
    std::cout << "Enter customer phone number: ";
    std::cin.ignore();
    std::getline(std::cin, customerPhoneNumber);

    auto customerIt = customers.find(customerPhoneNumber);
    if (customerIt != customers.end()) {
        Customer& customer = customerIt->second;

        std::cout << "Available food items and prices:\n";
        std::cout << "1. Tea - Rs. 20\n";
        std::cout << "2. Coffee - Rs. 20\n";
        std::cout << "3. Sandwich - Rs. 100\n";
        std::cout << "4. Pizza - Rs. 200\n";
        std::cout << "5. Noodles - Rs. 300\n";

        int choice;
        std::cout << "Enter food item choice (1-5): ";
        std::cin >> choice;

        std::string foodItem;
        double price;

        switch (choice) {
            case 1:
                foodItem = "Tea";
                price = 20;
                break;
            case 2:
                foodItem = "Coffee";
                price = 20;
                break;
            case 3:
                foodItem = "Sandwich";
                price = 100;
                break;
            case 4:
                foodItem = "Pizza";
                price = 200;
                break;
            case 5:
                foodItem = "Noodles";
                price = 300;
                break;
            default:
                std::cout << "Invalid choice. Food not added." << std::endl;
                return;
        }

        Food food;
        food.customerName = customer.name;
        food.roomNumber = customer.roomNumber;
        food.foodItem = foodItem;
        food.price = price;

        std::ofstream foodFile("food.txt", std::ios::app);
        foodFile << food.customerName << ',' << food.roomNumber << ',' << food.foodItem << ',' << food.price << '\n';
        foodFile.close();

        customer.totalPrice += price;

        std::ofstream customerFile("customers.txt");
        for (const auto& pair : customers) {
            const Customer& c = pair.second;
            customerFile << c.name << ',' << c.address << ',' << c.phoneNumber << ',' << c.roomNumber << ',' << c.numDays << ',' << c.totalPrice << '\n';
        }
        customerFile.close();

        std::cout << "Food item added successfully!" << std::endl;
    } else {
        std::cout << "Customer with phone number " << customerPhoneNumber << " does not exist." << std::endl;
    }
}

void deleteCustomer(std::unordered_map<std::string, Customer>& customers) {
    std::string customerPhoneNumber;
    std::cout << "Enter customer phone number to delete: ";
    std::cin.ignore();
    std::getline(std::cin, customerPhoneNumber);

    auto customerIt = customers.find(customerPhoneNumber);
    if (customerIt != customers.end()) {
        customers.erase(customerIt->first);

        std::ofstream customerFile("customers.txt");
        for (const auto& pair : customers) {
            const Customer& customer = pair.second;
            customerFile << customer.name << ',' << customer.address << ',' << customer.phoneNumber << ',' << customer.roomNumber << ',' << customer.numDays << ',' << customer.totalPrice << '\n';
        }
        customerFile.close();

        std::cout << "Customer deleted successfully!" << std::endl;
    } else {
        std::cout << "Customer with phone number " << customerPhoneNumber << " does not exist." << std::endl;
    }
}

/* void updateCustomer(std::unordered_map<std::string, Customer>& customers) {
    std::string customerPhoneNumber;
    std::cout << "Enter customer phone number to update: ";
    std::cin.ignore();
    std::getline(std::cin, customerPhoneNumber);

    auto customerIt = customers.find(customerPhoneNumber);
    if (customerIt != customers.end()) {
        Customer& customer = customerIt->second;

        std::cout << "Enter new customer name: ";
        std::getline(std::cin, customer.name);
        std::cout << "Enter new customer address: ";
        std::getline(std::cin, customer.address);
        std::cout << "Enter new customer room number: ";
        std::getline(std::cin, customer.roomNumber);
        std::cout << "Enter new number of days: ";
        std::cin >> customer.numDays;

        std::ofstream customerFile("customers.txt");
        for (const auto& pair : customers) {
            const Customer& c = pair.second;
            customerFile << c.name << ',' << c.address << ',' << c.phoneNumber << ',' << c.roomNumber << ',' << c.numDays << ',' << c.totalPrice << '\n';
        }
        customerFile.close();

        std::cout << "Customer updated successfully!" << std::endl;
    } else {
        std::cout << "Customer with phone number " << customerPhoneNumber << " does not exist." << std::endl;
    }
}
*/
void updateCustomer(std::unordered_map<std::string, Customer>& customers, std::unordered_map<std::string, Room>& rooms) {
    std::string customerPhoneNumber;
    std::cout << "Enter customer phone number to update: ";
    std::cin.ignore();
    std::getline(std::cin, customerPhoneNumber);

    auto customerIt = customers.find(customerPhoneNumber);
    if (customerIt != customers.end()) {
        Customer& customer = customerIt->second;

        std::cout << "Enter new customer name: ";
        std::getline(std::cin, customer.name);
        std::cout << "Enter new customer address: ";
        std::getline(std::cin, customer.address);

        std::string newRoomNumber;
        std::cout << "Enter new room number: ";
        std::getline(std::cin, newRoomNumber);

        auto roomIt = rooms.find(newRoomNumber);
        if (roomIt != rooms.end()) {
            Room& newRoom = roomIt->second;
            if (!newRoom.isAssigned) {
                Room& oldRoom = rooms[customer.roomNumber];
                oldRoom.isAssigned = false;

                newRoom.isAssigned = true;
                customer.roomNumber = newRoomNumber;

                std::ofstream customerFile("customers.txt");
                for (const auto& pair : customers) {
                    const Customer& c = pair.second;
                    customerFile << c.name << ',' << c.address << ',' << c.phoneNumber << ',' << c.roomNumber << ',' << c.numDays << ',' << c.totalPrice << '\n';
                }
                customerFile.close();

                std::ofstream roomFile("rooms.txt");
                for (const auto& pair : rooms) {
                    const Room& room = pair.second;
                    roomFile << room.roomNumber << ',' << room.roomType << ',' << room.price << ',' << room.isAssigned << '\n';
                }
                roomFile.close();

                std::cout << "Customer updated successfully!" << std::endl;
            } else {
                std::cout << "Room " << newRoomNumber << " is already assigned." << std::endl;
            }
        } else {
            std::cout << "Room " << newRoomNumber << " does not exist." << std::endl;
        }
    } else {
        std::cout << "Customer with phone number " << customerPhoneNumber << " does not exist." << std::endl;
    }
}





void displayCustomers(const std::unordered_map<std::string, Customer>& customers) {
    std::cout << "Customer Name    |  Address     |  Phone Number  |  Room Number  |  Num Days  |  Total Price" << std::endl;
    std::cout << "---------------------------------------------------------------------------------------------" << std::endl;
    for (const auto& pair : customers) {
        const Customer& customer = pair.second;
        std::cout << std::setw(15) << customer.name << "  |  " << std::setw(10) << customer.address << "  |  " << std::setw(12) << customer.phoneNumber << "  |  " << std::setw(10) << customer.roomNumber << "  |  " << std::setw(9) << customer.numDays << "  |  " << std::setw(11) << customer.totalPrice << std::endl;
    }
}

void displayRooms(const std::unordered_map<std::string, Room>& rooms) {
    std::cout << "Room Number  |  Room Type   |  Price   |  Assigned" << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    for (const auto& pair : rooms) {
        const Room& room = pair.second;
        std::cout << std::setw(11) << room.roomNumber << "  |  " << std::setw(10) << room.roomType << "  |  " << std::setw(6) << room.price << "  |  " << std::setw(8) << std::boolalpha << room.isAssigned << std::endl;
    }
}

void displayFood() {
    std::ifstream foodFile("food.txt");
    if (!foodFile) {
        std::cout << "No food orders found." << std::endl;
        return;
    }
    std::cout << "Customer Name  |  Room Number   |  Food Item   |  Price" << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;

    std::string line;
    while (std::getline(foodFile, line)) {
        std::stringstream ss(line);
        std::string customerName, roomNumber, foodItem;
        double price;
        std::getline(ss, customerName, ',');
        std::getline(ss, roomNumber, ',');
        std::getline(ss, foodItem, ',');
        ss >> price;
        std::cout << std::setw(13) << customerName << "  |  " << std::setw(12) << roomNumber << "  |  " << std::setw(10) << foodItem << "  |  " << std::setw(6) << price<< std::endl;

    }
    foodFile.close();
}


int main() {
    std::unordered_map<std::string, Room> rooms;
    std::unordered_map<std::string, Customer> customers;

    std::ifstream roomFile("rooms.txt");
    if (roomFile) {
        std::string line;
        while (std::getline(roomFile, line)) {
            std::stringstream ss(line);
            Room room;
            std::getline(ss, room.roomNumber, ',');
            std::getline(ss, room.roomType, ',');
            ss >> room.price;
            ss.ignore();
            ss >> room.isAssigned;
            rooms[room.roomNumber] = room;
        }
    }
    roomFile.close();

    std::ifstream customerFile("customers.txt");
    if (customerFile) {
        std::string line;
        while (std::getline(customerFile, line)) {
            std::stringstream ss(line);
            Customer customer;
            std::getline(ss, customer.name, ',');
            std::getline(ss, customer.address, ',');
            std::getline(ss, customer.phoneNumber, ',');
            std::getline(ss, customer.roomNumber, ',');
            ss >> customer.numDays;
            ss.ignore();
            ss >> customer.totalPrice;
            customers[customer.phoneNumber] = customer;
        }
    }
    customerFile.close();

    int choice;
    do {
        std::cout << "Hotel Management System\n";
        std::cout << "1. Add Room\n";
        std::cout << "2. Add Customer\n";
        std::cout << "3. Add Food\n";
        std::cout << "4. Delete Customer\n";
        std::cout << "5. Update Customer\n";
        std::cout << "6. Display Customers\n";
        std::cout << "7. Display Rooms\n";
        std::cout << "8. Display Food\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                addRoom(rooms);
                break;
            case 2:
                addCustomer(customers, rooms);
                break;
            case 3:
                addFood(customers);
                break;
            case 4:
                deleteCustomer(customers);
                break;
            case 5:
                updateCustomer(customers, rooms);
                break;
            case 6:
                displayCustomers(customers);
                break;
            case 7:
                displayRooms(rooms);
                break;
            case 8:
                displayFood();
                break;
            case 0:
                std::cout << "Exiting program." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }

        std::cout << std::endl;
    } while (choice != 0);

    return 0;
}
