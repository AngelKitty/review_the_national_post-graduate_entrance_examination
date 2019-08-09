#!/usr/bin/perl
$p=$ARGV[0];
$i=$ARGV[1];
while(1){
    print "offset: $i.\n";
    system("$p $i ");
    $i+=1; 
   } 

