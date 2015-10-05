#include "MiniUI.h"
#include <exception>

//Spooky ascii drawing of a skull for intro screen
string face = " _____________________________________________________\n/                                                     \\ \n|               \033[1;31mWelcome to teaching tools! \033[0m           |\n\\_________              ______________________________/ \n          \\_         __/    ___---------__               \n            \\      _/      /              \\_             \n             \\    /       /                 \\            \n              |  /       | _    _ \\          \\           \n              | |       / / \\  / \\ |          \\          \n              | |       ||   ||   ||           |         \n              | |       | \\_//\\\\_/ |           |         \n              | |       |_| (||)   |_______|   |         \n              | |         |  ||     | _  / /   |         \n               \\ \\        |_________|| \\/ /   /          \n                \\ \\_       |_|_|_|_|/|  _/___/           \n                 \\__>       _ _/_ _ /  |                 \n                          .|_|_|_|_|   |                 \n                          |           /                  \n                          |__________/                   \n";

//asccii computer saying welcome
string computer = "            ,----------------,               ,---------,\n        ,-----------------------,          ,\"        ,\"|\n      ,\"                      ,\"|        ,\"        ,\"  |\n     +-----------------------+  |      ,\"        ,\"    |\n     |  .-----------------.  |  |     +---------+      |\n     |  |                 |  |  |     | -==----'|      |\n     |  |  Welcome to     |  |  |     |         |      |\n     |  |  Teaching Tools |  |  |/----|`---=    |      |\n     |  |  TM@tm~: ..     |  |  |   ,/|==== ooo |      ;\n     |  |                 |  |  |  // |(((( [33]|    ,\"\n     |  `-----------------'  |,\" .;'| |((((     |  ,\"\n     +-----------------------+  ;;  | |         |,\"\n        /_)______________(_/  //'   | +---------+\n   ___________________________/___  `,\n  /  oooooooooooooooo  .o.  oooo /,   \\,\"-----------\n / ==ooooooooooooooo==.o.  ooo= //   ,`\\--{)B     ,\"\n/_==__==========__==_ooo__ooo=_/'   /___________,\"\n`-----------------------------'\n";


//Menus
vector< string > options1 = {"CFG", "TM", "UTM"};
pair< string, vector<string> > options2 =  make_pair("CFG", vector<string>({"Load CFG from XML", "Show CFG", "CFG -> PDA", "Put in CNF", "CYK test", "Write CNF to XML"}));
pair< string, vector<string> > options3 =  make_pair("TM", vector<string>({"Load TM from XML", "Show TM", "Set input on TM", "Run TM in console", "Run TM in console with less output", "Run TM to file"}));
pair< string, vector<string> > options4 =  make_pair("UTM", vector<string>({"Load UTM from XML", "Load TM onto UTM", "Set input on UTM", "Show UTM", "Run UTM"}));
vector< pair< string, vector<string> > > options = {options2, options3, options4};

MiniUI::MiniUI() { }
MiniUI::~MiniUI() { }

void MiniUI::LoadCFGfromXML(string input) {
	try {
		_cfgParser = shared_ptr<CFGParser> (new CFGParser(input.c_str()));
		_cfg = shared_ptr<CFG> (new CFG(_cfgParser->getVariables(), _cfgParser->getTerminals(), _cfgParser->getProductionRules(), _cfgParser->getStartSymbol(), _cfgParser->getName()));
		if (_cfg->isConsistent()) cout << *_cfg << endl;
		else cout << *_cfg << endl;
		_cfg_in_cnf = nullptr;
	} catch (exception& e) {
		cout << e.what() << endl;
		cout << "Make sure you selected the right path and that the XML file is valid.\n" << endl;
	}
}

void MiniUI::ShowCFG() {
	if (_cfg->isConsistent()) cout << *_cfg << endl;
	else cout << *_cfg << endl;
}

