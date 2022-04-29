#include <iostream>
#include <stdlib.h>
using namespace std;
using Byte = unsigned char;
using Word = unsigned short;
using u32 = unsigned int;

/*
    ProcessorStatusWord[0] = C (Carry)
    ProcessorStatusWord[1] = Z (Zero) 
    ProcessorStatusWord[2] = I (Intrupt) 
    ProcessorStatusWord[3] = D (Decimal) when in D mode CPU uses BCD arithmatic 
    ProcessorStatusWord[4] = B (Break) 
    ProcessorStatusWord[5] = V (Overflow) 
    ProcessorStatusWord[6] = N (Negetive)
*/

bool debug = 1;

void Binary(Byte x){
    if(x==0) {
        cout<<0;
        return;
    }
    Binary(x>>1);
    cout<<(x%2);
}

// Machine ISA/Opcodes 
static constexpr Byte LOAD_ACC = 0xA9; // needs 2 clock cycles 
static constexpr Byte ZERO_PG  = 0xA5; // needs 3 clock cycles 
static constexpr Byte ZERO_PG_X = 0xB5; // needs 4 clock cycles 
static constexpr Byte JUMP_SUBR = 0xB7; // variable number of cycles depending on the Jump address

class Memory{
    private:
        const static u32 MAX_MEMORY = (1<<16);  
        Byte Storage[MAX_MEMORY];
    public:
        void Initalize(){
            for(Byte &loc : this->Storage) loc = 0;
        }
        void PrintMemoryContents(int start=0,int end = ((1<<16)-1)){
            if(start<0 or end>=MAX_MEMORY) return;
            for(int i =start;i<=end;i++)  cout<<this->Storage[i]<<' ';
            cout<<"\n";
        }

        bool IsValidAddress(u32 Address){
            return (Address>=0 and Address<MAX_MEMORY);
        }

        Byte operator[] (u32 Address) const {
            if(Address<0 or Address>=MAX_MEMORY) return '\0';
            return this->Storage[Address];    
        }

        Byte& operator[] (u32 Address){
            return this->Storage[Address];
        }

        // Writing a word 2 bytes of data into memory
        void WriteWord(u32 &cycles,Word content,u32 Address){
            this->Storage[Address] = (content&0xFF);
            this->Storage[Address+1] = (content>>8);
            cycles-=2;
        }

};  

class CPU{
    private:
        Word ProgramCounter; // Program counter 
        Word StackPointer; // stack pointer 
        Byte Accumlator,IndexReg,Y; // Y is GPR 
        bool ProcessorStatusWord[7]; 
    public:
        CPU(){
            for(bool &x : ProcessorStatusWord) x = 1;
        }

        Byte FetchByte(u32 &cycles,Memory &bank){
            Byte content = bank[ProgramCounter++];
            if(debug) cout<<"Fetched Byte"<<'\n';
            cycles-=1;
            if(debug) cout<<" cycle : "<<cycles<<'\n';
            return content;
        }   

        Byte ReadByte(u32 &cycles,Byte Address,Memory &bank){
            Byte content = bank[Address];
            cycles-=1;
            if(debug) cout<<" Read Byte cycle : "<<cycles<<'\n';
            return content;
        }

        Word FetchWord(u32 &cycles,Memory &bank){ // 6502 is little endian 
            Word content = bank[ProgramCounter++];
            content |= (bank[ProgramCounter++]<<8);
            cycles-=2;
            return content;
        }


        void MachineReset(Memory &mem_bank){
            mem_bank.Initalize();
            ProgramCounter = 0xFFFC;
            StackPointer = 0x0100;
            Accumlator = IndexReg = Y =0;
            for(bool &flg : ProcessorStatusWord) flg = 0;
        }

        Byte GetDefaultProgramCounter(){
            return (Byte)0xFFFC;
        }

        Byte GetDefaultStackPoiner(){
            return (Byte)0x0100;
        }

        string InvalidMemAccess(){
            return "Invalid Memory Access";
        }

        void EpilogeFlagSets(){
            ProcessorStatusWord[1] = (Accumlator==0);
            ProcessorStatusWord[6] = (Accumlator&(1<<7));
        }

        void Exceute(u32 cycles, Memory &mem_bank){
           try{
                while(cycles-->0){
                    Byte instruction = mem_bank[ProgramCounter++];
                    if(instruction=='\0') throw InvalidMemAccess();
                    switch(instruction){
                        case (LOAD_ACC):{
                            Accumlator = FetchByte(cycles,mem_bank);
                            EpilogeFlagSets();
                            if(debug){
                                Binary(Accumlator);
                                cout<<"\n";
                            }
                        }break;
                        
                        case (ZERO_PG):{
                            Byte ZeroPageAddress = FetchByte(cycles,mem_bank);
                            if(not mem_bank.IsValidAddress(ZeroPageAddress)) throw InvalidMemAccess();
                            Accumlator = ReadByte(cycles,ZeroPageAddress,mem_bank);
                            EpilogeFlagSets();
                        }break;

                        case(ZERO_PG_X):{ // needs 4 clock cycles 
                             Byte ZeroPageAddress = FetchByte(cycles,mem_bank);
                             ZeroPageAddress+=IndexReg;
                             if(not mem_bank.IsValidAddress(ZeroPageAddress)) throw InvalidMemAccess();
                             cycles-=1; // Adding index reg to zero pg address takes 1 cycle
                             Accumlator = ReadByte(cycles,ZeroPageAddress,mem_bank);
                             EpilogeFlagSets();
                        }break;

                        case(JUMP_SUBR):{
                            Word SubroutineJmpAddress = FetchWord(cycles,mem_bank);
                            if(not mem_bank.IsValidAddress(SubroutineJmpAddress)) throw InvalidMemAccess();
                            mem_bank.WriteWord(cycles,ProgramCounter-1,StackPointer++);
                            ProgramCounter = SubroutineJmpAddress;
                            cycles-=1;
                            EpilogeFlagSets();
                            if(debug){
                                cout<<"Jump to Subrountine at address "<<SubroutineJmpAddress<<'\n';
                            }
                        }break;

                        defualt:{
                            cerr<<"Unhandled Instruction\n";
                        }
                    }
                }
           }catch(string msg){
               cerr<<"Error occured : "<<msg<<"\n";
           }
        }
};

