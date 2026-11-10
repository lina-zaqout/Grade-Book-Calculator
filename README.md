# Grade Book Calculator

A small C++ console program that calculates your **average**, **GPA**, and a summary of passed/failed courses for a semester.

## Features

- Enter the number of subjects and your total credit hours up front
- Credit hours per subject are validated so they always add up to your total
- Supports lecture and lab/tutorial subjects
- Prints a formatted table of all subjects with marks and letter grades
- Shows a summary: totals, passed/failed, highest/lowest mark, average, GPA
- Honor list message for an average of 90 or above
- Reminder to retake failed courses

## Build and Run

Requires a C++11 compiler (e.g. g++).

```
g++ -std=c++11 -o GradeCalculator GradeCalculator.cpp
./GradeCalculator
```

## How It Works

1. Choose whether to continue (`yes` / `no`).
2. Enter the number of subjects.
3. Enter your total credit hours.
4. For each subject, enter:
   - Name (up to 25 characters)
   - Type: `L` for lecture or `T` for lab/tutorial
   - Credit hours
   - Mark out of 100
5. View the results table and summary.

## Example Run

```
Hello! This program calculates your average and GPA.
Do you want to continue? (yes/no): yes
How many subjects do you want to calculate? 3
Enter your total credit hours: 12

--- Subject 1 of 3 (credit hours assigned: 0/12) ---
Subject name: Math
Type - (L)ecture or lab/(T)utorial? L
Credit hours (1-4): 4
Mark (out of 100): 92

--- Subject 2 of 3 (credit hours assigned: 4/12) ---
Subject name: Physics
Type - (L)ecture or lab/(T)utorial? L
Credit hours (1-4): 4
Mark (out of 100): 78

--- Subject 3 of 3 (credit hours assigned: 8/12) ---
Subject name: Chemistry Lab
Type - (L)ecture or lab/(T)utorial? T
Credit hours: 4 (set automatically to match your total)
Mark (out of 100): 85

+---------------+---------+-------+------+--------+
|    Subject    |  Type   | Hours | Mark | Letter |
+---------------+---------+-------+------+--------+
| Math          | Lecture |   4   |  92  |   A    |
| Physics       | Lecture |   4   |  78  |   C+   |
| Chemistry Lab | Lab/Tut |   4   |  85  |   B+   |
+---------------+---------+-------+------+--------+

============================================
                  SUMMARY
============================================
 Total credit hours  : 12
 Courses passed      : 3
 Courses failed      : 0
--------------------------------------------
 Highest mark        : 92   (Math)
 Lowest mark         : 78   (Physics)
--------------------------------------------
 Average             : 85.00
 GPA                 : 3.33
============================================
```

## Rules

| Rule | Value |
|------|-------|
| Minimum subjects | 3 |
| Maximum subjects | Depends on total hours (up to 21) |
| Total credit hours | 9 to 21 |
| Credit hours per lecture | 1 to 4 |
| Credit hours per lab/tutorial | 1 to 5 |
| Pass mark | 60 |
| Honor list average | 90 or above |

The allowed total depends on how many subjects you enter. Every subject needs at least 1 hour and at most 5, so for example 3 subjects can total 9 to 15 hours, and 15 subjects can total 15 to 21 hours.

The program also guides each subject's hours so the final total matches exactly. If only one value is possible (such as for the last subject), it is filled in automatically, and a subject that needs 5 hours must be a lab/tutorial.

## Grading Scale

| Mark | Letter | Points |
|------|--------|--------|
| 90 - 100 | A | 4.0 |
| 85 - 89 | B+ | 3.5 |
| 80 - 84 | B | 3.0 |
| 75 - 79 | C+ | 2.5 |
| 70 - 74 | C | 2.0 |
| 65 - 69 | D+ | 1.5 |
| 60 - 64 | D | 1.0 |
| Below 60 | F | 0.0 |

## Calculations

- **Average** = sum of all marks / number of subjects
- **GPA** = sum of (grade points x credit hours) / total credit hours

## Customizing

Limits are constants at the top of `GradeCalculator.cpp`, for example `MIN_COURSE_HOURS`, `MAX_TOTAL_HOURS`, and `PASS_MARK`. Change them and recompile.

## Author

Lina Zaqout

## License

Released under the [MIT License](LICENSE).