void MiniUI::CFGtoPDA() {
	FiniteAutomaton pda = _cfg->generatePDA();
	
	char buffer [50];
	time_t t = time (0);
	strftime (buffer, 50, "CFG-TO-PDA %Y-%m-%d %Hh%Mm%Ss.dot", localtime (&t));

	ofstream myfile;
	myfile.open(buffer);
	if(myfile.is_open())  {
		cout << "Success, PDA written to " << buffer << endl;
		writeDotFile(&pda, myfile);
	}    
}

void MiniUI::PutInCNF() {
    if (_cfg_in_cnf == nullptr) {
		cout << "Converting CFG to CNF" << endl;
		CFG2CNF converter;
		_cfg_in_cnf = shared_ptr<CFG> (new CFG(converter(*_cfg)));
	}
	cout << *_cfg_in_cnf << endl;
}

void MiniUI::CYKtest(string input) {
    if (_cfg_in_cnf == nullptr) {
		cout << "Converting CFG to CNF" << endl;
		CFG2CNF converter;
		_cfg_in_cnf = shared_ptr<CFG> (new CFG(converter(*_cfg)));
	} 
	cout << "The string \"" << input << "\" is " << (_cfg_in_cnf->isMember(input) ? "" : "not ") << "a member of the CFG.\n" << endl;
}

void MiniUI::CNFtoXML() {
    if (_cfg_in_cnf == nullptr) {
		cout << "Converting CFG to CNF" << endl;
		CFG2CNF converter;
		_cfg_in_cnf = shared_ptr<CFG> (new CFG(converter(*_cfg)));
	}
	
	char buffer [50];
	time_t t = time (0);
	strftime (buffer, 50, "CNF-TO-XML %Y-%m-%d %Hh%Mm%Ss.xml", localtime (&t));

	_cfg_in_cnf->generateXML(buffer);
	cout << "Success, CFG written to " << buffer << endl;
}

void MiniUI::LoadTMfromXML(string input) {
	try {
		_tmParser = shared_ptr<tum::TuringMachineParser> (new tum::TuringMachineParser(input));
		_tm = _tmParser->getTuringMachine();
		_tmSimulator = _tmParser->getTuringMachineSimulator();
		
		cout << endl;
		cout << *_tm << endl;
		_tmSimulator->outputNextTransitions(true);
		cout << *_tmSimulator << endl;
		
	} catch (exception& e) {
		cout << e.what() << endl;
		cout << "Make sure you selected the right path and that the XML file is valid.\n" << endl;
	}
}

void MiniUI::ShowTM() {
	cout << *_tm << endl;
	cout << *_tmSimulator << endl;
}

void MiniUI::SetInputTM(string input) {
	_tmSimulator = std::make_shared<tum::TuringMachineSimulator>(_tm.get(), std::vector<tum::Tape>({input}));
	cout << endl << *_tm << endl;
	cout << *_tmSimulator << endl;
}

void MiniUI::RunTMconsole() {
	auto simulate = shared_ptr<tum::TuringMachineSimulator> (new tum::TuringMachineSimulator(*_tmSimulator));
	while (!simulate->isHalted()) {
		cout << *simulate << endl;
		simulate->advance();
	}
	cout << *simulate << endl;
}

void MiniUI::RunTMconsoleTiny() {
	auto simulate = shared_ptr<tum::TuringMachineSimulator> (new tum::TuringMachineSimulator(*_tmSimulator));
	simulate->outputNextTransitions(false);
	while (!simulate->isHalted()) {
		cout << *simulate << endl;
		simulate->advance();
	}
	cout << *simulate << endl;
}

