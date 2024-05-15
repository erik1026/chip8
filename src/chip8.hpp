#include <cstdint>
#include <random>

class Chip8{
    public:
        uint8_t registers[16]{}; // Chip 8 has 16 8 bit registers
        uint8_t memory [4096]{}; // Chip 8 has 4K of memory
        uint16_t index{};        // 16 bit register used to store memory addresses for use in operations
        uint16_t pc{};           // 16 bit register that holds the memory address for the next instruction
        uint16_t stack[16]{};    // 16 bit array used to store the return addresses when a new CALL operation is executed
        uint8_t sp{};            // Pointer that tells the CPU where we are in the call stack
        uint8_t delayTimer{};    // 8 bit timer that decrements at 60 HZ when the value is non-zero
        uint8_t soundTimer{};    // 8 bit register that decrements at 60 HZ when the value is non-zero
        uint8_t keypad[16]{};    // Array used to store the keypad values of 0-F
        uint32_t video[64*32]{}; // Array that will store the current screen image
        uint16_t opcode;         // The current opcode value


        Chip8();
        void LoadROM(char const*);

    private:

        void OP_00E0(); // Clear the display
        void OP_00EE(); // Return from a subroutine
        void OP_1nnn(); // Jump to location nnn
        void OP_2nnn(); // Call subroutine at nnn
        void OP_3xkk(); // Skip next instruction if Vx = kk
        void OP_4xkk(); // skip next instruction if Vx != kk
        void OP_5xy0(); // Skip next instruction if Vx = Vy
        void OP_6xkk(); // Set Vx = kk
        void OP_7xkk(); // Set Vx = Vx + kk
        void OP_8xy0(); // Set Vx = Vy
        void OP_8xy1(); // Set Vx = Vx OR Vy
        void OP_8xy2(); // Set Vx = Vx AND Vy
        void OP_8xy3(); // Set Vx = Vx XOR Vy
        void OP_8xy4(); // Set Vx = Vx + Vy, set Vf = carry
        void OP_8xy5(); // Set Vx = Vx - Vy, set VF = Not borrow
        void OP_8xy6(); // Set Vx = Vx SHR 1
        void OP_8xy7(); // Set Vx = Vy - Vx, set VF = NOT borrow
        void OP_8xyE(); // Set Vx = Vx SHL 1
        void OP_9xy0(); // Skip next instruction if Vx != Vy
        void OP_Annn(); // Set I = nnn
        void OP_Bnnn(); // Jump to location nnn + V0
        void OP_Cxkk(); // Set Vx = random byte AND kk
        void OP_Dxyn(); // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
        void OP_Ex9E(); // Skip next instruction if key with the value of Vx is pressed
        void OP_ExA1(); // Skip next instruction if key with the value of Vx is not pressed
        void OP_Fx07(); // Set Vx = delay timer value
        void OP_Fx0A(); // Wait for a key press, store the value of the key in Vx
        void OP_Fx15(); // Set delay timer = Vx
        void OP_Fx18(); // Set sound timer = Vx
        void OP_Fx1E(); // Set I = I + Vx
        void OP_Fx29(); // Set I = location of sprite for digit Vx
        void OP_Fx33(); // Store BCD representation of Vc in memory locations I, I+1, I+2
        void OP_Fx55(); // Store registers V0 through Vx in memory starting at location I
        void OP_Fx65(); // Read registers V0 through Vx from memory starting at location I


        std::default_random_engine randGen;
        std::uniform_int_distribution<uint8_t> randByte;

};