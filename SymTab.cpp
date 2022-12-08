//
//		Implementation of the symbol table class.  This is the format I want for commenting functions.
//
#include "stdafx.h"
#include "SymTab.h"

/*
NAME

    AddSymbol - adds a new symbol to the symbol table.

SYNOPSIS

    void AddSymbol( const string &a_symbol, int a_loc );
    	a_symbol	-> The name of the symbol to be added to the symbol table.
    	a_loc		-> the location to be associated with the symbol.

DESCRIPTION

    This function will place the symbol "a_symbol" and its location "a_loc"
    in the symbol table.
*/
void 
SymbolTable::AddSymbol( const string &a_symbol, int a_loc )
{
    // If the symbol is already in the symbol table, record it as multiply defined.
    map<string, int>::iterator st = m_symbolTable.find( a_symbol );
    if( st != m_symbolTable.end() ) {

        st->second = multiplyDefinedSymbol;
        return;
    }
    // Record a the  location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;
}
/*
NAME

    DisplaySymbolTable - adds a new symbol to the symbol table.

SYNOPSIS

    void DisplaySymbolTable()
       

DESCRIPTION

    .  Display the symbol table.
*/
void SymbolTable::DisplaySymbolTable() {
    map<string, int>::iterator mp;
    cout << "Symbol # :" << "Symbol :" << "Location : " << endl;
    int symbolCount = 0;
    for (mp = m_symbolTable.begin(); mp != m_symbolTable.end(); mp++) {
        cout << symbolCount << " " << mp->first << mp->second << endl;
        symbolCount++;
    }
}

/*
NAME

    LookupSymbol - Lookup a symbol in the symbol table.

SYNOPSIS

    bool LookupSymbol( const string &a_symbol, int &a_loc );
        a_symbol	-> The name of the symbol to be added to the symbol table.
        a_loc		-> the location to be associated with the symbol.

DESCRIPTION

    This function will place the symbol "a_symbol" and its location "a_loc"
    in the symbol table. 
*/
bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc) {
    //If symbol found return true if not return false.
    if (m_symbolTable[a_symbol] == a_loc) {
        return true;
    }
    return false;
}
/*
NAME

   GetValueForKey  - Find the location of a symbol in the symbol table.

SYNOPSIS

    int GetValueForKey( const string &a_symbol, int & a_loc );
        a_symbol	-> The name of the symbol to be added to the symbol table.
         a_loc		-> the location to be associated with the symbol.
        

DESCRIPTION

    This function will determine the value that corresponds to the key "a_symbol".
    The value, "m_symbolTable[a_symbol]", corresponding to "a_symbol".
*/
int SymbolTable::GetValueForKey(const string& a_symbol) {
    return m_symbolTable[a_symbol]; // Return the value corresponding to the symbol given
}