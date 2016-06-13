import math

def find(item, list):
    for i in list:
        if item(i): 
            return True
        else:
            return False

#znajdowanie najpopularniejszej wartosci atrybutu
def majority(attributes, data, target):
    valFreq = {}
    index = attributes.index(target)
	#oblicz czestosc wartosci 
    for tuple in data:
        if (valFreq.has_key(tuple[index])):
            valFreq[tuple[index]] += 1 
        else:
            valFreq[tuple[index]] = 1
    max = 0
    major = ""
    for key in valFreq.keys():
        if valFreq[key]>max:
            max = valFreq[key]
            major = key
    return major

#obliczanie entropii
def entropy(attributes, data, targetAttr):

    valFreq = {}
    dataEntropy = 0.0
    
    i = 0
    for entry in attributes:
        if (targetAttr == entry):
            break
        ++i
    
	#oblicza czestosc kazdej z wartosci danego atrubutu
    for entry in data:
        if (valFreq.has_key(entry[i])):
            valFreq[entry[i]] += 1.0
        else:
            valFreq[entry[i]]  = 1.0

	#obliczanie entropii zgodnie ze wzorem
    for freq in valFreq.values():
        dataEntropy += (-freq/len(data)) * math.log(freq/len(data), 2) 
        
    return dataEntropy

def gain(attributes, data, attr, targetAttr):
    
    valFreq = {}
    subsetEntropy = 0.0
    
    i = attributes.index(attr)
	
    for entry in data:
        if (valFreq.has_key(entry[i])):
            valFreq[entry[i]] += 1.0
        else:
            valFreq[entry[i]]  = 1.0

    for val in valFreq.keys():
        valProb        = valFreq[val] / sum(valFreq.values())
        dataSubset     = [entry for entry in data if entry[i] == val]
        subsetEntropy += valProb * entropy(attributes, dataSubset, targetAttr)

    return (entropy(attributes, data, targetAttr) - subsetEntropy)

#wybiera najlepszy atrybut 
def chooseAttr(data, attributes, target):
    best = attributes[0]
    maxGain = 0;
    for attr in attributes:
        newGain = gain(attributes, data, attr, target) 
        if newGain>maxGain:
            maxGain = newGain
            best = attr
    return best

#pobiera wartosci "kolumny" danego atrybutu 
def getValues(data, attributes, attr):
    index = attributes.index(attr)
    values = []
    for entry in data:
        if entry[index] not in values:
            values.append(entry[index])
    return values

def getExamples(data, attributes, best, val):
    examples = [[]]
    index = attributes.index(best)
    for entry in data:
		#znajdowanie wejscia o odpowiedniej wartosci
        if (entry[index] == val):
            newEntry = []
            #dodaj wartosc jezeli nie jest ona w kolumnie najlepszych
            for i in range(0,len(entry)):
                if(i != index):
                    newEntry.append(entry[i])
            examples.append(newEntry)
    examples.remove([])
    return examples

def makeTree(data, attributes, target, recursion):
    recursion += 1
    data = data[:]
    vals = [record[attributes.index(target)] for record in data]
    default = majority(attributes, data, target)
	
	#Sprawdzam, czy sa jakiekolwiek dane oraz czy lista atrybutow nie jest pusta.
    #Jezeli zajdzie ktorys z warunkow zwrocona zostanie domyslna wartosc.
    if not data or (len(attributes) - 1) <= 0: return default
    elif vals.count(vals[0]) == len(vals): return vals[0]
    else:
        #Wybieramy najlepszy atrybut
        best = chooseAttr(data, attributes, target)
        #Tworzymy drzewo a danym atrybucie, wartosci dopisujemy pozniej.
        tree = {best:{}}
    
		#Tutaj tworzymy poddrzea dla kazdej wartosci odpowiadajacej najlepszemu atrybutowi
        for val in getValues(data, attributes, best):
            examples = getExamples(data, attributes, best, val)
            newAttr = attributes[:]
            newAttr.remove(best)
            subtree = makeTree(examples, newAttr, target, recursion)

			#dodajemy nowe poddrzewo do pustego slownika obiektow w naszym drzewie/wierzcholku, ktory utworzylismy
            tree[best][val] = subtree
    
    return tree

