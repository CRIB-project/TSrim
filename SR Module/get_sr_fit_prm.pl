#!/usr/bin/perl

use strict;
use warnings;

######################################################################
# Execute this perl script in the directory where SRModule.exe exists.
# Target material data file should be prepared in SR.IN format,
# and place it in "material_name/" directory in advance.
# A mass table file is needed in "Z A Element mass" format.
######################################################################
# Initial ion for loop
my $abeg = 1; 
my $elbeg = 'H';
# my $abeg = 70; 
# my $elbeg = 'Mn';
# my $abeg = 56; 
# my $elbeg = 'Ni';
# my $abeg = 219;
# my $elbeg = 'U';

# Final ion for loop
#my $aend = 295;
#my $elend = 'Og';
my $aend = 243;
my $elend = 'U';
# my $aend = 2;
# my $elend = 'H';
# my $aend = 56; 
# my $elend = 'Ni';

# Target material list
#my @mat = ('h','butane','al','si');
#my @mat = ('he','butane','al','si','havar','mylar','ch2');
#my @mat = ('he');
#my @mat = ('air');
#my @mat = ('c','cd2','kapton','au');
#my @mat = ('au','he_90_co2_10');
#my @mat = ('ar','mo','cu','3he');
my @mat = ('ti','be','ni','anthracene');

# Directory and mass table file name
my $dir = 'range';
my $fmass = '/usr/local/share/mass2020.dat'; # mass table
my $label = ''; # if you need to label the output fit parameter file
#my $npol = 10; # Degree of fittin polynomial
my $npol = 16; # Degree of fittin polynomial

# Run SRIM calculation SR.exe (1) or skip (0)
my $sr = 1;
#my $sr = 0;
# Run range fit macro fit_srim_table.C (1) or skip (0)
my $fit = 1;

# $ROOTSYS directory
my $rootsys = $ENV{ROOTSYS};
######################################################################

# Get Z, A, Element & Mass from mass table
my @z = `awk '{print \$1}' $fmass`;
my @a = `awk '{print \$2}' $fmass`;
my @el = `awk '{print \$3}' $fmass`;
my @m = `awk '{print \$4}' $fmass`;
my $ibeg = 0;
my $iend = 0;
for (my $i = 0; $i < 4000; $i++){ # get the range of calbularion 
    $z[$i] = $z[$i] + 0;
    $a[$i] = $a[$i] + 0;
    chomp($el[$i]);
    $m[$i] = $m[$i] + 0.0;
    #print "$i $z[$i] $a[$i] $el[$i] $m[$i]\n";
    if($a[$i]==$abeg && $el[$i] eq "$elbeg"){
	$ibeg = $i;
    }
    if($a[$i]==$aend && $el[$i] eq "$elend"){
	$iend = $i;	
	last;
    }
}
# print "\n$ibeg $iend \n";

my $nmat = @mat; # length of array
for (my $j = 0; $j < $nmat; $j++){ # loop for material
    # file definitions
    my $mat_dir = "$dir/$mat[$j]";
    my $mat_dat = "$mat_dir/$mat[$j].IN";
    if(!-d $mat_dir){ # Check if directory exists
	print "Material directory does not exist. \n";
	exit;
    }
    
    # Get the material information in SR.IN format
    open my $fh, '<', $mat_dat or die "Error: $!";
    my @lmat = <$fh>;
    close $fh;
    chomp(@lmat);
	
    for (my $i = $ibeg; $i < $iend+1; $i++){ # loop for isotope	    
	if ($z[$i] < 93) { # to skip elements above U
	    # file definitions
	    my $sr_out = "$mat_dir/$a[$i]$el[$i]_in_$mat[$j].txt"; # SR.exe output file
	    my $fit_out = "$mat_dir/range_fit_pol${npol}_$mat[$j]$label.txt"; # Fit output file
	    #my $fit_out = "$mat_dir/tmp.txt"; # Fit output file

	    if($sr == 1){
		# Rewrite SR.IN for each incident ion and material
		my $Emax = $a[$i]*400000;
		my $l_srin = << "EOF";
---Stopping/Range Input Data (Number-format: Period = Decimal Point)
---Output File Name
"$sr_out"
---Ion(Z), Ion Mass(u)
$z[$i]   $m[$i]
@lmat
---Output Stopping Units (1-8)
 5
---Ion Energy : E-Min(keV), E-Max(keV)
 1    $Emax
EOF
		open(SRIN, ">", "SR.IN") or die("Cannot open SR.IN: $!");
		print SRIN "$l_srin";
		close(SRIN);
		system("nkf -Lw --overwrite SR.IN"); # To covert LF to CRLF!
	    
		# execute SRModule.exe (you need to be in SR\ Module/ directory)
		system("wine SRModule.exe");
	    }
		
	    # Extract only the data part from the range file
	    if($fit == 1){
		my $sr_out_cut = "tmp.txt"; 
		open(my $in_fh, '<', $sr_out) or die "Cannot open input file: $!"; # file before
		open(my $out_fh, '>', $sr_out_cut) or die "Cannot open output file: $!"; #file after
		while (my $line = <$in_fh>) { # Skip lines until
		    last if $line =~ /-----------  ---------- ---------- ----------  ----------  ----------/;
		}
		while (my $line = <$in_fh>) { # Stop lines at 
		    last if $line =~ /-----------------------------------------------------------/;
		    print $out_fh $line;
		}
		close($in_fh);
		close($out_fh);    
		
		# Fit range and get parameters to be overwritten in $fit_par_file 
		system("$rootsys/bin/root -b \'fit_srim_table.C($z[$i],$a[$i],\"$mat[$j]\",\"$sr_out_cut\",$npol,\"$fit_out\",\"w\")\'");
		system("rm $sr_out_cut");
	    }
	}
    } # for i
} # for j
