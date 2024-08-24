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

// ������� ��� �������� ���������� �����
bool isCppFile(const string& filename) {
    size_t dot_pos = filename.rfind('.');
    if (dot_pos == string::npos) return false; // ��� ����� � �������� �����
    string extension = filename.substr(dot_pos);
    return extension == ".cpp";
}

int main(int argc, char* argv[]) {
    int multiline_comment_count = 0;  // ������� ������������� ������������

    setlocale(LC_ALL, "rus");

    // ��������� ���������� ��������� ������
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
            cout << "�������� ��������: " << argv[i] << endl;
            return 1;
        }
    }
    /*asdasasd
    
    dasda
    
    
    
    asd
    */
    // ���� ����� �������� �����, ���� �� ������ ����� ��������� ������
    if (InputFile.empty()) {
        cout << "������� ���� ��� ������: ";
        cin >> InputFile;
    }

    // �������� ���������� �������� �����
    if (!isCppFile(InputFile)) {
        cout << "������: ������� ���� ������ ����� ���������� .cpp" << endl;
        return 1;
    }

    input.open(InputFile.c_str());
    if (!input.is_open()) {
        cout << "���� ��� ������ �� ������." << endl;
        return 1;
    }/*
     
     
     
     
     
     
     d
     das*/

    // ���� ����� ��������� �����, ���� �� ������ ����� ��������� ������
    if (OutputFile.empty()) {
        cout << "������� ���� ��� ������: ";
        cin >> OutputFile;
    }

    // �������� ������������� ��������� �����
    ifstream test_of_outfile(OutputFile.c_str());
    if (test_of_outfile.is_open()) {
        test_of_outfile.close();
        double choice = 0;

        cout << "���� � ����� ��������� ��� ����������."
            << "\n������ ������������ ���� ���� ��� �������� ��� ��������?"
            << "\n������� 0, ����� ������������, 1 - �������� ��������.\n";
        cin >> choice;

        if (choice == 1) {
            cout << "������� ����� �������� ����� ��� ������: ";
            cin >> OutputFile;
        }
        else if (choice != 0) {
            cout << "�������� �����. ���� ����� ������� �� ��������� ����.\n";
            OutputFile.assign("Temp.txt");
        }
    }

    output.open(OutputFile.c_str());
    if (!output.is_open()) {
        cout << "�� ������� ������� ���� ��� ������." << endl;
        return 1;
    }

    cout << "\n��������� �����\n";

    bool inside_multiline_comment = false;
    while (!input.eof()) {
        string read_str;
        getline(input, read_str);
        cout << "��: " << read_str << endl;

        // �������� ������ �������������� �����������
        size_t start_pos = read_str.find();
        if (start_pos != string::npos) {
            inside_multiline_comment = true;
            multiline_comment_count++;
            read_str.erase(start_pos); // ������� ����� ������ ������� � �����������
        }

        // �������� ����� �������������� �����������
        size_t end_pos = read_str.find("*/");
        if (end_pos != string::npos && inside_multiline_comment) {
            inside_multiline_comment = false;
            read_str.erase(0, end_pos + 2); // ������� ����� ������ �� ����� �����������
        }

        // ���� �� ������ �������������� �����������, ���������� ������ � ����
        if (!inside_multiline_comment && !read_str.empty()) {
            output << read_str << endl;
            cout << "���: " << read_str;
        }

        if (inside_multiline_comment) {
            cout << "���: " << endl;
        }
        cout << endl;
    }

    cout << "���������� ������������� ������������: " << multiline_comment_count << endl;

    input.close();
    output.close();

    return 0;
}
