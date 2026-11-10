#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdlib>
using namespace std;

const int MIN_SUBJECTS = 3;
const int MIN_TOTAL_HOURS = 9;
const int MAX_TOTAL_HOURS = 21;
const int MIN_COURSE_HOURS = 1;
const int MAX_SUBJECTS = MAX_TOTAL_HOURS / MIN_COURSE_HOURS;
const int MAX_LECTURE_HOURS = 4;
const int MAX_LAB_HOURS = 5;
const int MAX_NAME_LENGTH = 25;
const int PASS_MARK = 60;
const int HONOR_AVG = 90;

struct Subject {
    string name;
    bool   isLab;
    int    mark;
    string letter;
    int    hours;
};

string markToLetter(int mark) {
    if (mark >= 90) return "A";
    if (mark >= 85) return "B+";
    if (mark >= 80) return "B";
    if (mark >= 75) return "C+";
    if (mark >= 70) return "C";
    if (mark >= 65) return "D+";
    if (mark >= 60) return "D";
    return "F";
}

double letterToPoint(const string& letter) {
    if (letter == "A")  return 4.0;
    if (letter == "B+") return 3.5;
    if (letter == "B")  return 3.0;
    if (letter == "C+") return 2.5;
    if (letter == "C")  return 2.0;
    if (letter == "D+") return 1.5;
    if (letter == "D")  return 1.0;
    return 0.0;
}

string center(const string& text, int width) {
    int space = width - (int)text.size();
    if (space < 0) space = 0;
    int left = space / 2;
    return string(left, ' ') + text + string(space - left, ' ');
}

string padRight(const string& text, int width) {
    int space = width - (int)text.size();
    if (space < 0) space = 0;
    return text + string(space, ' ');
}

void printRow(const string& label, const string& value) {
    cout << " " << padRight(label, 20) << ": " << value << "\n";
}

void endOfInput() {
    cout << "\nInput ended. Goodbye!\n";
    exit(0);
}

int readInt(const string& prompt, int min, int max) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= min && value <= max) {
            cin.ignore(10000, '\n');
            return value;
        }
        if (cin.eof()) endOfInput();
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Please enter a whole number between " << min << " and " << max << ".\n";
    }
}

int readSubjectCount() {
    int value;
    while (true) {
        cout << "How many subjects do you want to calculate? ";
        if (cin >> value) {
            cin.ignore(10000, '\n');
            if (value < MIN_SUBJECTS) {
                cout << "You need to have at least " << MIN_SUBJECTS << " courses.\n";
                continue;
            }
            if (value > MAX_SUBJECTS) {
                cout << "You can not add this many courses (max " << MAX_SUBJECTS
                     << ", since the total is capped at " << MAX_TOTAL_HOURS
                     << " credit hours and a course needs at least " << MIN_COURSE_HOURS << ").\n";
                continue;
            }
            return value;
        }
        if (cin.eof()) endOfInput();
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Please enter a whole number.\n";
    }
}

int readTotalHours(int subjectCount) {
    int lo = MIN_TOTAL_HOURS;
    if (subjectCount * MIN_COURSE_HOURS > lo) lo = subjectCount * MIN_COURSE_HOURS;
    int hi = MAX_TOTAL_HOURS;
    if (subjectCount * MAX_LAB_HOURS < hi) hi = subjectCount * MAX_LAB_HOURS;
    return readInt("Enter your total credit hours: ", lo, hi);
}

bool askYesNo(const string& prompt) {
    string answer;
    while (true) {
        cout << prompt;
        if (!(cin >> answer)) endOfInput();
        cin.ignore(10000, '\n');
        if (answer == "yes" || answer == "Yes" || answer == "YES" ||
            answer == "y"   || answer == "Y")  return true;
        if (answer == "no"  || answer == "No"  || answer == "NO"  ||
            answer == "n"   || answer == "N")  return false;
        cout << "Please answer yes or no.\n";
    }
}

