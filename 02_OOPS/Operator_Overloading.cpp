#include <iostream>
#include <string>
using namespace std;

class Money {
private:
    int rupees;
    int paise;

public:
    Money(int r = 0, int p = 0) {
        rupees = r + p / 100;
        paise = p % 100;
    }

    // Operator overloading for addition
    Money operator+(const Money& other) const {
        int totalPaise = paise + other.paise;
        int totalRupees = rupees + other.rupees + totalPaise / 100;
        int remainingPaise = totalPaise % 100;

        return Money(totalRupees, remainingPaise);
    }

    // Equality operator
    bool operator==(const Money& other) const {
        return (rupees == other.rupees && paise == other.paise);
    }

    // Output stream operator
    friend ostream& operator<<(ostream& out, const Money& m) {
        out << "$" << m.rupees << "." << (m.paise < 10 ? "0" : "") << m.paise;
        return out;
    }
};

int main() {
    // Dynamically allocating objects using pointers
    Money* m1 = new Money(5, 75);  // $5.75
    Money* m2 = new Money(3, 50);  // $3.50

    // Using overloaded + operator
    Money* total = new Money(*m1 + *m2); // Dereference to get values

    // Printing result using overloaded <<
    cout << "Total: " << *total << endl;

    // Checking equality
    if (*total == Money(9, 25)) {
        cout << "Equal to $9.25" << endl;
    } else {
        cout << "Not equal" << endl;
    }

    // Free memory
    delete m1;
    delete m2;
    delete total;

    return 0;
}
