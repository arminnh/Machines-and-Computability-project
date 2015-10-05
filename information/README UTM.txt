Het UTM programma gebruikt een transitietabel voor de Universtele Turing Machine opgesteld in een XML bestand.
Hoe zo'n transitietabel werkt wordt in dit bestand uitgeled, alsook hoe een UTM werkt.

De nodige XML file kan gegenereerd worden met het python script generateXMLfromTransitionsUTM.py. 
UTM Transitions.ods bevat een overzichtelijke transitie tabel voor de Universele Turing Machine.
transitionsUTM.txt bevat dezelfde tabel, in gewoon text formaat (copy paste van de .ods file).
Door het python script kunnen op een makkelijke manier veranderingen/bugfixes/optimalisaties gebeuren aan de universele turing machines.
De verandering kan gemaakt worden in de tabel, dan zal het script het omzetten in een geldige xml file.





Hoe werkt de Universele Turing Machine?

Je kan een Turing Machine bekijken als iets dat een algoritme uitvoert.
Zo zijn er TM's die een string van vorm 0*1* kunnen accepteren, of TM's die "proper subtraction" kunnen doen.
Voor elk zo'n algoritme ga je dus een TM maken.
Een Universal Turing Machine is zelf ook iets dat een algoritme uitvoert, maar in dit geval is het een algoritme dat andere algoritmes gaat uitvoeren.
Je kan dan één UTM maken die alle TMs kan uitvoeren, dan moet je niet elke keer een nieuwe TM bouwen voor elk algorime maar het gewoon in de UTM zetten en die zal het uitvoeren.

Een Turing Machine wordt gedefinieerd als onderstaand 7-tuple:
TM = (States, Input symbols, Tape symbols, Transitions, Start State, Blank Symbol, Final State) 
Voor de UTM geldt hetzelfde = (States, Input symbols, Tape symbols, Transitions, Start State, Blank Symbol, Final State) 
De Final State is echter niet verplicht.

Er moet dus een vaste verzameling (States, Input symbols, ... , Final State) zijn die we moeten vinden.
Zo een verzameling hebben we gevonden (let op: er is niet slechts 1 correcte verzameling).

Onz UTM is een multitape TM met 4 tapes, input/tape symbols {"0", "1", ".", ";", "B"}, 32 staten en 125 transities (transities staan in Transitions.ods).





Hoe werken transities in een Multitape Turing Machine?

Bij een Multitape Turing Machine gaan we kijken naar de combinatie van de symbolen die de tape heads lezen.
Dan beschrijven we wat elke tape moet doen.
Bijvoorbeeld: Tape head 1 leest "0", Tape head 2 leest "4", Tape head 3 leest "a", Tape head 4 leest "1".
              We hebben dan de input combinatie (0, 4, a, 1)
              Daarop kunnen we elke Tape head iets laten doen, bv. Tape head 1 (schrijf 1, beweeg links), Tape head 2 ...





Beschrijving van de tapes van de UTM:
	1: Simulation Tape, de tape dat de simulated TM zou hebben
	2: Beschrijving van de simulated TM in vorm van <blank symbol>B<transitions>, meer uitleg hier onder.
	3: De current state van de simulated TM
	4: "work tape", bevat <current state>";"<current symbol>
	   current state staat voor de state van simulated TM, current symbol staat voor het symbool waar de tape head van tape 1 naar wijst

Op tape 2 staat een encodering van de simulated TM in vorm: <blank symbol>B<transitions>
De encodering voor een Turing Machine:
	De TM zal worden veranderd naar een string van 0'en, 1'en, "." en ";" als volgt:
	Voor elk tape symbol van de TM: het eerste tape symbool wordt 1, het tweede wordt 11, het derde wordt 111, enz.
									Dus: verander symbool i naar een string van i 1'en
	Voor elke state van de TM: doe hetzelfde
	
	We willen dan dat elk symbool dezelfde lengte heeft, dit zal helpen bij het vergelijken/lezen/schrijven bij het uivoeren van de UTM.
	Dit wordt gedaan door elk symbool aan te vullen met "."'tjes tot het symbool even lang is als het langste symbool.
	Doe hetzelfde voor elke staat.
	Op de tapes zullen 0'en staan tussen de 1'en om de symbolen van elkaar te splitsen
	
	Voor elke transitie:
		Maak een string van vorm <current state>";"<current symbol>";"<newState>";"<newSymbol>";"<direction> met de net gecodeerde symbolen
		De ";" onderscheidt elk deel van de transitie van elkaar
		
	Op tape 2 kan je dan <blank symbol>B<transitions> zetten:
		<blank symbol> is de gecodeerde versie van het Blank Symbol van de simulated TM, dus iets als bv 1111
		B wordt gebruikt als blank symbol van de UTM
		<transitions> is een opeenvolging van gecodeerde transities in de vorm van hier boven, elk met een 0 er tussen om ze te splitsen van elkaar
		
	Voor de direction van elke transitie:
		r -> 1, l -> 11, s -> 111





