#
# Example of Perl hash
#
my	%hero = (  # identifier preceded by % holds a group of values, keyed by string
		"ÔÆ³¤"=>"¹ØÓð",
		"ÒíµÂ"=>"ÕÅ·É",
		"×ÓÁú"=>"ÕÔÔÆ",
		"ÃÏÆð"=>"Âí³¬"
	);
foreach $style (keys %hero)
	{ print "  ($style,$hero{$style})"; }	print "\n";
$hero{"ººÉý"} = "»ÆÖÒ";
foreach $style (keys %hero)
	{ print "  ($style,$hero{$style})"; }	print "\n";
foreach $style (sort keys %hero)
	{ print "  ($style,$hero{$style})"; }	print "\n";
foreach $style (reverse sort keys %hero)
	{ print "  ($style,$hero{$style})"; }	print "\n";