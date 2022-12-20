#include "stdafx.h"
#include "Instruction.h"
#include "Errors.h"
using namespace std;
/*
NAME

	ParseInstruction - Look for the command in machine learning and parse it.

SYNOPSIS

	Instruction::InstructionType Instruction::ParseInstruction(string a_line)
		a_line	-> The name of the symbol to be added to the symbol table.
		

DESCRIPTION

	This function will place the symbol "a_symbol" and its location "a_loc"
	in the symbol table.
*/
Instruction::InstructionType Instruction::ParseInstruction(string a_line)
{
	m_instruction = a_line;
	//Remove any comments.
	//RemoveComment(a_line);
	 size_t pos= a_line.find(';');
	if (pos == string::npos) a_line.erase(pos);
	//Parse the line.
	bool rv = ParseLine(a_line, m_Label, m_OpCode, m_Operand);

	//Capilizing the Commands 
	string x_OpCode =CommnadsCap(m_OpCode); //this is part of the issue it reads "org"
	//Machine Learning Code:
	string machineCode[]{ "ADD","SUB","MULT", "DIV", "LOAD","STORE","READ", "WRITE", "BRAN", "BM","BZ","BP","HALT" };
	//Assembler Instructions
	string assemblerCode[]{ "DC","DS","ORG" };
	//If  m_instruction is empty or has a comment it return to comment
	if (m_instruction.empty() || m_instruction.at(0) == ';') {
		return ST_Comment;
	}
	if (!m_Label.empty() && m_OpCode.empty()) {
		Errors::RecordError("Error Detected");

	}
	int j = 0; // an easy variable to intialized
	while (m_instruction[j] == ' ' && j != m_instruction.length()) {
		j++;
	}
	//The End Instructions
	if ( m_instruction.substr(j, 3) == "END") {
		return InstructionType::ST_End;
	}
	//The Halt Instructions
	if (m_instruction.substr(j, 4) == "HALT") {
		return InstructionType::ST_MachineLanguage;
	}
	istringstream inputs(a_line);
	string temp1, temp2, temp3, temp4;
	inputs >> temp1 >> temp2 >> temp3 >> temp4;
	
	if (a_line[0] == ' ') {//If it has a space then there is no label
		m_Label = "";
		m_OpCode = temp1;
		m_Operand = temp2;

		if (!temp3.empty()) {
			 Errors::RecordError("Error Detected");
		}
	}
	else {// Then Label if there is.
		m_Label = temp1;
		m_OpCode = temp2;
		m_Operand = temp3;
		// If temp 4 is  not empty, report an error.
		if (!temp4.empty()) {
			string msg = "Error: Temp 4 is not empty";
			Errors::RecordError(msg);
		}

	}
	if (find(begin(assemblerCode), end(assemblerCode), m_OpCode) != end(assemblerCode)) { // Assembler Instructions.
		m_type = InstructionType::ST_AssemblerInstr;
	}
	else if (find(begin(machineCode), end(machineCode), m_OpCode) != end(machineCode)) { // Machine Language Instruction.
		m_type = InstructionType::ST_MachineLanguage;
	}
	else { // If it not any of the Instructions thrown an error.
		string msg = "Error: Invalid/Illegal Instructions";
		Errors::RecordError(msg);
	}
	return m_type;
}
/*
NAME

	 RemoveComment- Removes comments

SYNOPSIS

	void Instruction::RemoveComment(string& a_line) 
	a_line	-> The name of the symbol to be added to the symbol table.

DESCRIPTION

	This function will remove any comments with the test.
*/

