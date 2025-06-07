// Autori:  xnovik03 - Novikova Veronika
//          xshevc02 - Shevchenko Anna
// IMS project - T8: Bydlení nebo správa budov
// Temata: Pracky
#ifndef STUDENT_H
#define STUDENT_H

#include <simlib.h>
#include <string>

// Convert seconds to HH:MM:SS format
std::string ConvertToClockFormat(double seconds);

class Student : public Process {
private:
    static int student_counter;   // Counter for unique student IDs
    int student_number;           // Unique number for each student
        bool AssignMachine(int &machine_number);

public:
    Student();                    // Constructor
    void Behavior() override;     // Define the behavior of a student
};

#endif
