#include <iostream>
#include <vector>
#include <string>
#include <sqlite3.h>
#include<Windows.h>
#include <regex>

using namespace std;

// ��������� ��� �������� ������ � ������������
struct Employer {
    int id;
    string name;
    string activity_type;
    string address;
    string phone;
};

// ��������� ��� �������� ������ � ����������
struct Applicant {
    int id;
    string last_name;
    string first_name;
    string middle_name;
    string qualification;
    string profession;
    string other_data;
    double expected_salary;
};

// ��������� ��� �������� ������ � ������
struct Deal {
    int applicant_id;
    int employer_id;
    string position;
    double commission;
    string contract_date; // ��������� ���� ��� ���� ��������
};

// ��������� ����� ������
string input_string(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (!input.empty()) break;
        cout << "������: ���� �� ����� ���� ������. ���������� �����." << endl;
    }
    return input;
}

// ��������� ����� ��������� �������� (������ �����)
int input_int(const string& prompt) {
    int input;
    while (true) {
        cout << prompt;
        cin >> input;
        if (cin.fail()) {
            cin.clear(); // ������� ��������� ������
            cin.ignore((numeric_limits<streamsize>::max)(), '\n'); // ������� ����������� �����
            cout << "������: ������� ���������� ����� �����." << endl;
        }
        else {
            cin.ignore((numeric_limits<streamsize>::max)(), '\n'); // ������� ����������� �����
            return input;
        }
    }
}

// ��������� ����� ����� � ��������� ������
double input_double(const string& prompt) {
    double input;
    while (true) {
        cout << prompt;
        cin >> input;
        if (cin.fail()) {
            cin.clear();
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            cout << "������: ������� ���������� �����." << endl;
        }
        else {
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            return input;
        }
    }
}

// ��������� ����� ���� � ������� YYYY-MM-DD
string input_date(const string& prompt) {
    string date;
    regex date_regex("\\d{4}-\\d{2}-\\d{2}");
    while (true) {
        cout << prompt;
        getline(cin, date);
        if (regex_match(date, date_regex)) break;
        cout << "������: ������� ���������� ���� � ������� YYYY-MM-DD." << endl;
    }
    return date;
}

// ������� ��� �������� ������ � ���� ������
void create_tables(sqlite3* db) {
    const char* sql_employers = R"(
        CREATE TABLE IF NOT EXISTS Employers (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            activity_type TEXT NOT NULL,
            address TEXT NOT NULL,
            phone TEXT NOT NULL
        );
    )";

    const char* sql_applicants = R"(
        CREATE TABLE IF NOT EXISTS Applicants (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            last_name TEXT NOT NULL,
            first_name TEXT NOT NULL,
            middle_name TEXT,
            qualification TEXT NOT NULL,
            profession TEXT NOT NULL,
            other_data TEXT,
            expected_salary REAL NOT NULL
        );
    )";

    const char* sql_deals = R"(
        CREATE TABLE IF NOT EXISTS Deals (
            applicant_id INTEGER NOT NULL,
            employer_id INTEGER NOT NULL,
            position TEXT NOT NULL,
            commission REAL NOT NULL,
            contract_date DATE NOT NULL,
            FOREIGN KEY(applicant_id) REFERENCES Applicants(id),
            FOREIGN KEY(employer_id) REFERENCES Employers(id)
        );
    )";

    sqlite3_exec(db, sql_employers, 0, 0, 0);
    sqlite3_exec(db, sql_applicants, 0, 0, 0);
    sqlite3_exec(db, sql_deals, 0, 0, 0);
}

// ������� ��� ���������� ������ ������������
void add_employer(sqlite3* db) {
    Employer employer;
    employer.name = input_string("������� �������� ������������: ");
    employer.activity_type = input_string("������� ��� ������������ ������������: ");
    employer.address = input_string("������� ����� ������������: ");
    employer.phone = input_string("������� ������� ������������: ");

    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO Employers (name, activity_type, address, phone) VALUES (?, ?, ?, ?);";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, employer.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, employer.activity_type.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, employer.address.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, employer.phone.c_str(), -1, SQLITE_STATIC);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    cout << "������������ ��������!" << endl;
}

