#coding=utf-8
filename = "Big5-ZhuYin.map"

file = open(filename,'r', encoding='big5-hkscs')
content = file.read();
spl = content.split('\n');

LIST = [];
for i in spl:
	if len(i)!=0:

		m1 = i[0];
		i = i[2:];
		m = i.split('/');
		for i in m:
			LIST.append([m1,i[0]]);


dic = dict()
Zhu = u"ㄅㄆㄇㄈㄉㄊㄋㄌㄍㄎㄏㄐㄑㄒㄓㄔㄕㄖㄗㄘㄙㄧㄨㄩㄚㄛㄜㄝㄞㄟㄠㄡㄢㄣㄤㄥㄦ";
for i in Zhu:
	dic[i] = [];

strings = [];
for i in LIST:
	for j in Zhu:
		if i[1] == j:
			if i[0] not in dic[j]:
				dic[j].append(i[0]);
for i in Zhu:
	strings.append(i);
	strings.append(" ");
	for j in dic[i]:
		strings.append(j)
		strings.append(" ");
	strings.append("\n")
	for j in dic[i]:
		strings.append(j);
		strings.append(" ");
		strings.append(j);
		strings.append("\n");

filename = "Zhu-Yin_Big5.map"	
file = open(filename,'w', encoding='big5-hkscs')
strings = ''.join(strings);
file.write(strings)
file.close()
