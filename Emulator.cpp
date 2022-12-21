#include "stdafx.h"
#include "Emulator.h"
#include "Errors.h"
#include <iostream>
using namespace std;
/*
NAME

	insertMemory  - Look for the command in machine learning and parse it.

SYNOPSIS

	bool Emulator::insertMemory(int a_location, int a_contents) 


DESCRIPTION

	This function will  its location "a_location" if it > MEMESZ it returns false if not it allocated to m_meory.
*/

bool emulator::insertMemory(int a_location, int a_contents) {
	if (a_location <= 0 || a_location>= MEMSZ) {
		return false;
	}
	m_memory[a_location] = a_contents;
	return true;
}

/*
NAME

	runProgram() - Run the program to translate it.

SYNOPSIS

	bool Emulator::runProgram()


DESCRIPTION

	This function will run the translation of the program.

RETURNS
	
	This function will return an FALSE if we exceed the max memory
*/
bool emulator::runProgram() {
	int loc = 100;
	bool finish = false;
	m_acc = 0;
	cout << "Emulator Results are: " << endl;
	while (!finish) {
		if (loc >= 10000) {//check if memory size exceeds
			string msg = " Error: Location Exceeded Memory Size, Emulator Terminated.";
			Errors::RecordError(msg);
			Errors::DisplayErrors();
			return false;
		}
		int instr = m_memory[loc];
		int opCode = instr / 10000;
		int addr = instr % 10000;
		switch (opCode) {
		case 1: // This is ADD: It adds the value to the accumulator.
			m_acc += m_memory[addr];
			loc++;
			break;
		case 2:// This is SUBTRACT: It subtract the value to the accumulator.
			m_acc -= m_memory[addr];
			loc++;
			break;
		case 3:// This is MULTIPLY:  Multiply the value together in the accumulator.
			m_acc *= m_memory[addr];
			loc++;
			break;
		case 4: // This is DIVIDE: Divide the value togther in the accumulator.
			m_acc /= m_memory[addr];
			if (m_memory[addr] == 0) {
				string msg = "Error: Can not Divide by Zero.";
				Errors::RecordError(msg);
			}
			loc++;
			break;
		case 5: // This is LOAD: Load set the accumulator equal to the same value in the memory.
			m_acc = m_memory[addr];
			loc++;
			break;
		case 6: // This is STORE: It stores the accumulator contents in the address.
			addr = m_acc;
			loc++;
			break;
		case 7: // This is READ: Read ask the user to enter a number for it to be placed in memory.
			cout << "?";
			cin >> m_memory[addr];
			loc++;
			break;
		case 8: // This is WRITE: Write displays the number on the computer.
			cout << m_acc << endl;
			loc++;
			break;
		case 9: // This is BRANCH: Branch loops back the given label.
			loc = addr;
			break;
		case 10: // This is BRANCH MINUS: BM will Loop back if accumulator has negative value.
			if (m_acc < 0) loc = addr;
			else loc++;
			break;
		case 11: // This is BRANCH ZERO: BZ will loop back if accumulator value is zero.
			if (m_acc == 0) loc = addr;
			else loc++;
			break;
		case 12: // This is BRANCH POSITIVE: BP will loop back if accumulator value is positive.
			if (m_acc > 0) loc = addr;
			else loc++;
			break;
		case 13: // This is HALT : Halt terminates execution.
			finish = true;
			break;
		}
	}
	cout << "The Emulator ending!" << endl;
	Errors::DisplayErrors();
	return true;
}