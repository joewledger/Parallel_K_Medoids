letters = ['a','b','c','d','e','f','g','h','i']
files = ["Vehicle_Data/xa%s.dat" % l for l in letters]
observations = []
for f in files:
	reader = open(f,"rb")
	for line in reader.readlines():
		l = line.strip().split(" ")
		features = [str(float(int(x))) for x in l[:-1]]
		features.append(l[-1])
		observations.append(features)
	reader.close()

writer = open("cars.data","wb")
for observation in observations:
	writer.write(",".join(x for x in observation) + "\n")
writer.close()

print(len(observations[-1]))