void MiniUI::RunTMfile() {
	char buffer [50];
	time_t t = time (0);
	strftime (buffer, 50, "TM-SIMULATION %Y-%m-%d %Hh%Mm%Ss.txt", localtime (&t));
		
	ofstream myfile;
	myfile.open(buffer);
	if(myfile.is_open()) {
		myfile << *_tm << endl;
		
		myfile << *_tmSimulator << endl;
		while (!_tmSimulator->isHalted()) {
			_tmSimulator->advance();
			myfile << *_tmSimulator << endl;
		}
		cout << "Success, simulation written to " << buffer << endl;
		_tmSimulator = _tmParser->getTuringMachineSimulator();
	}   
}

void MiniUI::LoadUTMfromXML(string input) {
	try {
		tum::TuringMachineParser utmParser(input);
		_utm = utmParser.getTuringMachine();
		_tm_on_utm_Parser = nullptr;
		_tm_on_utm = nullptr;
		_utmSimulator = nullptr;
		_converter = nullptr;
		
		char buffer [50];
		time_t t = time (0);
		strftime (buffer, 50, "UTM-SPECIFICATION %Y-%m-%d %Hh%Mm%Ss.txt", localtime (&t));
		
		ofstream myfile;
		myfile.open(buffer);
		myfile << *_utm << endl;
		myfile.close();
		
		cout << "Loaded UTM, its specification was written to " << buffer << endl;
	} catch (exception& e) {
		cout << e.what() << endl;
		cout << "Make sure you selected the right path and that the XML file is valid.\n" << endl;
	}
}

void MiniUI::LoadTMtoUTM(string input) {
	try {
		_tm_on_utm_Parser = shared_ptr<tum::TuringMachineParser> (new tum::TuringMachineParser(input));
		_tm_on_utm = _tm_on_utm_Parser->getTuringMachine();
		
		_converter = shared_ptr<tum::TuringMachineConverter> (new tum::TuringMachineConverter(*_tm_on_utm));
		auto tapes = _converter->encode(_tm_on_utm_Parser->getInitialTapes()->at(0));
		
		_utmSimulator = shared_ptr<tum::TuringMachineSimulator> (new tum::TuringMachineSimulator(_utm.get(), tapes));
		_utmSimulator->setNrEntriesToKeep(1);
		
		cout << "\nLoaded the TM succesfully. \n\nCurrent state of UTM: \n" << *_utmSimulator <<  endl;
	} catch (exception& e) {
		cout << e.what() << endl;
		cout << "Make sure you selected the right path and that the XML file is valid.\n" << endl;
	}
}


void MiniUI::SetInputUTM(string input) {
	_utmSimulator = std::make_shared<tum::TuringMachineSimulator>(_utm.get(), _converter->encode(tum::Tape(input)));
	
	cout << "\nCurrent state of UTM: \n" << *_utmSimulator <<  endl;
}

void MiniUI::ShowUTM() {
	cout << *_utmSimulator << endl;
}

