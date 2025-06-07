// Autori:  xnovik03 - Novikova Veronika
//          xshevc02 - Shevchenko Anna
// IMS project - T8: Bydlení nebo správa budov
// Temata: Pracky

#include <iostream>
#include <simlib.h>
#include "student.h"
#include "data.h"
#include "generator.h"
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;

// Struktura pro výsledky experimentu
struct ExperimentResult
{
    std::string description;    // Popis experimentu
    double avg_wait_time;       // Průměrná čekací doba
    int rejected_students;      // Počet odmítnutých studentů
    double machine_utilization; // Průměrné využití praček
};

// Simulační parametry
int TotalStudents = 0;
int RejectedStudents = 0;
double TotalWaitTime = 0.0;
double TotalMachineBusyTime = 0.0;
int NumberOfMachines = 5;
int SimulationStart = 0;
int SimulationEnd = 86400;       // 24 hodin
double ArrivalProbability = 0.5; // Pravděpodobnost příchodu s věcmi
// bool MachineFault[MACHINE_COUNT] = {false}; // Pole pro poruchy praček

// Funkce pro resetování statistik
void ResetStats()
{
    TotalStudents = 0;
    RejectedStudents = 0;
    TotalWaitTime = 0.0;
    TotalMachineBusyTime = 0.0;
}

// Simulace jednoho experimentu
ExperimentResult RunExperiment(std::string description, int start_time, int end_time, int machines, double arrival_prob, bool fault)
{
    // Nastavení parametrů simulace
    SimulationStart = start_time;
    SimulationEnd = end_time;
    NumberOfMachines = machines;
    ArrivalProbability = arrival_prob;

    // Nastavení poruchy
    for (int i = 0; i < MACHINE_COUNT; i++)
    {
        MachineFault[i] = (i == 0) && fault; // Nastaví první pračku jako poruchovou pokud je `fault` true
    }

    // Inicializace simulace
    ResetStats();
    Init(SimulationStart, SimulationEnd);

    // Spuštění simulace
    Run();

    // Výpočet výsledků
    double avg_wait_time = TotalWaitTime / TotalStudents;
    double utilization = TotalMachineBusyTime / (NumberOfMachines * (SimulationEnd - SimulationStart));

    return {description, avg_wait_time, RejectedStudents, utilization};
}

int main()
{
    RandomSeed(time(nullptr)); // Zajišťuje, že výsledky simulace budou pokaždé jiné

    // Experimenty pro různé parametry
    std::vector<ExperimentResult> results;

    // Experiment 1: Dopad prodloužení pracovní doby
    results.push_back(RunExperiment(
        "Pracovní doba 6:00 - 22:00", 6 * 3600, 22 * 3600, 5, 0.5, false));
    results.push_back(RunExperiment(
        "Pracovní doba 7:00 - 22:00", 7 * 3600, 22 * 3600, 5, 0.5, false));

    // Experiment 2: Zvýšení kapacity praček
    results.push_back(RunExperiment(
        "6 praček, 7:00 - 22:00", 7 * 3600, 22 * 3600, 6, 0.5, false));
    results.push_back(RunExperiment(
        "7 praček, 7:00 - 22:00", 7 * 3600, 22 * 3600, 7, 0.5, false));

    // Experiment 3: Pravděpodobnost příchodu studentů
    results.push_back(RunExperiment(
        "20% příchod připravených", 7 * 3600, 22 * 3600, 5, 0.2, false));
    results.push_back(RunExperiment(
        "50% příchod připravených", 7 * 3600, 22 * 3600, 5, 0.5, false));
    results.push_back(RunExperiment(
        "80% příchod připravených", 7 * 3600, 22 * 3600, 5, 0.8, false));

    // Experiment 4: Porucha pračky
    results.push_back(RunExperiment(
        "Porucha 1 pračky", 7 * 3600, 22 * 3600, 4, 0.5, true));

    // Aktivace simulace
    Init(7 * 3600, 22 * 3600);         // Simulace od 7:00 do 22:00
    (new Generator)->Activate();       // Aktivace generátoru studentů
    (new FaultProcess(0))->Activate(); // Aktivace simulace poruchy první pračky
    Run();                             // Spuštění simulace

    // Výpis statistik
    std::cout << "\nSimulation Statistics:\n";
    std::cout << "Total students: " << total_students << "\n";
    std::cout << "Rejected students: " << rejected_students << "\n";

    // Výstup statistik
    LaundryWaitTime.Output();
    RoomTimeHistogram.Output();
    QueueWaitTime.Output();

    return 0;
}
