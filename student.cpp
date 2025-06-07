// Autori:  xnovik03 - Novikova Veronika
//          xshevc02 - Shevchenko Anna
// IMS project - T8: Bydlení nebo správa budov
// Temata: Pracky

#include "student.h"
#include "data.h"
#include <iostream>
#include <iomanip>
#include <sstream>

#include <random> // Include for random selection
#include <vector>
#include <queue> // For queue management

// Define probabilities for each configuration
std::vector<double> probabilities = {0.1515, 0.2727, 0.1212, 0.0606, 0.2727, 0.0909, 0.0303};

// Define washing and drying times (in seconds)
std::vector<int> washing_times = {25 * 60, 33 * 60, 39 * 60, 41 * 60}; // Convert to seconds
std::vector<int> drying_times = {15 * 60, 22 * 60, 29 * 60, 39 * 60, 42 * 60};

// Function to randomly select an index based on probabilities
int selectConfiguration(const std::vector<double> &probabilities) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> dist(probabilities.begin(), probabilities.end());
    return dist(gen);
}

// Randomly select a time from a given vector
int selectTime(const std::vector<int> &times) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, times.size() - 1);
    return times[dist(gen)];
}

// Inicializace počítadla studentů
int Student::student_counter = 0;

// Převod času na formát HH:MM:SS
std::string ConvertToClockFormat(double seconds) {
    int hours = static_cast<int>(seconds) / 3600;
    int minutes = (static_cast<int>(seconds) % 3600) / 60;
    int secs = static_cast<int>(seconds) % 60;

    std::ostringstream clockTime;
    clockTime << std::setw(2) << std::setfill('0') << hours << ":"
              << std::setw(2) << std::setfill('0') << minutes << ":"
              << std::setw(2) << std::setfill('0') << secs;
    return clockTime.str();
}

// Konstruktor
Student::Student() {
    student_number = ++student_counter; // Assign a unique number to the student
}



void Student::Behavior() {
    double start_time = Time; // Start time of the student's process
    total_students++;         // Increment the total student count

    int machine_number = -1;  // Assigned machine number (-1 means not assigned)

    std::cout << "Student " << student_number << " created at time: "
              << ConvertToClockFormat(Time) << std::endl;

    // If outside work hours, the student leaves
    if (Time < START_WORK_TIME || Time > END_WORK_TIME) {
        rejected_students++;
        std::cout << "Student " << student_number
                  << " left due to laundry being closed at time: "
                  << ConvertToClockFormat(Time) << std::endl;
        return;
    }

    // Decide if the student arrives with or without items
    if (Random() < WITH_ITEMS_PROB) {
        // Student arrives directly to the laundry
        std::cout << "Student " << student_number << " arrived directly to the laundry.\n";
        Wait(TO_LAUNDRY_TIME);
    } else {
        // Student first goes to the room
        std::cout << "Student " << student_number << " goes to the room first.\n";
        Wait(TO_ROOM_TIME);
        Wait(ROOM_TIME);
        Wait(TO_LAUNDRY_TIME);
        RoomTimeHistogram(Time - start_time);
    }

    // Delay for entering the laundry
    Wait(VRATNIK_TIME);
    std::cout << "Student " << student_number
              << " entered the laundry at time: " << ConvertToClockFormat(Time) << ".\n";

    double queue_wait_start = Time; // Record the time the student starts waiting

    // Check if all machines are full
    if (LaundryMachines.Full()) {
        if (LaundryQueue.Length() >= MAX_QUEUE_SIZE) {
            rejected_students++;
            std::cout << "Student " << student_number
                      << " left due to full queue at time: "
                      << ConvertToClockFormat(Time) << ".\n";
            return;
        }

        // Add to the queue
        Into(LaundryQueue);
        std::cout << "Student " << student_number
                  << " entered the queue. Current queue length: "
                  << LaundryQueue.Length() << "\n";
        Passivate(); // Wait in the queue
        double queue_wait_time = Time - queue_wait_start;
        QueueWaitTime(queue_wait_time);
        std::cout << "Student " << student_number
                  << " waited in the queue for "
                  << queue_wait_time << " seconds.\n";
    }

    // Try to find an available machine
    LaundryMachines.Enter(this, 1);
    for (int i = 0; i < MACHINE_COUNT; ++i) {
        if (!MachineOccupied[i]) {
            MachineOccupied[i] = true;
            machine_number = i + 1;
            break;
        }
    }

    if (machine_number == -1) {
        std::cerr << "ERROR: No machine assigned to student "
                  << student_number << ".\n";
        return;
    }

    std::cout << "Student " << student_number << " got machine " << machine_number
              << " at time: " << ConvertToClockFormat(Time) << ".\n";

    // Laundry Process
    int config_index = selectConfiguration(probabilities);
    int num_wash_cycles = 0, num_dry_cycles = 0;

    switch (config_index) {
        case 0: num_wash_cycles = 2; num_dry_cycles = 2; break;
        case 1: num_wash_cycles = 1; num_dry_cycles = 1; break;
        case 2: num_wash_cycles = 2; num_dry_cycles = 0; break;
        case 3: num_wash_cycles = 2; num_dry_cycles = 1; break;
        case 4: num_wash_cycles = 1; num_dry_cycles = 0; break;
        case 5: num_wash_cycles = 3; num_dry_cycles = 0; break;
        case 6: num_wash_cycles = 3; num_dry_cycles = 2; break;
    }

    int wash_time = selectTime(washing_times);
    int dry_time = (num_dry_cycles > 0) ? selectTime(drying_times) : 0;

    // Perform wash cycles
    for (int i = 0; i < num_wash_cycles; ++i) {
        Wait(wash_time);
        std::cout << "Student " << student_number << " finished wash cycle "
                  << (i + 1) << " of " << num_wash_cycles
                  << " at time: " << ConvertToClockFormat(Time) << ".\n";
    }

    // Perform dry cycles
    for (int i = 0; i < num_dry_cycles; ++i) {
        Wait(dry_time);
        std::cout << "Student " << student_number << " finished dry cycle "
                  << (i + 1) << " of " << num_dry_cycles
                  << " at time: " << ConvertToClockFormat(Time) << ".\n";
    }

    // Release the machine
    LaundryMachines.Leave(1);
    MachineOccupied[machine_number - 1] = false;
    std::cout << "Student " << student_number << " finished using machine "
              << machine_number << " at time: " << ConvertToClockFormat(Time) << ".\n";

    // Simulate returning the key
    Wait(RETURN_KEY_TIME);
    std::cout << "Student " << student_number << " returned the key at time: "
              << ConvertToClockFormat(Time) << ".\n";

    // Activate the next student in the queue, if any
    if (!LaundryQueue.Empty()) {
        Process *next_student = dynamic_cast<Process *>(LaundryQueue.GetFirst());
        if (next_student) {
            next_student->Activate();
            std::cout << "Activated next student in the queue.\n";
        }
    }
}