string readName(const string& prompt) {
    string name;
    while (true) {
        cout << prompt;
        if (!getline(cin, name)) endOfInput();

        size_t first = name.find_first_not_of(" \t\r");
        if (first == string::npos) {
            cout << "The name cannot be empty.\n";
            continue;
        }
        size_t last = name.find_last_not_of(" \t\r");
        name = name.substr(first, last - first + 1);

        if ((int)name.size() > MAX_NAME_LENGTH) {
            cout << "Please keep the name under " << MAX_NAME_LENGTH + 1 << " characters.\n";
            continue;
        }
        return name;
    }
}

bool readIsLab(int minHoursNeeded) {
    string answer;
    while (true) {
        cout << "Type - (L)ecture or lab/(T)utorial? ";
        if (!(cin >> answer)) endOfInput();
        cin.ignore(10000, '\n');
        bool lecture = (answer == "L" || answer == "l" || answer == "lecture" || answer == "Lecture");
        bool lab = (answer == "T" || answer == "t" || answer == "tutorial" || answer == "Tutorial" ||
                    answer == "lab" || answer == "Lab");
        if (lecture) {
            if (minHoursNeeded > MAX_LECTURE_HOURS) {
                cout << "This subject needs " << minHoursNeeded << " hours to reach your total, "
                     << "but a lecture is max " << MAX_LECTURE_HOURS << ". It must be a lab/tutorial.\n";
                continue;
            }
            return false;
        }
        if (lab) return true;
        cout << "Please enter L for lecture or T for lab/tutorial.\n";
    }
}

int readHours(int typeMax, int minAllowed, int maxAllowed) {
    while (true) {
        int hours = readInt("Credit hours (" + to_string(MIN_COURSE_HOURS) + "-" + to_string(typeMax) + "): ",
                            MIN_COURSE_HOURS, typeMax);
        if (hours >= minAllowed && hours <= maxAllowed) return hours;

        cout << "To match your total credit hours, this subject needs ";
        if (minAllowed == maxAllowed) cout << "exactly " << minAllowed << " hours.\n";
        else cout << "between " << minAllowed << " and " << maxAllowed << " hours.\n";
    }
}

class GradeBook {
private:
    Subject* subjects;
    int      count;
    int      capacity;
    int      totalHours;

    GradeBook(const GradeBook&);
    GradeBook& operator=(const GradeBook&);

public:
    explicit GradeBook(int n)
        : subjects(new Subject[n]), count(0), capacity(n), totalHours(0) {}

    ~GradeBook() {
        delete[] subjects;
    }

    void addSubject(const string& name, bool isLab, int hours, int mark) {
        if (count >= capacity) return;
        subjects[count].name   = name;
        subjects[count].isLab  = isLab;
        subjects[count].hours  = hours;
        subjects[count].mark   = mark;
        subjects[count].letter = markToLetter(mark);
        totalHours += hours;
        count++;
    }

    int size() const { return count; }
    int getTotalHours() const { return totalHours; }
    const Subject& get(int i) const { return subjects[i]; }

    double getAverage() const {
        int sum = 0;
        for (int i = 0; i < count; i++) sum += subjects[i].mark;
        return (double)sum / count;
    }

    int countFailed() const {
        int failed = 0;
        for (int i = 0; i < count; i++) {
            if (subjects[i].mark < PASS_MARK) failed++;
        }
        return failed;
    }

    int countPassed() const { return count - countFailed(); }

    int highestIndex() const {
        int best = 0;
        for (int i = 1; i < count; i++) {
            if (subjects[i].mark > subjects[best].mark) best = i;
        }
        return best;
    }

    int lowestIndex() const {
        int worst = 0;
        for (int i = 1; i < count; i++) {
            if (subjects[i].mark < subjects[worst].mark) worst = i;
        }
        return worst;
    }

    double getGpa() const {
        double totalPoints = 0.0;
        for (int i = 0; i < count; i++) {
            totalPoints += letterToPoint(subjects[i].letter) * subjects[i].hours;
        }
        if (totalHours == 0) return 0.0;
        return totalPoints / totalHours;
    }

