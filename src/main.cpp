#include <iostream>
#include <ios>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <assert.h>
#include <bitset>
#include <iomanip>
#include <thread>
#include <chrono>

typedef unsigned char t_byte;
typedef unsigned short int t_word;
typedef bool t_pixel;

const int MEMORY_SIZE = 0xfff;
const int NUMBER_REGISTERS = 16;
const int NUMBER_CONTROLLER_KEYS = 16;
const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;

t_pixel display_matrix[SCREEN_WIDTH][SCREEN_WIDTH];

int controller[NUMBER_CONTROLLER_KEYS];

t_byte memory[MEMORY_SIZE];
std::vector<t_word> stack;


t_byte data_registers[NUMBER_REGISTERS];
t_word address_register;

t_word program_counter;

const int freq_timer_Hz = 60;
int m_delay_timer = 0;
int m_sound_timer = 0;


const int START_GAME_ADDRESS = 0x200;

void load_game(std::string absolute_path_game);

t_word get_next_op_code();

void clear_screen(t_word op_code);
void return_from_subroutine(t_word op_code);
void call_machine_routine(t_word op_code);

int get_vx_from_op_code(t_word op_code);
int get_vy_from_op_code(t_word op_code);

//opcodes
// call machine routine
void op_0nnn(t_word op_code);
// goto address
void op_1nnn(t_word op_code);
// call function
void op_2nnn(t_word op_code);
// skip if equal
void op_3xnn(t_word op_code);
// skip if not equal
void op_4xnn(t_word op_code);
// skip if vy == vx
void op_5xy0(t_word op_code);
// set vx to value
void op_6xnn(t_word op_code);
// auto increment vx
void op_7xnn(t_word op_code);
// copy value from vy to vx
void op_8000(t_word op_code);
void op_8xy0(t_word op_code);
// or bit betewwn vx and vy
void op_8xy1(t_word op_code);
// and bit between vx and vy
void op_8xy2(t_word op_code);
// xor between vx and vy
void op_8xy3(t_word op_code);
// add vy to vx
void op_8xy4(t_word op_code);
// vx -= vy
void op_8xy5(t_word op_code);
//shift bit towards right for vx
void op_8xy6(t_word op_code);
// vx = vy -vx
void op_8xy7(t_word op_code);
// shit vx bits left
void op_8xye(t_word op_code);
// skip if vx != vy
void op_9xy0(t_word op_code);
// set address register to nnn
void op_annn(t_word op_code);
// pc = v0 + nnn
void op_bnnn(t_word op_code);
// vx = rand() & nn
void op_cxnn(t_word op_code);
// draw (vx, vy, n) // n is 4 bit
void op_dxyn(t_word op_code);

void op_ex00(t_word op_code);

void op_ex9e(t_word op_code);

void op_exa1(t_word op_code);

void op_f000(t_word op_code);

void op_fx07(t_word op_code);

void op_fx0a(t_word op_code);

void op_fx15(t_word op_code);

void op_fx18(t_word op_code);

void op_fx1e(t_word op_code);

void op_fx29(t_word op_code);

void op_fx33(t_word op_code);

void op_fx55(t_word op_code);

void op_fx65(t_word op_code);

void draw_sprite(int x, int y, int width);


const std::string path {"/home/vlad/projects/chip-8-emulator/games/space_invaders.ch8"};

int main(){
    load_game(path);
    int n_ops = 0;

    t_word current_op_code;
    while (n_ops < 20) {
        n_ops += 1;
        std::cout << "PC at " << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << static_cast<int>(program_counter) << " ";
        current_op_code = get_next_op_code();
        //program_counter += 100;
        //continue;
        switch (current_op_code & 0xf000) {
            case 0x0000: op_0nnn(current_op_code); break;
            case 0x1000: op_1nnn(current_op_code); break;
            case 0x2000: op_2nnn(current_op_code); break;
            case 0x3000: op_3xnn(current_op_code); break;
            case 0x4000: op_4xnn(current_op_code); break;
            case 0x5000: op_5xy0(current_op_code); break;
            case 0x6000: op_6xnn(current_op_code); break;
            case 0x7000: op_7xnn(current_op_code); break;
            case 0x8000: op_8000(current_op_code); break;
            case 0x9000: op_9xy0(current_op_code); break;
            case 0xa000: op_annn(current_op_code); break;
            case 0xb000: op_bnnn(current_op_code); break;
            case 0xc000: op_cxnn(current_op_code); break;
            case 0xd000: op_dxyn(current_op_code); break;
            case 0xe000: op_ex00(current_op_code); break;
            case 0xf000: op_f000(current_op_code); break;
        
        default:
            break;
        }

    }

}


