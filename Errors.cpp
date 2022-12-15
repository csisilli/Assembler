#include "stdafx.h"
#include "Errors.h"

// if not declared it will report an error
vector<string> Errors::m_ErrorMsgs;

/*
NAME

	InitErrorReporting -Error reports.

SYNOPSIS

	void Errors::InitErrorReporting()
		


DESCRIPTION

	This function will initizale errors and report it.
*/
void Errors::InitErrorReporting() {
	for (int i = 0; i < m_ErrorMsgs.size(); i++) {
		m_ErrorMsgs.at(i) = "";
	}
};
/*
NAME

	RecordError -Error reports.

SYNOPSIS

	 void Errors::RecordError(string a_emsg)
		a_emsg	-> Leaves a message if error occurs


DESCRIPTION

	This function will record and collected error message.
*/
 void Errors::RecordError(string a_emsg) {
    m_ErrorMsgs.emplace_back(a_emsg);
};
 /*
 NAME

	 DisplayErrors -Error reports.

 SYNOPSIS

	   void Errors::DisplayErrors()

 DESCRIPTION

	 This function will display errors.
 */
 void Errors::DisplayErrors() {
	 cout << "Errors collected: ";
	 if (m_ErrorMsgs.size() == 0) {
		 cout << "None" << endl;
		 return;
	 }
	 cout << "Hello Errors" << endl;
	 cout << endl;
	 for (int i = 0; i < m_ErrorMsgs.size(); i++) {
		 cout << "Number of Errors are: " << m_ErrorMsgs.at(i) << endl;
	 }
 };