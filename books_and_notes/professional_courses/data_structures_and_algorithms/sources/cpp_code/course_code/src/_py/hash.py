#!/usr/bin/env python
# -*- coding: gbk -*-
#
# Example of Python dictionary (hashtable)
#
beauty = dict({	# declare and initialize a dictionary object
	"沉鱼":"西施",
	"落雁":"昭君",
	"闭月":"貂蝉",
	"羞花":"玉环"
})
print u'\n'.join(u' * ( %s : %s )' % (alias.decode('gbk').strip(), name.decode('gbk').strip()) for (alias, name) in beauty.items())

beauty["红颜"] = "圆圆"
print u'\n'.join(u' | ( %s : %s )' % (alias.decode('gbk').strip(), name.decode('gbk').strip()) for (alias, name) in beauty.items())

for (alias, name) in beauty.items():
    print " = (", alias,  ":", name, ")"

for (alias, name) in sorted(beauty.items()):
    print " | (", alias,  ":", name, ")"

for alias in sorted(beauty.keys(), reverse = True):
    print " - (", alias,  ":", beauty[alias], ")"