void Instruction::RemoveComment(string& a_line) 
{
	size_t pos= a_line.find(';');
	if (pos == string::npos) return;

	a_line.erase(pos);
}
/*
NAME

	 ParseLine- Look for the command in machine learning and parse it.

SYNOPSIS

	bool Instruction::ParseLine(const string& a_line, string& a_label, string& a_opcode, string& a_operand)
		a_line	-> The name of the symbol to be added to the symbol table.
		a_label		-> A memeber to hold other objects in it.


DESCRIPTION

	This function will place the symbol "a_symbol" and its location "a_loc"
	in the symbol table.
*/
bool Instruction::ParseLine(const string& a_line, string& a_label, string& a_opcode, string& a_operand)
{
	a_label = a_opcode = a_operand = "";
	if (a_line.empty()) {
		return true;
	}
	istringstream ins(a_line);
	if (a_line[0] != ' ' && a_line[0] != '\t') 
	{
		ins >> a_label;
	}
	string temp;
	ins >> a_opcode >> a_operand >> temp;
	return temp == "";
	
}
/*
NAME

	 CommandCap- Look for the command in machine learning and parse it.

SYNOPSIS

		string Instruction::CommnadsCap( string& a_opcode
		a_opcode	-> The name of the symbol to be added to the symbol table.


DESCRIPTION

	This function will capilize the command from machine learning.
*/
string Instruction::CommnadsCap( string a_opcode) {
	char c[6] = "";
	for (int i = 0; i < a_opcode.size(); i++) {
		a_opcode[i]=toupper(a_opcode[i]);

	}
	return a_opcode;
}
/*
NAME

	int LocationNextInstruction()- Locates the next instructions.

SYNOPSIS

		int LocationNextInstruction(int a_loc)
		a_loc -> Find the location as a variable.


DESCRIPTION

	This function will compute the location of the next instructions.

RETURNS
	
	It returns a_loc+1 unless the instruction is DS or ORG. 
	Which if it occurs the case of the location is added to the previous one.


*/
int Instruction::LocationNextInstruction(int a_loc)
{
	

	if (m_OpCode == "ORG" || m_OpCode=="DS") //returns the base case, loc +1 if oprend is not numeric.
	{
		return a_loc +m_Operand1Value;
	}
	else return a_loc + 1;

	/*if (m_OpCode == "DC") //returns the base case, loc +1 if oprend is not numeric.
	{
		for(int i=0; i<m_Operand.length(); i++){
			if(!isdigit(m_Operand.at(i))) return a_loc+1;
		}
		return a_loc +stoi(m_Operand);
	}
	 / If DS or ORG it will add the operand onto the location
	if (m_OpCode == "DS") //returns the base case, loc +1 if oprend is not numeric.
	{
		for (int i = 0; i < m_Operand.length(); i++) {
			if (!isdigit(m_Operand.at(i))) return a_loc + 1;
		}
		return a_loc + stoi(m_Operand);
	}*/
	//Add 1 to the location.
	
	
}

/*
NAME

	CompNumCode()- Declare the address of the Instructions.

SYNOPSIS

		int Instruction::CompNumCode()
		Part for Pass2


DESCRIPTION

	This function will capilize the command from machine learning.
*/
int Instruction::CompNumCode(){
	if (m_OpCode == "ADD") {
		m_NumOpCode = 1; // ADD
	}
	else if (m_OpCode == "SUB" ) {
		m_NumOpCode = 2; // SUBTRACT
	}
	else if (m_OpCode == "MULT" ) {
		m_NumOpCode = 3; // MULT
	}
	else if (m_OpCode == "DIV" ) {
		m_NumOpCode = 4; // DIV
	}
	else if (m_OpCode == "LOAD" ) {
		m_NumOpCode = 5; // LOAD
	}
	else if (m_OpCode == "STORE" ) {
		m_NumOpCode = 6; // STORE
	}
	else if (m_OpCode == "READ" ) {
		m_NumOpCode = 7; // READ
	}
	else if (m_OpCode == "WRITE" ) {
		m_NumOpCode = 8; // WRITE
	}
	else if (m_OpCode == "B" ) {
		m_NumOpCode = 9; // B
	}
	else if (m_OpCode == "BM" ) {
		m_NumOpCode = 10; // BM
	}
	else if (m_OpCode == "BZ" ) {
		m_NumOpCode = 11; // BZ
	}
	else if (m_OpCode == "BP" ) {
		m_NumOpCode = 12; // BP
	}
	else if (m_OpCode == "HALT" ) {
		m_NumOpCode = 13; // HALT
	}
	else {
		m_NumOpCode = 0; // This means it has to be an assembler instruction
	}
	return m_NumOpCode;
}
