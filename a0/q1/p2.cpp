#include <iostream>
#include <fstream>
#include "pin.H"

using std::cerr;
using std::ofstream;
using std::ios;
using std::string;
using std::endl;
using std::cout;

ofstream OutFile;

// count of total dynamic instructions
static UINT64 ins_count = 0;
// count of total number of loads
static UINT64 loads = 0;
// count of total number of stores
static UINT64 stores = 0;
// count of total number of branches
static UINT64 branch_ins_count = 0;
// count of total number of int ins
static UINT64 int_ins_count = 0;
// count of total number of float ins
static UINT64 float_ins_count = 0;

// This function is called before every instruction that is executed
VOID dynamicInsCount() { ++ins_count; }

// This function is called before every load 
VOID countLoads() { ++loads; }

// This function is called before every store 
VOID countStores() { ++stores; }

// This function is called before every int ins 
VOID intInsCount() { ++int_ins_count; }

// This function is called before every float ins 
VOID floatInsCount() { ++float_ins_count; }

// This function is called before every branch ins 
VOID branchInsCount() { ++branch_ins_count; }
    
// Pin calls this function every time a new instruction is encountered
VOID Instruction(INS ins, VOID *v)
{
	// for dynamic instruction count
	INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)dynamicInsCount, IARG_END);

	// cout<<CATEGORY_StringShort(INS_Category(ins)) << endl;
	
	// for loads
	if (INS_IsMemoryRead(ins)) {
		INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)countLoads, IARG_END);
	}

	// for stores
	if (INS_IsMemoryWrite(ins)) {
		INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)countStores, IARG_END);
	}

	// for int ins
	if (INS_Category(ins) == XED_CATEGORY_BINARY || INS_Category(ins) == XED_CATEGORY_LOGICAL) {
		INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)intInsCount, IARG_END);
	}

	// for float insb
	if (INS_Category(ins) == XED_CATEGORY_X87_ALU) {
		INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)floatInsCount, IARG_END);
	}

	// for branch instructions
	if (INS_IsBranch(ins)) {
		INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)branchInsCount, IARG_END);
	}
}

KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool",
    "o", "p2.out", "specify output file name");

// This function is called when the application exits
VOID Fini(INT32 code, VOID *v)
{
    OutFile.setf(ios::showbase);
    OutFile << "Dynamic Instruction Count: " << ins_count << endl;

	OutFile << "Total Loads: " << loads << endl;

	OutFile << "Total Stores: " << stores << endl;

	OutFile << "Integer Instruction Count: " << int_ins_count << endl;

	OutFile << "Float Instruction Count: " << float_ins_count << endl;

	OutFile << "Branch Instruction Count: " << branch_ins_count << endl;

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

	OutFile.open(KnobOutputFile.Value().c_str());

    // Register Instruction to be called to instrument instructions
    INS_AddInstrumentFunction(Instruction, 0);

    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);
    
    // Start the program, never returns
    PIN_StartProgram();
    
    return 0;
}
