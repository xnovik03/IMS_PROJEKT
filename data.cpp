#include "data.h"
#include <iostream>

// Parametry praček
bool MachineOccupied[MACHINE_COUNT] = {false};
bool MachineFault[MACHINE_COUNT] = {false};

// Statistiky
int rejected_students = 0;
int total_students = 0;
double total_waiting_time = 0.0;
double machine_busy_time[MACHINE_COUNT] = {0};
double simulation_time = 0.0;
double total_key_wait_time = 0.0;
int key_wait_count = 0;

// Inicializace zdrojů
Store LaundryMachines("Laundry Machines", MACHINE_COUNT);
Queue LaundryQueue("Laundry Queue");

// Histogramy
Histogram LaundryWaitTime("Laundry Waiting Time", 0, 1000, 90);
Histogram RoomTimeHistogram("Room Time", 0, 1000, 60);
Histogram QueueWaitTime("Queue Waiting Time", 0, 100, 50);

// Implementace třídy FaultProcess
FaultProcess::FaultProcess(int index)
    : machine_index(index)
{
    // Náhodně generujeme dobu opravy
    fault_duration = Uniform(MIN_REPAIR_TIME, MAX_REPAIR_TIME);
}

void FaultProcess::Behavior()
{
    MachineFault[machine_index] = true;
    std::cout << "Machine " << machine_index + 1
              << " is under repair for " << fault_duration / 3600
              << " hours at time: " << Time << " seconds.\n";

    // Simulace opravy
    Wait(fault_duration);

    MachineFault[machine_index] = false;
    std::cout << "Machine " << machine_index + 1
              << " has been repaired at time: " << Time << " seconds.\n";
}
