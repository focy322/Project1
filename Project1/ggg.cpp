#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

ifstream input;
ofstream output;
string InputFile, OutputFile;

// Функция для проверки расширения файла
bool isCppFile(const string& filename) {
    size_t dot_pos = filename.rfind('.');
    if (dot_pos == string::npos) return false; // Нет точки в названии файла
    string extension = filename.substr(dot_pos);
    return extension == ".cpp";
}

int main(int argc, char* argv[]) {
    int multiline_comment_count = 0;  // Счётчик многострочных комментариев

    setlocale(LC_ALL, "rus");

    // Обработка аргументов командной строки
    for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == "-I" && i + 1 < argc) {
            InputFile = argv[i + 1];
            i++;
        }
        else if (string(argv[i]) == "-O" && i + 1 < argc) {
            OutputFile = argv[i + 1];
            i++;
        }
        else {
            cout << "Неверный аргумент: " << argv[i] << endl;
            return 1;
        }
    }
    /*asdasasd
    
    dasda
    
    
    
    asd
    */
    // Ввод имени входного файла, если не указан через командную строку
    if (InputFile.empty()) {
        cout << "Введите файл для чтения: ";
        cin >> InputFile;
    }

    // Проверка расширения входного файла
    if (!isCppFile(InputFile)) {
        cout << "Ошибка: входной файл должен иметь расширение .cpp" << endl;
        return 1;
    }

    input.open(InputFile.c_str());
    if (!input.is_open()) {
        cout << "Файл для чтения не найден." << endl;
        return 1;
    }/*
     
     
     
     
     
     
     d
     das*/

    // Ввод имени выходного файла, если не указан через командную строку
    if (OutputFile.empty()) {
        cout << "Введите файл для записи: ";
        cin >> OutputFile;
    }

    // Проверка существования выходного файла
    ifstream test_of_outfile(OutputFile.c_str());
    if (test_of_outfile.is_open()) {
        test_of_outfile.close();
        double choice = 0;

        cout << "Файл с таким названием уже существует."
            << "\nХотите перезаписать этот файл или изменить его название?"
            << "\nВведите 0, чтобы перезаписать, 1 - изменить название.\n";
        cin >> choice;

        if (choice == 1) {
            cout << "Введите новое название файла для вывода: ";
            cin >> OutputFile;
        }
        else if (choice != 0) {
            cout << "Неверный выбор. Файл будет записан во временный файл.\n";
            OutputFile.assign("Temp.txt");
        }
    }

    output.open(OutputFile.c_str());
    if (!output.is_open()) {
        cout << "Не удалось открыть файл для записи." << endl;
        return 1;
    }

    cout << "\nОбработка файла\n";

    bool inside_multiline_comment = false;
    while (!input.eof()) {
        string read_str;
        getline(input, read_str);
        cout << "вх: " << read_str << endl;

        // Проверка начала многострочного комментария
        size_t start_pos = read_str.find();
        if (start_pos != string::npos) {
            inside_multiline_comment = true;
            multiline_comment_count++;
            read_str.erase(start_pos); // Удаляем часть строки начиная с комментария
        }

        // Проверка конца многострочного комментария
        size_t end_pos = read_str.find("*/");
        if (end_pos != string::npos && inside_multiline_comment) {
            inside_multiline_comment = false;
            read_str.erase(0, end_pos + 2); // Удаляем часть строки до конца комментария
        }

        // Если не внутри многострочного комментария, записываем строку в файл
        if (!inside_multiline_comment && !read_str.empty()) {
            output << read_str << endl;
            cout << "вых: " << read_str;
        }

        if (inside_multiline_comment) {
            cout << "вых: " << endl;
        }
        cout << endl;
    }

    cout << "Количество многострочных комментариев: " << multiline_comment_count << endl;

    input.close();
    output.close();

    return 0;
}
