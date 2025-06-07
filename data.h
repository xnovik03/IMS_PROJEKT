#ifndef DATA_H
#define DATA_H
#define MAX_QUEUE_SIZE 4

#include "simlib.h"

// Constants
const double VRATNIK_TIME = 53;           // Gatekeeper time (seconds)
const double ROOM_TO_LAUNDRY_TIME = 127;  // Room -> Laundry (seconds)
const double RETURN_KEY_TIME = 52;        // Key return time (seconds)
const double TO_ROOM_TIME = 119;          // Gatekeeper -> Room (seconds)
const double ROOM_TIME = 185;             // Time in the room (3 minutes 5 seconds)
const double TO_LAUNDRY_TIME = 127;       // Room -> Laundry (seconds)

// Probabilities
const double WITH_ITEMS_PROB = 0.22;      // Probability of student having items
const double WITHOUT_ITEMS_PROB = 0.78;   // Probability of student without items

// Resources and queues
extern Store LaundryMachines;             // Washing machines (5 machines)
extern Queue LaundryQueue;                // Queue for washing machines

const int MACHINE_COUNT = 5;              // Number of washing machines
extern bool MachineOccupied[MACHINE_COUNT]; // Machine status (true = occupied, false = free)
extern bool MachineFault[MACHINE_COUNT];    // Machine status (true = faulty, false = functional)
// Minimum and maximum repair time for a washing machine
const double MIN_REPAIR_TIME = 24 * 3600;  // 24 hours in seconds
const double MAX_REPAIR_TIME = 14 * 24 * 3600; // 2 weeks in seconds

// Tracking statistics
extern int rejected_students;             // Number of students who left the system without washing
extern int total_students;                // Total number of students who entered the system
extern double total_waiting_time;         // Sum of all waiting times in the queue
extern double machine_busy_time[MACHINE_COUNT]; // Array to track busy time for each machine
extern double simulation_time;            // Total simulation time
extern double total_key_wait_time;        // Total time students waited for keys
extern int key_wait_count;                // Total number of students who waited for keys

const double START_WORK_TIME = 7 * 3600;  // 7:00 AM
const double END_WORK_TIME = 22 * 3600;   // 10:00 PM

const double ARRIVAL_RATE = 3600 / 10.0;  // 1 arrival every 6 minutes (in seconds)

// Statistics
extern Histogram LaundryWaitTime;
extern Histogram RoomTimeHistogram;
extern Histogram QueueWaitTime;

// Declaration of the FaultProcess class
class FaultProcess : public Process {
public:
    FaultProcess(int machine_index);
    void Behavior() override;

private:
    int machine_index;
    double fault_duration;
};
#endif
