// Updated version 3/26/25
#include "claudia_calc_example.h"


#include <sys/types.h>
#include <iostream>
#include <cassert>
#include "spdlog/spdlog.h"


using namespace std;


namespace claudia_calc {


// Constants
const int menu_width = 30;


// Registers A, B, C, D
double registers[4] = {0, 0, 0, 0};  // A=0, B=0, C=0, D=0


/*
 * Utility functions
 */
inline bool is_register(char const ch) { return tolower(ch) >= 'a' && ch <= 'd'; }


inline bool is_register(string const str) { return str.size() != 0 && is_register(str[0]); }


inline reg_name to_reg_name(char const ch) {
    assert(is_register(ch));
    return static_cast<reg_name>(tolower(ch) - 'a');
}


inline reg_name to_reg_name(string const str) {
    assert(is_register(str));
    return to_reg_name(str[0]);
}


inline char to_char(reg_name rn) { return static_cast<char>(rn + 'a'); }


/*
 * Calculator display functions
 */
inline void print_line() { cout << std::string(MENU_WIDTH, '-') << endl; }


inline void print_title(string const title) { cout << fmt::format("{:^{}}", title, MENU_WIDTH) << endl; }

// Print menu
void print_menu() {
    print_line();
    print_title("ClaudiaCalc");
    print_line();
    cout << "+\tAdd (A = A + B)" << endl;
    cout << "-\tSubtract (A = A - B)" << endl;
    cout << "*\tMultiply (A = A * B)" << endl;
    cout << "/\tDivide (A = A / B)" << endl;
    cout << "a-d\tEnter a value for register A-D" << endl;
    cout << "1-4\tClear register A-D" << endl;
    cout << "m\tShow this menu" << endl;
    cout << "p\tPrint registers" << endl;
    cout << "q\tQuit" << endl;
    print_line();
}


/*
 * Execute command
 */
void execute(string const cmd) {
    if (cmd.size() == 0) {
        spdlog::error("Empty command");
        return;
    }


    char const cmd_ch = std::tolower(cmd[0]);


    switch (cmd_ch) {
        // Set register value
        case 'a': case 'b': case 'c': case 'd': {
            double value;
            cout << "Enter value for register " << cmd_ch << ": ";
            cin >> value;
            registers[to_reg_name(cmd_ch)] = value;
            spdlog::info("Register {} set to {}", cmd_ch, value);
            break;
        }


        // Clear register
        case '1': case '2': case '3': case '4': {
            registers[cmd_ch - '1'] = 0;
            spdlog::info("Register {} cleared", to_char(static_cast<reg_name>(cmd_ch - '1')));
            break;
        }


        // Added Arithmetic operations (A and B)
        case '+':
            registers[0] += registers[1];
            spdlog::info("A = A + B -> {}", registers[0]);
            break;


        case '-':
            registers[0] -= registers[1];
            spdlog::info("A = A - B -> {}", registers[0]);
            break;


        case '*':
            registers[0] *= registers[1];
            spdlog::info("A = A * B -> {}", registers[0]);
            break;


        case '/':
            if (registers[1] != 0) {
                registers[0] /= registers[1];
                spdlog::info("A = A / B -> {}", registers[0]);
            } else {
                spdlog::error("Division by zero error!");
            }
            break;


        // Added Show menu
        case 'm':
            print_menu();
            break;


        // Print registers
        case 'p':
            for (int i = 0; i < 4; i++) {
                cout << to_char(static_cast<reg_name>(i)) << " = " << registers[i] << endl;
            }
            break;


        // Quit (already handled by loop)
        case 'q':
            break;


        // Unknown command
        default:
            spdlog::error("{} is an unknown command", cmd_ch);
            break;
    }
}


/*
 * Start the calculator
 */
void start() {
    string cmd = "";
    print_menu();
    while (cmd != "q") {
        cout << "Enter a command: ";
        cin >> cmd;
        spdlog::debug("cmd={}", cmd);
        execute(cmd);
    }
}


}  // namespace claudia_calc


/*
 * Main entry point
 */
using namespace claudia_calc;


int main() {
    spdlog::set_level(spdlog::level::debug);
    start();
    return 0;
}



