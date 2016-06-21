import DecisionTree

def main():
    file = open('targetTraining.csv')
    target = "next"
    data = [[]]
    for line in file:
        line = line.strip("\r\n")
        data.append(line.split(','))
    data.remove([])
    attributes = data[0]
    data.remove(attributes)
    tree = DecisionTree.makeTree(data, attributes, target, 0)
    print "generated decision tree for target"
    file = open('target_program.py', 'w')
    file.write("import Node\n\n")
    file.write("data = [[]]\n")
    file.write("f = open('garden.csv')\n")
    file.write("output = open('target_result', 'w')\n")
    file.write("for line in f:\n\tline = line.strip(\"\\r\\n\")\n\tdata.append(line.split(','))\n")
    file.write("data.remove([])\n")
    file.write("tree = %s\n" % str(tree))
    file.write("attributes = %s\n" % str(attributes))
    file.write("count = 0\n")
    file.write("for entry in data:\n")
    file.write("\tcount += 1\n")
    file.write("\ttempDict = tree.copy()\n")
    file.write("\tresult = \"\"\n")
    file.write("\twhile(isinstance(tempDict, dict)):\n")
    file.write("\t\troot = Node.Node(tempDict.keys()[0], tempDict[tempDict.keys()[0]])\n")
    file.write("\t\ttempDict = tempDict[tempDict.keys()[0]]\n")
    file.write("\t\tindex = attributes.index(root.value)\n")
    file.write("\t\tvalue = entry[index]\n")
    file.write("\t\tif(value in tempDict.keys()):\n")
    file.write("\t\t\tchild = Node.Node(value, tempDict[value])\n")
    file.write("\t\t\tresult = tempDict[value]\n")
    file.write("\t\t\ttempDict = tempDict[value]\n")
    file.write("\t\telse:\n")
    file.write("\t\t\tprint \"can't process input %s switching to default\" % count\n")
    file.write("\t\t\tresult = \"CZEKA_NA_KELNERA\"\n")
    file.write("\t\t\tbreak\n")
    file.write("\toutput.write(\"%s\" % (result))\n")
    print "written program"
    
    
if __name__ == '__main__':
    main()