void load_game(std::string absolute_path_game) {
    // reset program counter
    program_counter = START_GAME_ADDRESS;
    std::cout << "program conunter reset.\n";

    // reset registers
    std::memset(data_registers, 0, NUMBER_REGISTERS);
    address_register = 0;
    std::cout << "registers reset.\n";

    std::cout << "loading game in memory...\n";
    FILE* file_descriptor;
    file_descriptor = fopen(absolute_path_game.c_str(), "rb");
    fread(memory + START_GAME_ADDRESS, MEMORY_SIZE, 1, file_descriptor);
    fclose(file_descriptor);
    std::cout << "game loaded with success!\n";
    return;
}


t_word get_next_op_code(){
    t_word op_code;
    op_code = memory[program_counter];
    // move operation to first byte of opcode
    op_code <<= 8;
    // get argument for the opcode
    op_code |= memory[program_counter + 1];

    program_counter += 2;

    std::cout << "fetched new istruction: ";
    std::cout << std::hex << std::uppercase << std::setw(4) <<  std::setfill('0') << static_cast<int>(op_code);
    std::cout << " from binary opcode: " << std::bitset<sizeof(t_word) * 8>(op_code) << "\n";

    return op_code;
}

int get_vx_from_op_code(t_word op_code){
    int vx = (op_code & 0x0f00) >> 8;
    std::cout << "VX = " << vx << "\n";
    return vx;

}
int get_vy_from_op_code(t_word op_code){
    int vy = (op_code & 0x00f0) >> 4;
    std::cout << "VY = " << vy << "\n";
    return vy;
}

int get_value_from_op_code(int op_code, int n_nibbles) {
    assert (n_nibbles <= 3 && n_nibbles >= 1);
    int extracted_value;
    switch (n_nibbles)
    {
    case 1:
        extracted_value = op_code & 0x000f;
        break;
    case 2:
        extracted_value = op_code & 0x00ff;
        break;
    case 3:
        extracted_value = op_code & 0x0fff;
        break;
    
    default:
        break;
    }
    std::cout << "extracted value:" << extracted_value << "\n";
    return extracted_value;
}


void clear_screen(t_word op_code) {
    std::cout << "cleared screen";
}

void return_from_subroutine(t_word op_code) {
    program_counter = stack.back();   
    stack.pop_back();
    std::cout << "return from routine. Current PC = " << program_counter << "\n";
}

void call_machine_routine(t_word op_code) {
    std::cout << "calling machine code routine...\n";
}

//opcodes
// call machine routine
void op_0nnn(t_word op_code) {
    switch (0xf & op_code) {
        case 0x0:
            clear_screen(op_code);
            break;
        case 0xe:
            return_from_subroutine(op_code);
            break;
        default:
            call_machine_routine(op_code); 
            break;
    }
}
// dips_clear()
void op_00e0(t_word op_code) {
    clear_screen(op_code);
}
// return
void op_00ee(t_word op_code) {
    return_from_subroutine(op_code);
}
 
// goto address
void op_1nnn(t_word op_code) {
    int address = get_value_from_op_code(op_code, 3);
    program_counter = address;
    std::cout << "jump to " << program_counter << "\n";
}

// call function
void op_2nnn(t_word op_code) {
    int address = get_value_from_op_code(op_code, 3);
    stack.push_back(program_counter);
    program_counter = address;
    std::cout << "call function. PC = " << program_counter << "\n";
    
}
// skip if equal
void op_3xnn(t_word op_code){
    int reg_x;
    int value;

    reg_x = get_vx_from_op_code(op_code);
    value = get_value_from_op_code(op_code, 2);
    std::cout << "skip if V" << reg_x << " == " << value << "\n";

    if (data_registers[reg_x] == value) {
        program_counter += 2;
        return;
    }
    return;
}
// skip if not equal
void op_4xnn(t_word op_code){
    int reg_x;
    int value;

    reg_x = get_vx_from_op_code(op_code);
    value = get_value_from_op_code(op_code, 2);

    if (data_registers[reg_x] != value) {
        program_counter += 2;
        return;
    }
    return;
}