    void printTable() const {
        int nameWidth = 7;
        for (int i = 0; i < count; i++) {
            if ((int)subjects[i].name.size() > nameWidth) nameWidth = (int)subjects[i].name.size();
        }

        string line = "+-" + string(nameWidth, '-') + "-+---------+-------+------+--------+";

        cout << "\n" << line << "\n";
        cout << "| " << center("Subject", nameWidth) << " |  Type   | Hours | Mark | Letter |\n";
        cout << line << "\n";
        for (int i = 0; i < count; i++) {
            cout << "| " << padRight(subjects[i].name, nameWidth)
                 << " | " << center(subjects[i].isLab ? "Lab/Tut" : "Lecture", 7)
                 << " | " << center(to_string(subjects[i].hours), 5)
                 << " | " << center(to_string(subjects[i].mark), 4)
                 << " | " << center(subjects[i].letter, 6) << " |\n";
        }
        cout << line << "\n";
    }
};

int main() {
    cout << fixed << setprecision(2);

    cout << "Hello! This program calculates your average and GPA.\n";

    if (!askYesNo("Do you want to continue? (yes/no): ")) {
        cout << "Thanks for using the application :)\n";
        return 0;
    }

    int n = readSubjectCount();
    int totalHoursGoal = readTotalHours(n);

    GradeBook book(n);

    for (int i = 0; i < n; i++) {
        int left      = n - i - 1;
        int remaining = totalHoursGoal - book.getTotalHours();

        cout << "\n--- Subject " << (i + 1) << " of " << n << " (credit hours assigned: "
             << book.getTotalHours() << "/" << totalHoursGoal << ") ---\n";

        int minHours = remaining - left * MAX_LAB_HOURS;
        if (minHours < MIN_COURSE_HOURS) minHours = MIN_COURSE_HOURS;
        int maxHours = remaining - left * MIN_COURSE_HOURS;

        string name = readName("Subject name: ");
        bool isLab  = readIsLab(minHours);

        int typeMax = isLab ? MAX_LAB_HOURS : MAX_LECTURE_HOURS;
        if (maxHours > typeMax) maxHours = typeMax;

        int hours;
        if (minHours == maxHours) {
            hours = minHours;
            cout << "Credit hours: " << hours << " (set automatically to match your total)\n";
        } else {
            hours = readHours(typeMax, minHours, maxHours);
        }
        int mark = readInt("Mark (out of 100): ", 0, 100);

        book.addSubject(name, isLab, hours, mark);
    }

    book.printTable();

    int failed = book.countFailed();
    int passed = book.countPassed();

    int high = book.highestIndex();
    int low  = book.lowestIndex();

    string border = string(44, '=');
    string divider = string(44, '-');

    cout << "\n" << border << "\n";
    cout << center("SUMMARY", 44) << "\n";
    cout << border << "\n";
    printRow("Total credit hours", to_string(book.getTotalHours()));
    printRow("Courses passed", to_string(passed));
    printRow("Courses failed", to_string(failed));
    cout << divider << "\n";
    printRow("Highest mark", padRight(to_string(book.get(high).mark), 3) + "  (" + book.get(high).name + ")");
    printRow("Lowest mark", padRight(to_string(book.get(low).mark), 3) + "  (" + book.get(low).name + ")");
    cout << divider << "\n";

    ostringstream avg, gpa;
    avg << fixed << setprecision(2) << book.getAverage();
    gpa << fixed << setprecision(2) << book.getGpa();
    printRow("Average", avg.str());
    printRow("GPA", gpa.str());
    cout << border << "\n\n";

    if (book.getAverage() >= HONOR_AVG) {
        cout << "You made it to the honor list!! Congratulations <3\n";
    }

    if (failed > 0) {
        cout << "Note: you must retake the failed course(s).\n";
    }

    return 0;
}