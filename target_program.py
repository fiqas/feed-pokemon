import Node

data = [[]]
f = open('garden.csv')
output = open('target_result', 'w')
for line in f:
	line = line.strip("\r\n")
	data.append(line.split(','))
data.remove([])
tree = {'last': {'CZEKA_NA_KELNERA': {'max': {'CZEKA_NA_KELNERA': {'waiter_counter': {'10': 'CZEKA_NA_KELNERA', '13': 'CZEKA_NA_KELNERA', '20': 'CZEKA_NA_KELNERA', '21': 'CZEKA_NA_KELNERA', '22': 'CZEKA_NA_KELNERA', '23': 'CZEKA_NA_KELNERA', '19': 'CZEKA_NA_KELNERA', '18': 'CZEKA_NA_KELNERA', '16': 'CZEKA_NA_KELNERA', '15': 'CZEKA_NA_KELNERA', '5': 'CZEKA_NA_RACHUNEK', '17': 'CZEKA_NA_KELNERA', '8': {'bill_counter': {'10': 'CZEKA_NA_RACHUNEK', '8': 'CZEKA_NA_KELNERA'}}, '14': 'CZEKA_NA_KELNERA'}}, 'CZEKA_NA_RACHUNEK': 'CZEKA_NA_RACHUNEK'}}}}
attributes = ['last', 'max', 'waiter_counter', 'bill_counter', 'next']
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
			print "can't process input %s switching to default" % count
			result = "CZEKA_NA_KELNERA"
			break
	output.write("%s" % (result))