// skip if vy == vx
void op_5xy0(t_word op_code){
    int reg_x;
    int reg_y;
    
    reg_x = get_vx_from_op_code(op_code);
    reg_y = get_vy_from_op_code(op_code);

    if (data_registers[reg_x] == data_registers[reg_y]) {
        program_counter += 2;
        return;
    }
    return;
}

// set vx to value
void op_6xnn(t_word op_code) {
    int reg_x;
    int value;

    reg_x = get_vx_from_op_code(op_code);
    value = get_value_from_op_code(op_code, 2);

    data_registers[reg_x] = value;
    return;
}
// auto increment vx
void op_7xnn(t_word op_code) {
    int reg_x;
    int value;

    reg_x = get_vx_from_op_code(op_code);
    value = get_value_from_op_code(op_code, 2);

    data_registers[reg_x] += value;
}

void op_8000(t_word op_code) {
    switch(op_code & 0x000f){
        case 0x0000: op_8xy0(op_code); break;
        case 0x0001: op_8xy1(op_code); break;
        case 0x0002: op_8xy2(op_code); break;
        case 0x0003: op_8xy3(op_code); break;
        case 0x0004: op_8xy4(op_code); break;
        case 0x0005: op_8xy6(op_code); break;
        case 0x0006: op_8xy6(op_code); break;
        case 0x0007: op_8xy7(op_code); break;    
        case 0x000e: op_8xye(op_code); break;
    }
    return;
}

// copy value from vy to vx
void op_8xy0(t_word op_code) {
    int vx_idx = get_vx_from_op_code(op_code);
    int vy_idx = get_vy_from_op_code(op_code);

    data_registers[vx_idx] = data_registers[vy_idx];
    std::cout << "copy value";
    return;
}
// or bit betewwn vx and vy
void op_8xy1(t_word op_code) {
    int vx_idx = get_vx_from_op_code(op_code);
    int vy_idx = get_vy_from_op_code(op_code);

    data_registers[vx_idx] |= data_registers[vy_idx];
    return;
}
// and bit between vx and vy
void op_8xy2(t_word op_code) {
    int vx_idx = get_vx_from_op_code(op_code);
    int vy_idx = get_vy_from_op_code(op_code);

    data_registers[vx_idx] &= data_registers[vy_idx];
    return;
}
// xor between vx and vy
void op_8xy3(t_word op_code) {
    int vx_idx = get_vx_from_op_code(op_code);
    int vy_idx = get_vy_from_op_code(op_code);

    data_registers[vx_idx] ^= data_registers[vy_idx];
    return;
}
// add vy to vx
void op_8xy4(t_word op_code) {
    int vx_idx = get_vx_from_op_code(op_code);
    int vy_idx = get_vy_from_op_code(op_code);

    data_registers[vx_idx] += data_registers[vy_idx];
    return;
}
// vx -= vy
void op_8xy5(t_word op_code) {
    int vx_idx = get_vx_from_op_code(op_code);
    int vy_idx = get_vy_from_op_code(op_code);

    data_registers[vx_idx] -= data_registers[vy_idx];
    return;
}
//shift bit towards right for vx
void op_8xy6(t_word op_code) {
    int vx_idx = get_vx_from_op_code(op_code);

    data_registers[vx_idx] >>= 1;
    return;
}
// vx = vy -vx
void op_8xy7(t_word op_code) {
    int vx_idx = get_vx_from_op_code(op_code);
    int vy_idx = get_vy_from_op_code(op_code);

    data_registers[vx_idx] = data_registers[vy_idx] - data_registers[vx_idx];
    return;
}
// shit vx bits left
void op_8xye(t_word op_code) {
    int vx_idx = get_vx_from_op_code(op_code);

    data_registers[vx_idx] <<= 1;
    return;
}
// skip if vx != vy
void op_9xy0(t_word op_code) {
    int vx_idx = get_vx_from_op_code(op_code);
    int vy_idx = get_vy_from_op_code(op_code);

    if (data_registers[vx_idx] != data_registers[vy_idx]){
        program_counter += 2;
    }
    return;
}
// set address register to nnn
void op_annn(t_word op_code) {
    int address = get_value_from_op_code(op_code, 3);
    address_register = address;
    return;
}
// pc = v0 + nnn
void op_bnnn(t_word op_code) {
    int value = get_value_from_op_code(op_code, 3);
    program_counter = data_registers[0] + value;
    return;
}
// vx = rand() & nn
void op_cxnn(t_word op_code) {
    int vx_idx = get_vx_from_op_code(op_code);
    int value = get_value_from_op_code(op_code, 2);
    int random_number = rand() % 255;
    data_registers[vx_idx] = random_number & value;
    return;  
}
// draw (vx, vy, n) // n is 4 bit
void op_dxyn(t_word op_code) {
    int vx_idx = get_vx_from_op_code(op_code);
    int vy_idx = get_vy_from_op_code(op_code);
    int value = get_value_from_op_code(op_code, 1);
    draw_sprite(
        data_registers[vx_idx],
        data_registers[vy_idx],
        value
    );
    return;
}

