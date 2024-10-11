#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <cmath>
#include <set>
#include <cassert>

using namespace std;

class DateTime {
private:
    int day, month, year, hour, minute;

    double days_since_01011970() const {
        // exploits C function mktime ("man mktime" for details)
        struct tm t;
        t.tm_mday = day;
        t.tm_mon = month - 1;
        t.tm_year = year - 1900;
        t.tm_sec = 0;
        t.tm_min = minute;
        t.tm_hour = hour;
        time_t t_seconds = mktime(&t);
        return t_seconds / 86400.0;
    }

public:
    DateTime(int d, int m, int y, int hr = 0, int mn = 0) {
        day = d;
        month = m;
        year = y;
        hour = hr;
        minute = mn;
    }

    // copy constructor
    DateTime(const DateTime &other) : day(other.day), month(other.month), year(other.year), hour(other.hour),
                                      minute(other.minute) {
    }

    // make global operator for outputting dates to streams
    // a friend of the class
    friend ostream &operator<<(ostream &out, const DateTime &d);

    //  operator belonging to class for subtracting dates
    double operator-(const DateTime &other) const {
        return days_since_01011970() - other.days_since_01011970();
    }

    static DateTime now() {
        time_t rawtime;
        time(&rawtime);
        struct tm *t = localtime(&rawtime);
        assert(t);
        return DateTime(t->tm_mday, t->tm_mon + 1, t->tm_year + 1900, t->tm_hour, t->tm_min);
    }
};

ostream &operator<<(ostream &out, const DateTime &d) {
    out << d.day << "/" << d.month << "/" << d.year << " " << setw(2) << setfill('0') << d.hour << ":" << setw(2)
        << d.minute << endl;;
    return out;
}

/* part a: write your declarations for Agent, TrainingCourse and Booking classes here */
class Agent;

class TrainingCourse;

class Booking {
private:
    Agent *agent;
public:
    Agent *getAgent() const;

private:
    TrainingCourse *training_course;
public:
    TrainingCourse *getTrainingCourse() const;

    Booking(Agent *_agent, TrainingCourse *_training_course);

    bool cancel(Agent *agt);
};

class Agent {
private:
    string code_name;
    int security_level;
    set<Booking *> bookings;
public:
    Agent(const string &_code_name, int _security_level);

    Booking *book(TrainingCourse &tc);

    bool cancel(Booking &r);

    bool attend(TrainingCourse &tc);

    int getSecurityLevel() const;


};

class TrainingCourse {
private:
    string title;
    unsigned int capacity;
    int min_security_level;
    DateTime starts_at;
    set<Agent *> attendees;
public:
    TrainingCourse(string _title, unsigned int _capacity, int _min_security_level, const DateTime &start_time);

    bool isFull();

    Booking *book(Agent &agt);

    bool cancel(Agent &agt);

    const DateTime &getStart() const;

};


/* part b: write your definitions for Agent, TrainingCourse and Booking methods here */
/////////////////////////////////
Booking::Booking(Agent *_agent, TrainingCourse *_training_course) :
        agent(_agent), training_course(_training_course) {
}

bool Booking::cancel(Agent *agt) {
    delete this;
    return true;
}

TrainingCourse *Booking::getTrainingCourse() const {
    return training_course;
}

Agent *Booking::getAgent() const {
    return agent;
}

/////////////////////////////////
Agent::Agent(const string &_code_name, int _security_level) :
    code_name(_code_name), security_level(_security_level) {
}

int Agent::getSecurityLevel() const {
    return security_level;
}

Booking *Agent::book(TrainingCourse &tc) {
    Booking* booking = tc.book(*this);
    if (booking){
        bookings.insert(booking);
    }
    return booking;
}

bool Agent::attend(TrainingCourse &tc) {
    if (DateTime::now() - tc.getStart() > 0) {
        return false;
    }
    for (const auto &booking: bookings) {
        if (booking->getTrainingCourse() == &tc) {
            return true;
        }
    }
    return false;
}

bool Agent::cancel(Booking &r) {
    if (r.getAgent() != this){
        return false;
    }
    if (DateTime::now() - r.getTrainingCourse()->getStart() > 0){
        return false;
    }
    r.getTrainingCourse()->cancel(*this);
    r.cancel(this);
    return true;
}


/////////////////////////////////
TrainingCourse::TrainingCourse(string _title, unsigned int _capacity, int _min_security_level, const DateTime &start_time)
        : title(_title), capacity(_capacity), min_security_level(_min_security_level),
          starts_at(start_time) {
}

const DateTime &TrainingCourse::getStart() const {
    return starts_at;
}

bool TrainingCourse::isFull() {
    return attendees.size() == capacity;
}

Booking *TrainingCourse::book(Agent &agt) {
    if (agt.getSecurityLevel() < min_security_level) {
        cout << "Security level too low!" << endl;
        return nullptr;
    }
    if (DateTime::now() - getStart() > 0) {
        cout << "Too late to book!" << endl;
        return nullptr;
    }
    if (isFull()) {
        cout << "Course is full!" << endl;
        return nullptr;
    }
    attendees.insert(&agt);
    Booking* new_booking = new Booking(&agt, this);
    return new_booking;
}

bool TrainingCourse::cancel(Agent &agt) {
    attendees.erase(&agt);
    return true;
}


/* part c: write your main function here */

int main() {
    DateTime start = DateTime(10, 6, 2024, 19, 0);
    TrainingCourse advancedHacking = TrainingCourse("Advanced Hacking", 30, 1, start);
    Agent bond = Agent("007", 2);
    Agent will = Agent("Bald Eagle", 0);

    Booking *bond_booking = bond.book(advancedHacking);
    will.book(advancedHacking);
    bond.cancel(*bond_booking);

    cout << "Can";
    if (!bond.attend(advancedHacking)) {
        cout << "not";
    };
    cout << " attend the course";

    return 0;
}