void MiniUI::RunUTM() {
	cout << "Running UTM" << endl;
	
	char buffer [50];
	char buffer2 [50];
	time_t t = time (0);
	strftime (buffer, 50, "UTM-SIMULATION %Y-%m-%d %Hh%Mm%Ss.txt", localtime (&t));
	strftime (buffer2, 50, "UTM-SIMULATION-TINY %Y-%m-%d %Hh%Mm%Ss.txt", localtime (&t));
		
	ofstream myfile;
	myfile.open(buffer);
	ofstream myfile2;
	myfile2.open(buffer2);
	
	auto utmSimulator = std::make_shared<tum::TuringMachineSimulator>(*_utmSimulator);
	utmSimulator->setNrEntriesToKeep(1);
	
	utmSimulator->outputNextTransitions(true);
	myfile << *utmSimulator << endl;
	utmSimulator->outputNextTransitions(false);
	myfile2 << *utmSimulator << endl;
	while (!utmSimulator->isHalted()) {
		utmSimulator->advance();
		utmSimulator->outputNextTransitions(true);
		myfile << *utmSimulator << endl;
		utmSimulator->outputNextTransitions(false);
		myfile2 << *utmSimulator << endl;
	}
	
	myfile << "Turing machine " << (utmSimulator->isDead() ? "is dead" : "halted") << ", stopping now.\n";
	myfile << "Number of steps executed in simulation: " << utmSimulator->nrStepsExecuted() << endl;
	myfile2 << "Turing machine " << (utmSimulator->isDead() ? "is dead" : "halted") << ", stopping now.\n";
	myfile2 << "Number of steps executed in simulation: " << utmSimulator->nrStepsExecuted() << endl;

	if (!utmSimulator->currentStates().size()) {
		utmSimulator->rewind();
	}
		
	cout << "Success, simulation written to " << buffer << endl;
	cout << "Also written to " << buffer2 << " for less output" << endl << endl;
	
	myfile << "Decoded tape of simulated turing machine at the end of the simulation:" << endl;
	myfile2 << "Decoded tape of simulated turing machine at the end of the simulation:" << endl;
	cout << "Decoded tape of simulated turing machine at the end of the simulation:" << endl;
	
	assert(utmSimulator->currentStates().size());
	
	myfile << _converter->decodeSimulatedTape(utmSimulator->currentStates().at(0)->tapes.at(0)) << endl;
	myfile2 << _converter->decodeSimulatedTape(utmSimulator->currentStates().at(0)->tapes.at(0)) << endl;
	cout << _converter->decodeSimulatedTape(utmSimulator->currentStates().at(0)->tapes.at(0)) << endl;
	
	const tum::State& lastState = _tm_on_utm->states().at(_converter->decodeCurrentStateTape(utmSimulator->currentStates().at(0)->tapes.at(2)));
	myfile << "State: " << lastState.name() << endl;
	myfile2 << "State: " << lastState.name() << endl;
	cout << "State: " << lastState.name() << endl;
	if (lastState.isHaltingState()) {
		myfile << "This state is an accept state, so the simulated Turing Machine accepted." << endl;
		myfile2 << "This state is an accept state, so the simulated Turing Machine accepted." << endl;
		cout << "This state is an accept state, so the simulated Turing Machine accepted." << endl;
	} else {
		myfile << "This state is not an accept state, so the simulated Turing Machine died." << endl;
		myfile2 << "This state is not an accept state, so the simulated Turing Machine died." << endl;
		cout << "This state is not an accept state, so the simulated Turing Machine died." << endl;
	}
	
	cout << "\nNumber of steps executed in simulation: " << utmSimulator->nrStepsExecuted() << endl;

	myfile.close();
	myfile2.close();
}

//Print a menu with a (*) at the current options
void MiniUI::printWithChoice(vector<string> &lijst, int i) const {
	for(int j = 0; j < lijst.size(); j++) {
		if (j == i) cout << "*) ";
		else cout << "   ";
		cout << lijst[j] << endl;
	}
}

//Print 2 menus
void MiniUI::printWithBothChoices(vector<string> &lijst1, vector<string> &lijst2, int i, int j) const {
	int a = 0;
	string choice1, choice2, list1, list2;
	
	for (a = 0; a < lijst1.size(); a++) {
		if (a == i) choice1 = "*) ";
		else choice1 = "   ";
		list1 = lijst1[a];
		
		if (a < lijst2.size()) {
			if (a == j) choice2 = "*) ";
			else choice2 = "   ";
			list2 = lijst2[a];
		} else {
			list2 = "";
			choice2 = "";
		}
		
		printf("%-3s%-10s%-3s%-20s\n", choice1.c_str(), list1.c_str(), choice2.c_str(), list2.c_str());
	}
	
	while (a < lijst2.size()) {
		if (a == j) choice2 = "*) ";
		else choice2 = "   ";
		list2 = lijst2[a];
		a++;
		
		printf("%-3s%-10s%-3s%-20s\n", "", "", choice2.c_str(), list2.c_str());
	} 
}

