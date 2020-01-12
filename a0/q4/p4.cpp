#include <iostream>
#include <fstream>
#include "pin.H"
#include <unordered_map> 
#include <utility> 

using std::cerr;
using std::ofstream;
using std::ios;
using std::string;
using std::endl;
using std::unordered_map;
using std::pair;
using std::make_pair;
using std::cout;

ofstream OutFile;

// counts total number of read after write (RAW) dependency
static UINT64 rawcount = 0;
// counts of number of write after read (WAR) dependency
static UINT64 war_count = 0;
// counts of number of write after write (WAW) dependency
static UINT64 waw_count = 0;

// Create a map for storing memory access history until the current instruction
// key: memory address, value: (first: no of time read, second: no of time written into) 
unordered_map<ADDRINT, pair<UINT64, UINT64> > mem_access_history;

// This function is called before every instruction is executed
VOID dataDependencyCount(ADDRINT mem_addr, bool is_read)  
{
	// check if this address has been accessed earlier
	if (mem_access_history.find(mem_addr) != mem_access_history.end()) {
		// read operation
		if (is_read) {
			// all earlier writes to this memory will be a dependency to this read
			raw_count += mem_access_history[mem_addr].second;
			// update
			mem_access_history[mem_addr].first++;
		}
		// write operation
		else {
			// all earlier reads to this memory will be a dependency to this write
			war_count += mem_access_history[mem_addr].first;
			// all earlier write to this memory will be a dependency to this write
			waw_count += mem_access_history[mem_addr].second;
			// update
			mem_access_history[mem_addr].second++;
		}
	}
	else {
		UINT64 read_count = 0; UINT64 write_count = 0;
		if (is_read) { read_count = 1; }
		else { write_count = 1; }
		// new memory address access
		mem_access_history[mem_addr] = make_pair(read_count, write_count);
	}
}

// Pin calls this function every time a new instruction is encountered
VOID Instruction(INS ins, VOID *v)
{
	UINT32 memOperands = INS_MemoryOperandCount(ins);

    // Iterate over each memory operand of the instruction.
    for (UINT32 memOp = 0; memOp < memOperands; memOp++) {
		if (INS_MemoryOperandIsWritten(ins, memOp)) {
			// write operand 
			INS_InsertPredicatedCall(
			ins, IPOINT_BEFORE, (AFUNPTR)dataDependencyCount,
			IARG_MEMORYOP_EA, memOp,
			IARG_BOOL, false,
			IARG_END);
		}
		else if (INS_MemoryOperandIsRead(ins, memOp)) {
			// read operand 
			INS_InsertPredicatedCall(
			ins, IPOINT_BEFORE, (AFUNPTR)dataDependencyCount,
			IARG_MEMORYOP_EA, memOp,
			IARG_BOOL, true,
			IARG_END);
		}
    }	
}

// This function is called when the application exits
VOID Fini(INT32 code, VOID *v)
{
    OutFile.setf(ios::showbase);
    OutFile << "RAW Dependency Count: " << raw_count << endl;

	OutFile << "WAR Dependency Count: " << war_count << endl;

	OutFile << "WAW Dependency Count: " << waw_count << endl;

    OutFile.close();
}

INT32 Usage()
{
    cerr << "This tool counts the RAW, WAR and WAW dependencies in the dynamic instruction stream" << endl;
    cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}

int main(int argc, char * argv[])
{
    // Initialize pin
    if (PIN_Init(argc, argv)) return Usage();

    // Register Instruction to be called to instrument instructions
    INS_AddInstrumentFunction(Instruction, 0);

    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);
    
    // Start the program, never returns
    PIN_StartProgram();
    
    return 0;
}
