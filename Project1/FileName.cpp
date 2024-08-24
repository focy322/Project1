#include <iostream>
#include <vector>
#include <string>
#include <sqlite3.h>
#include<Windows.h>
#include <regex>

using namespace std;

// Структура для хранения данных о работодателе
struct Employer {
    int id;
    string name;
    string activity_type;
    string address;
    string phone;
};

// Структура для хранения данных о соискателе
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

// Структура для хранения данных о сделке
struct Deal {
    int applicant_id;
    int employer_id;
    string position;
    double commission;
    string contract_date; // Добавляем поле для даты договора
};

// Валидация ввода строки
string input_string(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (!input.empty()) break;
        cout << "Ошибка: Ввод не может быть пустым. Попробуйте снова." << endl;
    }
    return input;
}

// Валидация ввода числового значения (целого числа)
int input_int(const string& prompt) {
    int input;
    while (true) {
        cout << prompt;
        cin >> input;
        if (cin.fail()) {
            cin.clear(); // Очистка состояния ошибки
            cin.ignore((numeric_limits<streamsize>::max)(), '\n'); // Пропуск оставшегося ввода
            cout << "Ошибка: Введите корректное целое число." << endl;
        }
        else {
            cin.ignore((numeric_limits<streamsize>::max)(), '\n'); // Пропуск оставшегося ввода
            return input;
        }
    }
}

// Валидация ввода числа с плавающей точкой
double input_double(const string& prompt) {
    double input;
    while (true) {
        cout << prompt;
        cin >> input;
        if (cin.fail()) {
            cin.clear();
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            cout << "Ошибка: Введите корректное число." << endl;
        }
        else {
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            return input;
        }
    }
}

// Валидация ввода даты в формате YYYY-MM-DD
string input_date(const string& prompt) {
    string date;
    regex date_regex("\\d{4}-\\d{2}-\\d{2}");
    while (true) {
        cout << prompt;
        getline(cin, date);
        if (regex_match(date, date_regex)) break;
        cout << "Ошибка: Введите корректную дату в формате YYYY-MM-DD." << endl;
    }
    return date;
}

// Функция для создания таблиц в базе данных
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

// Функция для добавления нового работодателя
void add_employer(sqlite3* db) {
    Employer employer;
    employer.name = input_string("Введите название работодателя: ");
    employer.activity_type = input_string("Введите вид деятельности работодателя: ");
    employer.address = input_string("Введите адрес работодателя: ");
    employer.phone = input_string("Введите телефон работодателя: ");

    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO Employers (name, activity_type, address, phone) VALUES (?, ?, ?, ?);";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, employer.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, employer.activity_type.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, employer.address.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, employer.phone.c_str(), -1, SQLITE_STATIC);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    cout << "Работодатель добавлен!" << endl;
}

// Функция для добавления нового соискателя
void add_applicant(sqlite3* db) {
    Applicant applicant;
    applicant.last_name = input_string("Введите фамилию соискателя: ");
    applicant.first_name = input_string("Введите имя соискателя: ");
    applicant.middle_name = input_string("Введите отчество соискателя (может быть пустым): ");
    applicant.qualification = input_string("Введите квалификацию соискателя: ");
    applicant.profession = input_string("Введите профессию соискателя: ");
    applicant.other_data = input_string("Введите иные данные о соискателе (может быть пустым): ");
    applicant.expected_salary = input_double("Введите предполагаемый размер заработной платы: ");

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

    cout << "Соискатель добавлен!" << endl;
}

// Функция для добавления новой сделки
void add_deal(sqlite3* db) {
    Deal deal;
    deal.applicant_id = input_int("Введите код соискателя: ");
    deal.employer_id = input_int("Введите код работодателя: ");
    deal.position = input_string("Введите должность: ");
    deal.commission = input_double("Введите комиссионные: ");
    deal.contract_date = input_date("Введите дату договора (YYYY-MM-DD): ");

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

    cout << "Сделка добавлена!" << endl;
}

// Функция для обновления данных работодателя
void update_employer(sqlite3* db) {
    int id = input_int("Введите код работодателя, которого хотите изменить: ");
    Employer employer;
    employer.name = input_string("Введите новое название работодателя: ");
    employer.activity_type = input_string("Введите новый вид деятельности работодателя: ");
    employer.address = input_string("Введите новый адрес работодателя: ");
    employer.phone = input_string("Введите новый телефон работодателя: ");

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

    cout << "Данные работодателя обновлены!" << endl;
}

