#
# Example of Ruby hash
#
scarborough = {  # declare and initialize a hash table
	"P"=>"parsley",
	"S"=>"sage",
	"R"=>"rosemary",
	"T"=>"thyme"
}
puts scarborough  # output the hash table
puts "\n"
for k in scarborough.keys  # output hash table items
	puts k + "=>" + scarborough[k]  # 1-by-1
end
puts "\n"
for k in scarborough.keys.sort  # output sorted hash table items
	puts k + "=>" + scarborough[k]  # 1-by-1
end
