#include <fstream>
#include <chrono>
#include <cstring>
#include "chip8.hpp"

const unsigned int FONTSET_SIZE = 80;
const unsigned int FONTSET_START_ADDRESS = 0x50;
const unsigned int START_ADDRESS = 0x200; // Starting address in memory

uint8_t fontset[FONTSET_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8() : randGen(std::chrono::system_clock::now().time_since_epoch().count()){
    pc = START_ADDRESS;

    // Load fonts into memory
    for (unsigned int i = 0; i < FONTSET_SIZE; ++i){
        memory[FONTSET_START_ADDRESS+i] = fontset[i];
    }

    randByte = std::uniform_int_distribution<uint8_t>(0, 255U);
}

void Chip8::LoadROM(char const* filename){
    // Open the file as a stream of binary and move the file pointer to the end
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (file.is_open()){
        // Get size of the file and allocate a buffer to hold the contents
        std::streampos size = file.tellg();
        char* buffer = new char[size];

        // Go back to the beginning of the file and fill the buffer
        file.seekg(0, std::ios::beg);
        file.read(buffer, size);
        file.close();

        // Load the ROM contents into Chip8's memory, starting at 0x200
        for(long i = 0; i < size; i++){
            memory[START_ADDRESS + i] = buffer[i];
        }

        // Free the buffer
        delete[] buffer;
    }
}

// Operations
// 0nnn - SYS addr
void Chip8::OP_00E0(){
    memset(video, 0, sizeof(video));
}

// 00E0 - CLS
void Chip8::OP_00EE(){
    --sp;
    pc = stack[sp];
}

// 1nnn - JP addr
void Chip8::OP_1nnn(){
    uint16_t address = opcode & 0x0FFFu; // the hex value is used to get the address value from the opcode

    pc = address;
}

// 2nnn - CALL addr
void Chip8::OP_2nnn(){
    uint16_t address = opcode & 0x0FFFu;

    stack[sp] = pc;
    ++sp;
    pc = address;
}

// 3xkk - SE Vx, byte
void Chip8::OP_3xkk(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    if (registers[Vx] == byte){
        pc += 2;
    }
}

// 4xkk - SNE Vx, byte
void Chip8::OP_4xkk(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    if (registers[Vx] != byte){
        pc += 2;
    }
}