// ������� ��� ���������� ������ ����������
void add_applicant(sqlite3* db) {
    Applicant applicant;
    applicant.last_name = input_string("������� ������� ����������: ");
    applicant.first_name = input_string("������� ��� ����������: ");
    applicant.middle_name = input_string("������� �������� ���������� (����� ���� ������): ");
    applicant.qualification = input_string("������� ������������ ����������: ");
    applicant.profession = input_string("������� ��������� ����������: ");
    applicant.other_data = input_string("������� ���� ������ � ���������� (����� ���� ������): ");
    applicant.expected_salary = input_double("������� �������������� ������ ���������� �����: ");

    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO Applicants (last_name, first_name, middle_name, qualification, profession, other_data, expected_salary) VALUES (?, ?, ?, ?, ?, ?, ?);";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, applicant.last_name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, applicant.first_name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, applicant.middle_name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, applicant.qualification.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, applicant.profession.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, applicant.other_data.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 7, applicant.expected_salary);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    cout << "���������� ��������!" << endl;
}

// ������� ��� ���������� ����� ������
void add_deal(sqlite3* db) {
    Deal deal;
    deal.applicant_id = input_int("������� ��� ����������: ");
    deal.employer_id = input_int("������� ��� ������������: ");
    deal.position = input_string("������� ���������: ");
    deal.commission = input_double("������� ������������: ");
    deal.contract_date = input_date("������� ���� �������� (YYYY-MM-DD): ");

    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO Deals (applicant_id, employer_id, position, commission, contract_date) VALUES (?, ?, ?, ?, ?);";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, deal.applicant_id);
    sqlite3_bind_int(stmt, 2, deal.employer_id);
    sqlite3_bind_text(stmt, 3, deal.position.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, deal.commission);
    sqlite3_bind_text(stmt, 5, deal.contract_date.c_str(), -1, SQLITE_STATIC);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    cout << "������ ���������!" << endl;
}

// ������� ��� ���������� ������ ������������
void update_employer(sqlite3* db) {
    int id = input_int("������� ��� ������������, �������� ������ ��������: ");
    Employer employer;
    employer.name = input_string("������� ����� �������� ������������: ");
    employer.activity_type = input_string("������� ����� ��� ������������ ������������: ");
    employer.address = input_string("������� ����� ����� ������������: ");
    employer.phone = input_string("������� ����� ������� ������������: ");

    sqlite3_stmt* stmt;
    const char* sql = "UPDATE Employers SET name = ?, activity_type = ?, address = ?, phone = ? WHERE id = ?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, employer.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, employer.activity_type.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, employer.address.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, employer.phone.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    cout << "������ ������������ ���������!" << endl;
}

// ������� ��� ���������� ������ ����������
void update_applicant(sqlite3* db) {
    int id = input_int("������� ��� ����������, �������� ������ ��������: ");
    Applicant applicant;
    applicant.last_name = input_string("������� ����� ������� ����������: ");
    applicant.first_name = input_string("������� ����� ��� ����������: ");
    applicant.middle_name = input_string("������� ����� �������� ���������� (����� ���� ������): ");
    applicant.qualification = input_string("������� ����� ������������ ����������: ");
    applicant.profession = input_string("������� ����� ��������� ����������: ");
    applicant.other_data = input_string("������� ����� ������ � ���������� (����� ���� ������): ");
    applicant.expected_salary = input_double("������� ����� �������������� ������ ���������� �����: ");

    sqlite3_stmt* stmt;
    const char* sql = "UPDATE Applicants SET last_name = ?, first_name = ?, middle_name = ?, qualification = ?, profession = ?, other_data = ?, expected_salary = ? WHERE id = ?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, applicant.last_name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, applicant.first_name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, applicant.middle_name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, applicant.qualification.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, applicant.profession.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, applicant.other_data.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 7, applicant.expected_salary);
    sqlite3_bind_int(stmt, 8, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    cout << "������ ���������� ���������!" << endl;
}

// ������� ��� ���������� ������ ������
void update_deal(sqlite3* db) {
    int applicant_id = input_int("������� ��� ���������� ��� ���������� ������: ");
    int employer_id = input_int("������� ��� ������������ ��� ���������� ������: ");
    Deal deal;
    deal.position = input_string("������� ����� ���������: ");
    deal.commission = input_double("������� ����� ������������: ");
    deal.contract_date = input_date("������� ����� ���� �������� (YYYY-MM-DD): ");

    sqlite3_stmt* stmt;
    const char* sql = "UPDATE Deals SET position = ?, commission = ?, contract_date = ? WHERE applicant_id = ? AND employer_id = ?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, deal.position.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 2, deal.commission);
    sqlite3_bind_text(stmt, 3, deal.contract_date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, applicant_id);
    sqlite3_bind_int(stmt, 5, employer_id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    cout << "������ ������ ���������!" << endl;
}

// ������� ��� �������� ������������
void delete_employer(sqlite3* db) {
    int id = input_int("������� ��� ������������, �������� ������ �������: ");

    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM Employers WHERE id = ?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    cout << "������������ ������!" << endl;
}

// ������� ��� �������� ����������
void delete_applicant(sqlite3* db) {
    int id = input_int("������� ��� ����������, �������� ������ �������: ");

    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM Applicants WHERE id = ?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    cout << "���������� ������!" << endl;
}

// ������� ��� �������� ������
void delete_deal(sqlite3* db) {
    int applicant_id = input_int("������� ��� ���������� ��� �������� ������: ");
    int employer_id = input_int("������� ��� ������������ ��� �������� ������: ");

    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM Deals WHERE applicant_id = ? AND employer_id = ?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, applicant_id);
    sqlite3_bind_int(stmt, 2, employer_id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    cout << "������ �������!" << endl;
}

// ������� ��� ������ ������ �������������
void list_employers(sqlite3* db) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT id, name, activity_type, address, phone FROM Employers;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    cout << "������ �������������:" << endl;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << "���: " << sqlite3_column_int(stmt, 0) << ", ��������: " << sqlite3_column_text(stmt, 1)
            << ", ��� ������������: " << sqlite3_column_text(stmt, 2) << ", �����: " << sqlite3_column_text(stmt, 3)
            << ", �������: " << sqlite3_column_text(stmt, 4) << endl;
    }
    sqlite3_finalize(stmt);
}

