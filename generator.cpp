#include "generator.h"
#include "student.h"
#include "data.h"
#include <iostream>
#include <cmath>
#include "simlib.h" // Tento řádek je nutný, pokud není zahrnut v generator.h

const double ONE_DAY = 86400; // Jeden den v sekundách

// Funkce na výpočet začátku dalšího pracovního dne
double GetNextWorkStartTime(double currentTime)
{
    // Začátek dalšího pracovního dne
    return START_WORK_TIME + ONE_DAY * floor(currentTime / ONE_DAY) + ONE_DAY;
}

void Generator::Behavior()
{
    while (true)
    {
        // Pokud je aktuální čas mimo pracovní dobu, generátor přestane generovat
        if (Time >= END_WORK_TIME || Time < START_WORK_TIME)
        {
            double next_day_start = GetNextWorkStartTime(Time);
            // std::cout << "Generator scheduled for next work day at: "
            //           << ConvertToClockFormat(next_day_start) << " ("
            //           << next_day_start << " seconds).\n";
            Wait(next_day_start - Time); // Čekání do začátku dalšího pracovního dne
            continue;
        }

        // Vytvoření studenta
        (new Student)->Activate();

        // Čekání na další interval generování
        double next_interval = Uniform(5 * 60, 40 * 60); // Interval 5–40 minut
        if (Time + next_interval >= END_WORK_TIME)
        {
            double remaining_time = END_WORK_TIME - Time;
            std::cout << "Next interval adjusted to end of work hours: "
                      << ConvertToClockFormat(Time + remaining_time) << "\n";
            Wait(remaining_time); // Čekání na konec pracovního dne
        }
        else
        {
            Wait(next_interval); // Čekání do dalšího generování
        }
    }
}
