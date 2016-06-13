import Node

data = [[]]
f = open('pokemon.csv')
output = open('result', 'w')
first_line = f.readline()
second_line = f.readline()

for line in f:
	line = line.strip("\r\n")
	data.append(line.split(','))
data.remove([])

tree = {'masa': {'77': 'muchy w polewie dziegciowej', '115': 'duzy wysmazony stek z ryzem i warzywami', '88': 'bezglutenowe kotlety z kasza', '130': 'duzy bezglutenowy kotlet sojowy z ryzem', '65': 'bezglutenowe kotlety sojowe z kasza', '66': 'bezglutenowe udko kurczaka z kasza', '80': 'duza salatka z tunczyka', '85': 'duzy wysmazony stek z ryzem i warzywami', '25': 'maly falafel', '20': {'wegetarianin': {'tak': {'alergia': {'nie': {'typ': {'wodny': 'zupa cebulowa', 'normalny': 'tarta z pieczarkami'}}, 'tak': 'pizza gryczana bez drozdzy'}}, 'nie': {'alergia': {'nie': {'typ': {'psychiczny': 'sredni stek z ziemniakami i sur\xc3\xb3wka', 'truj\xc4\x85cy': 'srednia porcja pomidorowej z arszenikiem', 'normalny': 'salatka z tunczyka'}}}}}}, '22': 'maly kebab', '49': 'kotlety sojowe z kasza kuskus', '44': 'bezglutenowe udko kurczaka z kasza', '45': 'bezglutenowe kotlety sojowe z kasza', '40': {'wegetarianin': {'tak': 'sredni kotlet sojowy z kasza', 'nie': {'alergia': {'nie': {'typ': {'truj\xc4\x85cy': 'muchy w polewie dziegciowej', 'wodny': 'rosol z kury'}}}}}}, '5': {'wegetarianin': {'tak': 'budyn jaglany', 'nie': {'alergia': {'nie': {'typ': {'robak': 'mala porcja rozk\xc5\x82adajacego si\xc4\x99 miesa', 'elektryczny': 'motyle w zalewie serowej', 'normalny': 'minipizza z peperoni'}}}}}}, '60': 'sredni kotlet sojowy z kasza', '120': 'kebab XXL', '75': 'duzy kotlet sojowy z kasza', '125': 'duzy bezglutenowy wysmazony stek z ryzem i warzywami', '100': 'duzy wysmazony stek z ryzem i warzywami', '95': 'bezglutenowe udko kurczaka z kasza', '105': 'bezglutenowe udko kurczaka z kasza', '11': 'budyn jaglany', '10': {'wegetarianin': {'tak': {'alergia': {'nie': {'typ': {'ro\xc5\x9blinny': 'kotlety sojowe z kasza kuskus', 'wodny': 'zupa cebulowa z grzankami'}}}}}}, '15': 'mala tarta z pieczarkami', '48': 'salatka z tunczyka', '55': 'udko cielece ze szpinakiem', '30': {'wegetarianin': {'tak': 'pasztet z ciecierzycy i dyni', 'nie': 'bezglutenowe udko kurczaka z kasza'}}, '51': 'muchy w polewie dziegciowej', '50': {'wegetarianin': {'nie': 'jagniecina', 'tak': {'alergia': {'nie': {'typ': {'wodny': 'srednia tarta z pieczarkami', 'roslinny': 'sredni falafel', 'normalny': 'tarta z pieczarkami'}}}}}}, '35': 'maly stek z ziemniakami', '99': 'kebab XXL', '33': 'maly falafel'}}
attributes = ['masa', 'wegetarianin', 'alergia', 'typ', 'class']
count = 0
for entry in data:
	count += 1
	tempDict = tree.copy()
	result = ""
	while(isinstance(tempDict, dict)):
		root = Node.Node(tempDict.keys()[0], tempDict[tempDict.keys()[0]])
		tempDict = tempDict[tempDict.keys()[0]]
		index = attributes.index(root.value)
		value = entry[index]
		if(value in tempDict.keys()):
			child = Node.Node(value, tempDict[value])
			result = tempDict[value]
			tempDict = tempDict[value]
		else:
			print "can't process input %s" % count
			result = "?"
			break
	output.write("%s" % result)