VOORBEELD:
//Turing machine op http://morphett.info/turing/turing.html
Voorbeeld TM M: 
	States: 1, 2, 3, 4, 5, 6, halt
	Tape Symbols: 0, 1, x, B
	Start State: 0
	Blank Symbol: B
	
	Transitions: 	<current state> <current symbol> <new state> <new symbol> <direction>
					0 0 0 0 r
					0 1 1 x r
					1 1 1 1 r
					1 0 2 0 r
					2 0 2 0 r
					2 1 3 1 r
					3 1 3 1 r
					3 0 4 1 l
					3 B 4 1 l
					4 1 4 1 l
					4 0 5 0 l
					5 0 5 0 l
					5 1 6 1 l
					5 x halt x x
					6 1 6 1 l
					6 x 0 x r
					6 0 0 0 r


Encoderen van M voor de UTM:
	States: 0 -> 1......., 1 -> 11......, 2 -> 111....., 3 -> 1111...., 4 -> 11111..., 5 -> 111111.., 6 -> 1111111., halt -> 11111111
	Input/Tape Symbols: 0 -> 1..., 1 -> 11.., x -> 111., B -> 1111
	Start State:  Neem encoding van 0 van er boven, 0 -> 1.......
	Blank Symbol: Neem encoding van B van er boven, B -> 1111
	
	Transitions: 
		Transitions hebben de vorm:
			<current state>";"<current symbol>";"<new state>";"<new symbol>";"<direction>"0" (de 0 om aan te duiden dat er een andere transitie aan komt)
		Voorbeeld:
			0 0 0 r 0 -> (Encoding van staat 0)";"(Encoding van Tape Symbol 0)";"(Encoding van staat 0)";"(Encodeer r -> 1)";"(Encoding van Tape Symbol 0)"0"
		
		DUS: 
		0 0 0 0 r -> 1.......;1...;1.......;1...;10
		0 1 1 x r -> 1.......;11..;11......;111.;10
		1 1 1 1 r -> 11......;11..;11......;11..;10
		1 0 2 0 r -> 11......;1...;111.....;1...;10
		enzovoort
		
		De laatste transitie heeft geen 0 op het einde, dit betekent dat er dan geen transitie meer komt er na
		
	Initialiseren van UTM met de gecodeerde TM:
		Tape 1 bevat de input van gesimuleerde TM
		Tape 2 bevat <blank symbol>B<transitions>
		Tape 3 bevat <current state>
		Tape 4 is leeg





Elke TM kan gecodeerd worden op deze manier.
In het begin zal met deze TM (met als input 11110011111) de UTM er dan uitzien als volgt:

Simulated Tape
------------------------------------------------------------------------------------------------------------------------------
11..011..011..011..01...01...011..011..011..011..011.. 
------------------------------------------------------------------------------------------------------------------------------

Encoded TM tape
------------------------------------------------------------------------------------------------------------------------------
1111B1.......;1...;1.......;1...;101.......;11..;11......;111.;1011......;11..;11......;11..;1011......;1...;111.....;1...;10111.....;1...;111.....;1...;10111.....;11..;1111....;11..;101111....;11..;1111....;11..;101111....;1...;11111...;11..;1101111....;1111;11111...;11..;11011111...;11..;11111...;11..;11011111...;1...;111111..;1...;110111111..;1...;111111..;1...;110111111..;11..;1111111.;11..;110111111..;111.;11111111;111.;111.01111111.;11..;1111111.;11..;1101111111.;111.;1.......;111.;101111111.;1...;1.......;1...;1
------------------------------------------------------------------------------------------------------------------------------

Current State tape
------------------------------------------------------------------------------------------------------------------------------
1.......
------------------------------------------------------------------------------------------------------------------------------

Work tape
------------------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------------





Hoe gaat de UTM nu de TM simuleren?
De UTM kan zelf voorgesteld worden door een algoritme om wat duidelijker te maken hoe het werkt, dan kan een transitie tabel voor de UTM worden gemaakt die het algoritme volgt.
Het algoritme zelf is simpel, de UTM wordt ge-initialiseerd door data op de work tape te kopieren, dan volgt de UTM een while loop:

VERY SIMPLIFIED ALGORITHM:	
	//Prepare UTM for while loop
	Copy <current state>";" to work tape
	Copy <current symbol> to work tape
	//Work tape now contains <current state>";"<current symbol>, we will compare this to Encoded TM tape to find a transition
	//<current symbol> is the encoded version of the symbol the simulated TM would be pointing to
	
	//Remember transitions are of form <current state>";"<current symbol>";"<new state>";"<new symbol>";"<direction>"0"
	//Now we can compare Encoded TM tape to the work tape and see if the <current state>";"<current symbol> parts match
	While (Encoded TM tape has not reached end):						//Loop over all transitions to try and find a match
		Compare Encoded TM tape to work tape
		If (work tape matches current transition in Encoded TM tape): 	//if <current state>";"<current symbol> are the same, we have a match on the transition
			Copy <new state> over in Current State tape					//execute the transition
			Copy <new state> over in work tape
			Copy <new symbol> over in Simulated State tape
			Copy <new symbol> over in work tape							//work tape now contains the new <current state>";"<current symbol> needed for the next transition
			Move the tape head in Simulated State tape in the direction of <direction>
			//Transition is done, -> restart loop with the new variables in work tape
		
		Else (not a match): 											//Looped over all transitions and no match found -> end simulation
			Move tape head on Encoded TM tape to next transition
	
	//If no transition was found for the TM, it means that the TM has halted. Whether or not it halted in an accepting state is of no importance to the UTM, the UTM just simulates the TM.

