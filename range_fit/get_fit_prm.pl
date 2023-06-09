#!/usr/bin/perl

use strict;
use warnings;

# Usage:
# 1. Edit below
# 2. $ perl get_fit_prm.pl
# 3. It'll append fitting parameters to the range_fit file 
#########################################
my $z = 1;
my $a = 1;
my $mat = 'mylar';
my $input_dir = '../../SRIM Outputs';
my $input_file = "$input_dir/Hydrogen 1 in Mylar.txt";
my $output_file = 'range_fit_h.txt';
##########################################

# No need to change below
my $input2_file = 'input_tmp.txt'; 

# 入力ファイルを開く
open(my $in_fh, '<', $input_file) or die "Cannot open input file: $!";

# 出力ファイルを開く
open(my $out_fh, '>', $input2_file) or die "Cannot open output file: $!";

# "aaa" という単語のある行までのデータを読み飛ばす
while (my $line = <$in_fh>) {
    last if $line =~ /  --------------  ---------- ---------- ----------  ----------  ----------/;
}

# "aaa" という単語のある行以降のデータを読み飛ばし、"bbb" という単語のある行までのデータを出力ファイルに書き込む
while (my $line = <$in_fh>) {
    last if $line =~ /-----------------------------------------------------------/;
    print $out_fh $line;
}

# ファイルを閉じる
close($in_fh);
close($out_fh);

print "$input_file -> $input2_file \n";
print "Extracted data part only. \n";

system("root -l \'fit_srim_table.C($z,$a,\"$mat\",\"$input2_file\",\"$output_file\")\'");
system("rm $input2_file");
print "Removed $input2_file. \n";
