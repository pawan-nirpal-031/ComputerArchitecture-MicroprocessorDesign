#include "Emulator_6502.h"

void LoadTestProgram(Memory &bank){
    bank[0xFFFC] = JUMP_SUBR;
    bank[0xFFFD] = 0x42;
    bank[0xFFFE] = 0x42;
    bank[0x4242] = LOAD_ACC;
    bank[0x4243] = 0x84; // 0b010000100
} 


int main(){
    Memory bank;
    CPU cpu;
    cpu.MachineReset(bank);
    LoadTestProgram(bank);
    cpu.Exceute(9,bank);   
    return 0;
}