void op_ex00(t_word op_code) {
    switch (op_code & 0x00ff) {
        case 0x009e: op_ex9e(op_code); break;
        case 0x00a1: op_exa1(op_code); break;
        default: break;
    }
}

bool is_key_pressed(int key){
    bool ans = true;
    std::cout << "key " << key << " is pressed" << ans << "\n";
    return ans;
}

void op_ex9e(t_word op_code) {
    int vx = get_vx_from_op_code(op_code);
    
    if (is_key_pressed(data_registers[vx])) {
        program_counter += 2;
    }
    return;

}

void op_exa1(t_word op_code) {
    int vx = get_vx_from_op_code(op_code);

    if (!is_key_pressed(data_registers[vx])) {
        program_counter += 2;
    }
    return;
}

void op_f000(t_word op_code) {
    switch (op_code & 0x00ff) {
        case 0x0007: op_fx07(op_code); break;
        case 0x000a: op_fx0a(op_code); break;
        case 0x0015: op_fx15(op_code); break;
        case 0x0018: op_fx18(op_code); break;
        case 0x001e: op_fx1e(op_code); break;
        case 0x0029: op_fx29(op_code); break;
        case 0x0033: op_fx33(op_code); break;
        case 0x0055: op_fx55(op_code); break;
        case 0x0065: op_fx65(op_code); break;
        default: break;
    }
}

void op_fx07(t_word op_code) {
    int vx = get_vx_from_op_code(op_code);
    std::cout << "set v" << vx << " to delay timer: " << m_delay_timer << "\n";
    data_registers[vx] = m_delay_timer;
    return;
}

int press_key() {
    std::cout << "waiting for user to press a key:\n";
    int pressed_key;
    std::cin >> pressed_key;
    return pressed_key;
    
}

void op_fx0a(t_word op_code) {
    int vx = get_vx_from_op_code(op_code);
    data_registers[vx] = press_key();
    return;
}

void op_fx15(t_word op_code) {
    int vx = get_vx_from_op_code(op_code);
    m_delay_timer = data_registers[vx];
}

void op_fx18(t_word op_code) {
    int vx = get_vx_from_op_code(op_code);

    m_sound_timer = data_registers[vx];
    std::cout << "set sound timer to " << m_sound_timer << "\n";
    return;
}

void op_fx1e(t_word op_code) {
    int vx = get_vx_from_op_code(op_code);

    std::cout << "I+=V" << vx << "\n";
    address_register += data_registers[vx];
    return;
}

void op_fx29(t_word op_code) {}

void op_fx33(t_word op_code) {}

void op_fx55(t_word op_code) {
    int vx_idx = get_vx_from_op_code(op_code);
    memcpy(memory + address_register, data_registers, sizeof(t_byte) * vx_idx);
    return;
}

void op_fx65(t_word op_code) {
    int vx_idx = get_vx_from_op_code(op_code);
    memcpy(data_registers, memory + address_register, sizeof(t_byte) * vx_idx);
    return;
}



void draw_sprite(int x, int y, int width) {
    std::cout << "draw sprite at (" << x << "," << y << ")";
    return;  
}