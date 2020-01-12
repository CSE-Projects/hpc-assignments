#include <iostream>
#include <fstream>
#include "pin.H"
using std::cerr;
using std::ofstream;
using std::ios;
using std::string;
using std::endl;

ofstream OutFile;

// count of total number of branches that are taken
static UINT64 branch_taken_count = 0;
// count of total number of forward branches that are taken
static UINT64 fb_taken_count = 0;

// This function is called before every branch which is taken
VOID docount(ADDRINT curr_ins_addr, ADDRINT target_addr) 
{ 
	// taken branch so increment count
	branch_taken_count++; 
	// check if this is forward branch
	if (target_addr > curr_ins_addr) {
		// forward branch which is taken
		fb_taken_count++;
	}
}

// Pin calls this function every time a new instruction is encountered
VOID Instruction(INS ins, VOID *v)
{
	// check if it is a branch instruction
	if (INS_IsBranch(ins)) {
		INS_InsertCall(ins, IPOINT_TAKEN_BRANCH, (AFUNPTR)docount, IARG_INST_PTR, IARG_BRANCH_TARGET_ADDR, IARG_END);
	}
}

// This function is called when the application exits
VOID Fini(INT32 code, VOID *v)
{
    OutFile.setf(ios::showbase);
    OutFile << "Total Taken Branches: " << branch_taken_count << endl;
	OutFile << "Total Taken Forward Branches: " << fb_taken_count << endl;
    OutFile.close();
}

INT32 Usage()
{
    cerr << "This tool counts the number of branches that are taken and forwards branches that are taken" << endl;
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
