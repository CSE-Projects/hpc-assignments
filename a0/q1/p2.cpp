#include <iostream>
#include <fstream>
#include "pin.H"
using std::cerr;
using std::ofstream;
using std::ios;
using std::string;
using std::endl;

ofstream OutFile;

// count of total dynamic instructions
static UINT64 ins_count = 0;
// count of total number of loads
static UINT64 loads = 0;
// count of total number of stores
static UINT64 stores = 0;
// count of total number of branches
static UINT64 branch_Ins_Count = 0;

// This function is called before every instruction that is executed
VOID dynamicInsCount() { ++ins_count; }

// This function is called before every load 
VOID countLoads() { ++loads; }

// This function is called before every store 
VOID countStores() { ++loads; }

// This function is called before every branch ins 
VOID branchInsCount() { ++branch_ins_count; }
    
// Pin calls this function every time a new instruction is encountered
VOID Instruction(INS ins, VOID *v)
{
	// for dynamic instruction count
	INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)dynamicInsCount, IARG_END);

	fprintf(trace, "==> %s\n", CATEGORY_StringShort(INS_Category(ins)));
	
	// for loads
	if (INS_IsMemoryRead(ins)) {
		INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)countLoads, IARG_END);
	}

	// for stores
	if (INS_IsMemoryWrite(ins)) {
		INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)countStores, IARG_END);
	}

	// for branch instructions
	if (INS_IsBranch(ins)) {
		INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)branchInsCount, IARG_END);
	}
}

// This function is called when the application exits
VOID Fini(INT32 code, VOID *v)
{
    OutFile.setf(ios::showbase);
    OutFile << "Dynamic Instruction Count: " << icount << endl;

	OutFile << "Total Loads: " << lcount << endl;

	OutFile << "Total Stores: " << scount << endl;

	OutFile << "Branch Instruction Count: " << bcount << endl;

    OutFile.close();
}

INT32 Usage()
{
    cerr << "This tool gives the Instruction mix: dynamic instructions, integer ops, floating ops, loads, stores and branch ins" << endl;
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
