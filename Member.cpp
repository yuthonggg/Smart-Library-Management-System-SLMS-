#include "Member.h"
#include <iostream>
#include <iomanip>


string formatTime(time_t t) {
    if (t == 0) return "N/A";
    char buffer[26];
    ctime_s(buffer, sizeof(buffer), &t);
    string s(buffer);
    if (!s.empty() && s.back() == '\n') s.pop_back();
    return s;
}

Member::Member(string name, string id, string email) : Person(name, id, email) {}

string Member::getRole() const {
    return "Member";
}

void Member::displayInfo() const {
    Person::displayInfo();
    cout << "Role: Member" << endl;
    cout << "Borrowing History:" << endl;

    Node<BorrowRecord>* current = borrowHistory.getHead();
    if (!current) {
        cout << "  No history." << endl;
    }
    while (current) {
        BorrowRecord& rec = current->data;
        cout << "  BookID: " << rec.bookID
             << " | Borrowed: " << formatTime(rec.borrowDate)
             << " | Due: " << formatTime(rec.dueDate)
             << " | Returned: " << (rec.isReturned ? formatTime(rec.returnDate) : "No")
             << endl;
        current = current->next;
    }
}

void Member::addBorrowRecord(BorrowRecord record) {
    borrowHistory.add(record);
}

LinkedList<BorrowRecord>& Member::getBorrowHistory() {
    return borrowHistory;
}