Dit algoritme bevat niet precies alle bewegingen die de UTM zal doen, maar is een kleine samenvatting van de logica er achter.
Het algoritme zal worden omgezet in een transitie tabel die precies elke stap zal voorstellen (zie UTM Transitions.ods file).





More in depth explanation of each state and what it does:		//To see exactly how everything works, look at the transitions table along with the explanation
	//Remember tape 1 = simulation, tape 2 = encoding of TM, tape 3 = current state of TM, tape 4 = work tape, used for comparisons
	//Each state will go to the one under it after its job is done, unless noted differently
	
	START STATE = Copy3TO4
	Copy3TO4: Copies <current state> and ";" onto the work tape
	ReturnHead3.a: Return the head on tape 3 back to where it was after copying to tape 4, later a new state will be written over it
	Copy1TO4.a: Copies <current symbol> onto work tape
	ReturnHead1.a: Return tape head 1 to where it was
	ReturnHead4.a: Return tape head 4 to where it was
	SetTape2: Tape head to will start at <blank symbol>, move it to the right so it will be at the first transition
	FindTransition.a: Compares the <current state> parts of tape 2 and 4. If they match, go to FindTransition.b. If they are different, go to (ReturnHead4.b)
	FindTransition.b: Compares the <current symbol> parts of tape 2 and 4. If they match, go to ReturnHead4.d. If they are different, go to (ReturnHead4.b)
	(ReturnHead4.b): Return tape head 4 to where it was, so we can compare it to the next transition
	(GoNextTrans): Set tape 2 to be at the next transition and go loop back to FindTransition.a. If tape 2 reaches a B, this means it checked all transitions and none of them matched -> go to NoTransFound
	ReturnHead4.d: A match has been found, return tape head 4 to where it was so we can get ready to execute the transition
	Copy2TO3AND4: Tape 2 will now be at the <new state> part of a transition, copy this over in tape 3 and 4
	ReturnHead3.b: Again, return the tape head to where it was
	Copy2TO1: Tape 2 is now at the <new symbol> part of a transition, copy this over in tape 1, this is the symbol that TM would write on its tape.
	ReturnHead1.b: After writing the new symbol, return the tape head to where it was.
	CountDirection.a: Tape 2 is now at the <direction> part. There are 3 possibilities for <direction>: 1, 11 or 111. There must be at least one 1, CountDirection.a will just go to CountDirection.b
	CountDirection.b: If a 0 is read now, this means there was only one 1 -> go to ReturnHead2.a. If a 1 is read, go to CountDirection.c
	CountDirection.c: If a 0 is read now, there were two 1's -> go to ReturnHead2.b If a 1 is read, there were three 1's -> go to ReturnHead2.c
	ReturnHead2.a: The work with the transition is almost over, return tape head 2 back to the beginning and go to MoveHead1Right
	ReturnHead2.b: The work with the transition is almost over, return tape head 2 back to the beginning and go to MoveHead1Left.a
	ReturnHead2.c: The work with the transition is almost over, return tape head 2 back to the beginning and go to Copy1TO4.b
	MoveHead1Left.a: The <direction> was left, this means tape head 1 has to move to the encoded symbol left of the one it's currently at. 
					 If tape head 1 does not read a B -> go to MoveHead1Left.b.
					 If tape head 1 reads a B, it means that TM would be at a blank symbol, so we have to take the encoded blank symbol for TM and put it on tape 1. -> go to WriteBlankLeft.
	MoveHead1Left.b: Move tape head 1 to the left until a 0 or B is found, tape 1 moved over a whole encoded symbol -> go to Copy1TO4.b
	MoveHead1Right: Move tape head 1 to the left until a 0 or B is found. 
					If tape head 1 reads a 0, it arrived at the next encoded symbol -> go to Copy1TO4.b
					If tape head 1 reads a B, it means that TM would be at a blank symbol, so we have to take the encoded blank symbol for TM and put it on tape 1. -> go to SetTape2Blank.
	Copy1TO4.b: Copy the new <current symbol> to the work tape
	ReturnHead1.d: Return tape head 1 to make it ready for next loop
	ReturnHead4.c: Return tape head 4 to make it ready for next loop -> go to FindTransition.a		//THE LOOP IS HERE
	
	WriteBlankLeft: Copy <blank symbol> from tape 2 to tape 1
	ReturnHead2.d: Return tape head 2 back to where it was -> go to Copy1TO4.b
	SetTape2Blank: Set tape 2 to the left of the <blank symbol> for copying it to tape 1
	WriteBlankRight: Copy the <blank symbol> to tape 1
	ReturnHead1.c: return tape head 1 to where it was -> go to Copy1TO4.b

	NoTransFound: The state UTM will be in after no matching transition could be found, the end of the simulation

