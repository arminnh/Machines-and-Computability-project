from xml.etree.ElementTree import Element, SubElement, Comment, tostring
from xml.etree import ElementTree
from xml.dom import minidom

#Found on internet
def prettify(elem):
    """Return a pretty-printed XML string for the Element. """
    rough_string = ElementTree.tostring(elem, 'utf-8')
    reparsed = minidom.parseString(rough_string)
    return reparsed.toprettyxml(indent="  ")

#Read input file 
f = open('transitionsUTM.txt', 'r')
text = f.read()
f.close()

#Remove all whitespace, commas and quotes
text = text.split("“")
text = " ".join(text)
text = text.split("”")
text = " ".join(text)
text = text.split("\"")
text = " ".join(text)
text = text.split("\'")
text = " ".join(text)
text = text.split(",")
text = " ".join(text)
text = text.split()

#We get a list with States for the UTM followed by its transitions
#transitions are of form: current state, (input) 4 entries, next state, (write symbol, head direction) 4 entries 
#-> a transition take 14 spots in the list

#Setup the xml file
top = Element("turingmachine")
top.set("nrtapes", "4")
top.set("name", text[0])

blanksymbol = SubElement(top, "blanksymbol")
blanksymbol.text = "B"

anysymbol = SubElement(top, "anysymbol")
anysymbol.text = "a"

samesymbol = SubElement(top, "samesymbol")
samesymbol.text = "r"

i = 2
stateslist = []

states = SubElement(top, "states")
#Set the first state = initial state
state = SubElement(states, "st")
state.set("name", text[1])
state.set("initial", "true")
stateslist.append(text[1])
    
#The rest of the states
while (text[i] not in stateslist):
    state = SubElement(states, "st")
    state.set("name", text[i])
    #Set the last state = halting state
    if (text[i+1] in stateslist):
        state.set("halting", "true")
    stateslist.append(text[i])
    i += 1


#Do the transitions
transitions = SubElement(top, "transitions")
    
while i < len(text):
    #Make a transition element
    tr = SubElement(transitions, 'tr')
    
    #First comes current state
    current = SubElement(tr, 'current')
    current.set("state", text[i])
    i += 1
    
    #Then come the 4 inputs
    tape1 = SubElement(current, 'tape')
    tape2 = SubElement(current, 'tape')
    tape3 = SubElement(current, 'tape')
    tape4 = SubElement(current, 'tape')
    tape1.set("symbol", text[i])
    tape2.set("symbol", text[i+1])
    tape3.set("symbol", text[i+2])
    tape4.set("symbol", text[i+3])
    i += 4
    
    #Next state
    next = SubElement(tr, 'next')
    next.set("state", text[i])
    i += 1
    
    #Then the instructions for the 4 tapes
    tape1 = SubElement(next, 'tape')
    tape2 = SubElement(next, 'tape')
    tape3 = SubElement(next, 'tape')
    tape4 = SubElement(next, 'tape')
    tape1.set("symbol", text[i])
    tape1.set("direction", text[i+1])
    tape2.set("symbol", text[i+2])
    tape2.set("direction", text[i+3])
    tape3.set("symbol", text[i+4])
    tape3.set("direction", text[i+5])
    tape4.set("symbol", text[i+6])
    tape4.set("direction", text[i+7])
    i += 8

#Write output file
f = open('utm.xml', 'w')
f.write(prettify(top))
f.close()