void MiniUI::doIntro() {
	cout << "\033[2J\033[1;1H" << endl;
	cout << face << endl;
	cout << "This program can serve as a tool to help students understand Context-Free Grammars and Turing Machines." << endl;
	cout << "CFG's and TM's can be read from XML files." << endl;
	cout << "The CFG's can be converted to Pushdown Automata and Chomsky Normal Form and be tested with CYK." << endl;
	cout << "Turing Machines can be read out XML files and simulated where each step will be written to the console or a text file." << endl << endl;
	cout << "Additionaly, the program contains a Universal Turing Machine simulator, which can take a 4-tape UTM (see readme) in an XML file and simulate any Turing Machine." << endl << endl;
	cout << "Press any key to continue." << endl;
	getch();
}
		
//Display current menu(s), check input and react accordingly
void MiniUI::doMenu(vector< string > option, int chosen) {
	int choice = 0;
	bool done = false;
	bool write = true;
	
	while(!done) {
		if (!write) {
			write = true;
		} else {
			cout << "\033[2J\033[1;1H" << endl;
			//(\033[2J) clears the entire screen , (\033[1;1H) positions the cursor at row 1, column 1.
			//cout << "\033[1;31mWelcome to teaching tools! \033[0m\n" << endl << endl; //red coloured text
			cout << computer << "\nControl the program with the arrow keys \n\nMake a choice:\n";
			if (option[0] != "CFG") {
				printWithBothChoices(options1, option, chosen, choice);
			} else {
				printWithChoice(option, choice);
			}
			cout << endl;
		}
		
		// Arrow keys come in 3 parts: <escape character> <[> <A, B, C or D>
		if (getch() == 27) { 				// skip escape character
			if (getch() == 91) { 		// skip the [
				switch(getch()) {
					case 'A':			// up
						choice--;
						if (choice < 0) choice = option.size() - 1;
						break;
					case 'B':			// down
						choice++;
						if (choice > option.size() - 1) choice = 0;
						break;
					case 'C':			// right
						if (option[0] == "CFG") {
							for (auto keuze : options) {
								if (option[choice] == keuze.first)
									doMenu(keuze.second, choice);
							}
							done = true;
						} else { 		// do action					
							if (option[choice] == "Load CFG from XML") {
								cout << "Enter the path to your file. (write 'back' to return)" << endl;
								char input [256];
								cin >> input;
								if (strcmp (input,"back") != 0) {
									LoadCFGfromXML(input);
									write = false;
								}
								getch(); //catch the "\n" which for some  reason gets sent to the next loop
							} else if (option[choice] == "CFG -> PDA") {
								if (_cfg == nullptr) {
									cout << "Load a CFG first!" << endl;
								} else 
									CFGtoPDA();
								write = false;
							}  else if (option[choice] == "Show CFG") {
								if (_cfg == nullptr) {
									cout << "Load a CFG first!" << endl;
								} else {
									ShowCFG();
								}
								write = false;
							}else if (option[choice] == "Put in CNF") {
								if (_cfg == nullptr) {
									cout << "Load a CFG first!" << endl;
								} else 
									PutInCNF();
								write = false;
							} else if (option[choice] == "CYK test") {
								if (_cfg == nullptr) {
									cout << "Load a CFG first!" << endl;
								} else {
									cout << "Enter the string you wish to check. (write 'back' to return)" << endl;
									char input [256];
									cin >> input;
									if (strcmp (input,"back") != 0) {
										CYKtest(input);
									}
									getch(); //catch the "\n" which for some reason gets sent to the next loop
								}
								write = false;
							} else if (option[choice] == "Write CNF to XML") {
								if (_cfg == nullptr) {
									cout << "Load a CFG first!" << endl;
								} else {
									CNFtoXML();
								}
								write = false;
							} else if (option[choice] == "Load TM from XML") {
								cout << "Enter the path to your file. (write 'back' to return)" << endl;
								char input [256];
								cin >> input;
								if (strcmp (input,"back") != 0) {
									LoadTMfromXML(input);
									write = false;
								}
								getch(); //catch the "\n" which for some reason gets sent to the next loop
							} else if (option[choice] == "Run TM in console") {
								if (_tmSimulator == nullptr) {
									cout << "Load a TM first!" << endl;
								} else {
									RunTMconsole();
								}
								write = false;
							} else if (option[choice] == "Run TM in console with less output") {
								if (_tmSimulator == nullptr) {
									cout << "Load a TM first!" << endl;
								} else {
									RunTMconsoleTiny();
								}
								write = false;
							} else if (option[choice] == "Show TM") {
								if (_tmSimulator == nullptr) {
									cout << "Load a TM first!" << endl;
								} else {
									ShowTM();
								}
								write = false;
							} else if (option[choice] == "Set input on TM") {
								if (_tmSimulator == nullptr) {
									cout << "Load a TM first!" << endl;
								write = false;
								} else {
									cout << "Enter the string you wish to check. (write 'back' to return)" << endl;
									char input [256];
									cin >> input;
									if (strcmp (input,"back") != 0) {
										SetInputTM(input);
										write = false;
									}
									getch(); //catch the "\n" which for some reason gets sent to the next loop
								}
							} else if (option[choice] == "Run TM to file") {
								if (_tmSimulator == nullptr) {
									cout << "Load a TM first!" << endl;
								} else {
									RunTMfile();
								}
								write = false;
							} else if (option[choice] == "Load UTM from XML") {
								cout << "Enter the path to your file. (write 'back' to return)" << endl;
								char input [256];
								cin >> input;
								if (strcmp (input,"back") != 0) {
									LoadUTMfromXML(input);
									write = false;
								}
								getch(); //catch the "\n" which for some reason gets sent to the next loop
							} else if (option[choice] == "Load TM onto UTM") {
								if (_utm == nullptr) {
									cout << "Load a UTM first!" << endl;
									write = false;
								} else {
									cout << "Enter the path to your file. (write 'back' to return)" << endl;
									char input [256];
									cin >> input;
									if (strcmp (input,"back") != 0) {
										LoadTMtoUTM(input);
										write = false;
									}
									getch(); //catch the "\n" which for some reason gets sent to the next loop
								}
							} else if (option[choice] == "Set input on UTM") {
								if (_utm == nullptr) {
									cout << "Load a UTM first!" << endl;
									write = false;
								} else {
									if (_tm_on_utm == nullptr) {
										cout << "Load a TM on the UTM first!" << endl;
										write = false;
									} else {
										cout << "Enter the string you wish to check. (write 'back' to return)" << endl;
										char input [256];
										cin >> input;
										if (strcmp (input,"back") != 0) {
											SetInputUTM(input);
											write = false;
										}
										getch(); //catch the "\n" which for some reason gets sent to the next loop
									}
								}
							} else if (option[choice] == "Show UTM") {
								if (_utm == nullptr) {
									cout << "Load a UTM first!" << endl;
								} else {
									if (_tm_on_utm == nullptr) {
										cout << "Load a TM on the UTM first!" << endl;
									} else {
										ShowUTM();
									}
								}
									write = false;
							} else if (option[choice] == "Run UTM") {
								if (_utm == nullptr) {
									cout << "Load a UTM first!" << endl;
								} else {
									if (_tm_on_utm == nullptr) {
										cout << "Load a TM onto the UTM first!" << endl;
									} else {
										RunUTM();
									}
								}
								write = false;
							}
						}
						break;
					case 'D':			// left
						if (option[0] != "CFG") {
							doMenu(options1, 0);
						}
						done = true;
						break;
				}
			} 
		}
	}
}

void MiniUI::mainLoop() {
	this->doIntro();
	this->doMenu(options1, 0);
}
