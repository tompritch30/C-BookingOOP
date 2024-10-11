#include <iostream>

using namespace std;

class Item{};
class Briefcase : public Item{};

class Pen : public Item{};

class Employee {
private:
    int number;
    Item *item;

    void checkin() {
        cout << "Employee " << number << " checking in" << endl;
    }

    void checkout() {
        cout << "Employee " << number << " checking out" << endl;
    }

protected:
    virtual void work() = 0;

public:
    Employee(int n, Item* i) : number(n), item(i) {
    }
    virtual ~Employee(){
        delete item;
    };

    virtual void daily_routine() {
        checkin();
        work();
        checkout();
    }
};


class Boss : public Employee {
private:
    void work() override {
        cout << "Signs papers" << endl;
    }

public:
    Boss(int n, Item* i) : Employee(n, i) {};
    ~Boss() override{};
};

class Worker : public Employee {
private:
    void work() override {
        cout << "Do useful work" << endl;
    }

public:
    Worker(int n, Item* i) : Employee(n, i) {};
    ~Worker() override{};
};


int main() {
    // Part a
//    Boss *b1 = new Boss(0);
//    Worker *w1 = new Worker(1);
//
//    b1->daily_routine();
//    w1->daily_routine();
//
//    delete b1;
//    delete w1;

    // Part b
//    Boss2 *b2 = new Boss2(0);
//    Worker2 *w2 = new Worker2(1);
//
//    b2->daily_routine();
//    w2->daily_routine();
//
//    delete b2;
//    delete w2;

    // Part c
    Briefcase *bc = new Briefcase;
    Employee *e1 = new Boss(0, bc);

    Pen *p = new Pen;
    Employee *e2 = new Worker(1, p);

    delete e1;
    delete e2;
}
