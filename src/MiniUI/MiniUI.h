#ifndef MiniUI_H_
#define MiniUI_H_

#include <time.h>
#include <memory>
#include <fstream>
#include <stdio.h>
#include <stdio.h>
#include <iomanip>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <termios.h>

#include <UTM/util.h>
#include <UTM/dot.h>
#include <UTM/CFG.h>
#include <UTM/CFG2CNF.h>
#include <UTM/CFGParser.h>
#include <UTM/finiteautomaton.h>
#include <UTM/TuringMachine.h>
#include <UTM/TMState.h>
#include <UTM/TMTransition.h>
#include <UTM/TuringMachineSimulator.h>
#include <UTM/TuringMachineParser.h>
#include <UTM/TuringMachineEncodingConverter.h>


using namespace std;

//Function which takes a character without waiting for a newline/carriage return, it will take input without anything coming up in the console
//tcgetattr gets settings for terminal
/**
 * @brief Get a character.
 * @details Gets a character without waiting for a newline/charriage return. It will take input without anything coming up in the console.
 */
int getch( ) {		
  struct termios oldt, newt;
  int ch;
  tcgetattr( STDIN_FILENO, &oldt ); 			//Get terminal settings and put them in oldt
  newt = oldt;									//Put the settings in newt
  newt.c_lflag &= ~( ICANON | ECHO ); 			//Change some setting, probably turns off ECHO or so
  tcsetattr( STDIN_FILENO, TCSANOW, &newt ); 	//Set the new settings in the terminal
  ch = getchar();								//Take a character from terminal
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt );	//Set the old terminal settings
  return ch;									//^That's what i think it does, i honestly have no idea
}

/**
 * @brief User interface class.
 * @details Handles the user interface for the UTM project.
 */
class MiniUI {
	private:
		string _message;
		shared_ptr<CFGParser> _cfgParser;
		shared_ptr<CFG> _cfg;
		shared_ptr<CFG> _cfg_in_cnf;
		std::shared_ptr<tum::TuringMachine> _tm;
		std::shared_ptr<tum::TuringMachine> _utm;
		std::shared_ptr<tum::TuringMachine> _tm_on_utm;
		shared_ptr<tum::TuringMachineParser> _tmParser;
		shared_ptr<tum::TuringMachineParser> _tm_on_utm_Parser;
		shared_ptr<tum::TuringMachineSimulator> _tmSimulator;
		shared_ptr<tum::TuringMachineSimulator> _utmSimulator;
		shared_ptr<tum::TuringMachineConverter> _converter;
		
	public:
    /*! @brief Default constructor */
		MiniUI();
    /*! @brief Default destructor */
		~MiniUI();
		
    /**
     * @brief Load a CFG.
     * @param input XML file describing a CFG.
     */
		void LoadCFGfromXML(string input);
		void ShowCFG(); //!< Show the CFG.
		void CFGtoPDA();  //!< Convert the CFG to PDA.
		void PutInCNF();  //!< Put the CFG in CNF.
    /**
     * @brief Test a string for membership.
     * @details Use CYK algorithm for membership testing.
     * @param input The string to be tested.
     */
		void CYKtest(string input);
		void CNFtoXML();  //!< Write an CFG (in CNF) to an XML file.
		
    /**
     * @brief Load a Turing Machine.
     * @param input XML file describing an Turing Machine.
     */
		void LoadTMfromXML(string input);
		void ShowTM();  //!< Show the TM.
		void SetInputTM(string input);  //!< Render a TM using a TMSimulator.
		void RunTMconsole();  //!< Run the TM - verbose.
		void RunTMconsoleTiny();  //!< Run the TM.
		void RunTMfile(); //!< Run the TM - logfile.
		
    /**
     * @brief Load the UTM.
     * @param input XML file describing the UTM.
     */
		void LoadUTMfromXML(string input);
    /**
     * @brief Load a TM in the UTM.
     * @param input The XML file describing an arbitrary TM.
     */
		void LoadTMtoUTM(string input);
    /**
     * @brief Load the UTM in the simulator.
     * @param input XML file containing the UTM.
     */
		void SetInputUTM(string input);
		void ShowUTM();	//!< Show the UTM.
		void RunUTM();  //!< Run the UTM simulation.
			
		void printWithChoice(vector<string> &lijst, int i) const; //!< Print a menu with a (*) at the current option
		void printWithBothChoices(vector<string> &lijst1, vector<string> &lijst2, int i, int j) const;  //!< Print 2 menus with a (*) at the current options
		
		void doIntro(); //!< Show the introduction screen.
		void doMenu(vector<string> option, int chosen); //!< Display current menu(s), check input and react accordingly
		void mainLoop();  //!< Run the mainloop.
};

#endif //MiniUI_H_