// ������� ��� ������ ������ �����������
void list_applicants(sqlite3* db) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT id, last_name, first_name, middle_name, qualification, profession, expected_salary FROM Applicants;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    cout << "������ �����������:" << endl;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << "���: " << sqlite3_column_int(stmt, 0) << ", ���: " << sqlite3_column_text(stmt, 1) << " "
            << sqlite3_column_text(stmt, 2) << " " << sqlite3_column_text(stmt, 3) << ", ������������: "
            << sqlite3_column_text(stmt, 4) << ", ���������: " << sqlite3_column_text(stmt, 5) << ", �/�: "
            << sqlite3_column_double(stmt, 6) << endl;
    }
    sqlite3_finalize(stmt);
}

// ������� ��� ������ ������ ������
void list_deals(sqlite3* db) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT applicant_id, employer_id, position, commission, contract_date FROM Deals;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    cout << "������ ������:" << endl;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << "��� ����������: " << sqlite3_column_int(stmt, 0) << ", ��� ������������: " << sqlite3_column_int(stmt, 1)
            << ", ���������: " << sqlite3_column_text(stmt, 2) << ", ������������: " << sqlite3_column_double(stmt, 3)
            << ", ���� ��������: " << sqlite3_column_text(stmt, 4) << endl;
    }
    sqlite3_finalize(stmt);
}

// ������� ������� ���������
int main() {
    setlocale(LC_ALL, "ru");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    sqlite3* db;
    if (sqlite3_open("employment_agency.db", &db)) {
        cerr << "������ �������� ���� ������: " << sqlite3_errmsg(db) << endl;
        return 1;
    }

    create_tables(db);

    while (true) {
        cout << "\n1. �������� ������������\n2. �������� ����������\n3. �������� ������\n"
            << "4. �������� ������������\n5. �������� ����������\n6. �������� ������\n"
            << "7. ������� ������������\n8. ������� ����������\n9. ������� ������\n"
            << "10. ������ �������������\n11. ������ �����������\n12. ������ ������\n"
            << "13. �����\n";
        int choice = input_int("�������� ��������: ");

        switch (choice) {
        case 1: add_employer(db); break;
        case 2: add_applicant(db); break;
        case 3: add_deal(db); break;
        case 4: update_employer(db); break;
        case 5: update_applicant(db); break;
        case 6: update_deal(db); break;
        case 7: delete_employer(db); break;
        case 8: delete_applicant(db); break;
        case 9: delete_deal(db); break;
        case 10: list_employers(db); break;
        case 11: list_applicants(db); break;
        case 12: list_deals(db); break;
        case 13: sqlite3_close(db); return 0;
        default: cout << "�������� �����. ���������� �����." << endl;
        }
    }

    sqlite3_close(db);
    return 0;
}