// Функция для обновления данных соискателя
void update_applicant(sqlite3* db) {
    int id = input_int("Введите код соискателя, которого хотите изменить: ");
    Applicant applicant;
    applicant.last_name = input_string("Введите новую фамилию соискателя: ");
    applicant.first_name = input_string("Введите новое имя соискателя: ");
    applicant.middle_name = input_string("Введите новое отчество соискателя (может быть пустым): ");
    applicant.qualification = input_string("Введите новую квалификацию соискателя: ");
    applicant.profession = input_string("Введите новую профессию соискателя: ");
    applicant.other_data = input_string("Введите новые данные о соискателе (может быть пустым): ");
    applicant.expected_salary = input_double("Введите новый предполагаемый размер заработной платы: ");

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

    cout << "Данные соискателя обновлены!" << endl;
}

// Функция для обновления данных сделки
void update_deal(sqlite3* db) {
    int applicant_id = input_int("Введите код соискателя для обновления сделки: ");
    int employer_id = input_int("Введите код работодателя для обновления сделки: ");
    Deal deal;
    deal.position = input_string("Введите новую должность: ");
    deal.commission = input_double("Введите новые комиссионные: ");
    deal.contract_date = input_date("Введите новую дату договора (YYYY-MM-DD): ");

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

    cout << "Данные сделки обновлены!" << endl;
}

// Функция для удаления работодателя
void delete_employer(sqlite3* db) {
    int id = input_int("Введите код работодателя, которого хотите удалить: ");

    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM Employers WHERE id = ?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    cout << "Работодатель удален!" << endl;
}

// Функция для удаления соискателя
void delete_applicant(sqlite3* db) {
    int id = input_int("Введите код соискателя, которого хотите удалить: ");

    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM Applicants WHERE id = ?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    cout << "Соискатель удален!" << endl;
}

// Функция для удаления сделки
void delete_deal(sqlite3* db) {
    int applicant_id = input_int("Введите код соискателя для удаления сделки: ");
    int employer_id = input_int("Введите код работодателя для удаления сделки: ");

    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM Deals WHERE applicant_id = ? AND employer_id = ?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, applicant_id);
    sqlite3_bind_int(stmt, 2, employer_id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    cout << "Сделка удалена!" << endl;
}

// Функция для вывода списка работодателей
void list_employers(sqlite3* db) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT id, name, activity_type, address, phone FROM Employers;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    cout << "Список работодателей:" << endl;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << "Код: " << sqlite3_column_int(stmt, 0) << ", Название: " << sqlite3_column_text(stmt, 1)
            << ", Вид деятельности: " << sqlite3_column_text(stmt, 2) << ", Адрес: " << sqlite3_column_text(stmt, 3)
            << ", Телефон: " << sqlite3_column_text(stmt, 4) << endl;
    }
    sqlite3_finalize(stmt);
}

// Функция для вывода списка соискателей
void list_applicants(sqlite3* db) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT id, last_name, first_name, middle_name, qualification, profession, expected_salary FROM Applicants;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    cout << "Список соискателей:" << endl;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << "Код: " << sqlite3_column_int(stmt, 0) << ", ФИО: " << sqlite3_column_text(stmt, 1) << " "
            << sqlite3_column_text(stmt, 2) << " " << sqlite3_column_text(stmt, 3) << ", Квалификация: "
            << sqlite3_column_text(stmt, 4) << ", Профессия: " << sqlite3_column_text(stmt, 5) << ", З/п: "
            << sqlite3_column_double(stmt, 6) << endl;
    }
    sqlite3_finalize(stmt);
}

// Функция для вывода списка сделок
void list_deals(sqlite3* db) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT applicant_id, employer_id, position, commission, contract_date FROM Deals;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    cout << "Список сделок:" << endl;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << "Код соискателя: " << sqlite3_column_int(stmt, 0) << ", Код работодателя: " << sqlite3_column_int(stmt, 1)
            << ", Должность: " << sqlite3_column_text(stmt, 2) << ", Комиссионные: " << sqlite3_column_double(stmt, 3)
            << ", Дата договора: " << sqlite3_column_text(stmt, 4) << endl;
    }
    sqlite3_finalize(stmt);
}

// Главная функция программы
int main() {
    setlocale(LC_ALL, "ru");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    sqlite3* db;
    if (sqlite3_open("employment_agency.db", &db)) {
        cerr << "Ошибка открытия базы данных: " << sqlite3_errmsg(db) << endl;
        return 1;
    }

    create_tables(db);

    while (true) {
        cout << "\n1. Добавить работодателя\n2. Добавить соискателя\n3. Добавить сделку\n"
            << "4. Обновить работодателя\n5. Обновить соискателя\n6. Обновить сделку\n"
            << "7. Удалить работодателя\n8. Удалить соискателя\n9. Удалить сделку\n"
            << "10. Список работодателей\n11. Список соискателей\n12. Список сделок\n"
            << "13. Выход\n";
        int choice = input_int("Выберите действие: ");

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
        default: cout << "Неверный выбор. Попробуйте снова." << endl;
        }
    }

    sqlite3_close(db);
    return 0;
}
