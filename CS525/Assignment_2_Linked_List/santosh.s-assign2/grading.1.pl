#!/usr/bin/perl -w
my ($course, $loc, $login) = @ARGV;

die "Usage:\n\t$0 <course> <test> <login>\n" 
    unless ($course ne "" && $loc ne "");
print "grading $course $loc $login\n";

use File::Copy;
use POSIX;
require("./test.pl");

my $subm_max=10;
@tests=test("",-1);
@common_files=test("",-2);
foreach my $file (@common_files) { unlink("$loc/$login/$file"); }
sleep 1;
foreach my $file (@common_files) { copy("$file", "$loc/$login"); }
@targets=test("",-3);
@targetweights=test("",-7);
@check_quality_files=test("",-4);
@required_files=test("",-5);
@ignored_warnings=(
#    foreach $warn ("4005",     # borland - need parenthesis
#                   "ignoring", # 
#"C4244"     # MS - can not create assignment
		"C4512" # MS - assignment operator could not be generated
                  );
srand(`date +%N`);
chdir("$loc/$login");
$grade=0;

print("== SUBMISSION =============================\n");
#`touch *`;#set time
#get the first line only
my $subm_grade = `cat r.txt | head -n 1`; chop($subm_grade);
print "$subm_grade\n";
open(RTX,">r.txt"); #OVERWRITE
print RTX "$subm_grade\n";
if ($subm_grade==0) {
  chdir("../..");
  exit 1;
};
$grade=$subm_grade;

print("== COMPILE ================================\n");
# 5) compile, grep warning and errors, count them
`make clean`;
my $old_grade=$grade;
my $numtargets=$#targets+1;
my $targetsweighttotal=0;
for (my $i=0;$i<$numtargets;++$i) {
	my $target=$targets[$i];
	my $targetweight=$targetweights[$i];
	$targetsweighttotal += $targetweight>0?$targetweight:0; #count only positive targets
	my $file_res =  "out.make.$target.txt";
	print("\"make $target &> $file_res\"\t");
	if ($targetweight>0) { print RTX "$target "; } 
	`make $target 1>$file_res 2>&1`; 
	$num_warn  = 0;
	$num_error = 0;
#check warning from each file
	foreach $file (@required_files) {
		$num_warn_t  = `cat $file_res | grep -i 'warning' | grep -i '$file' | wc -l`; chop($num_warn_t);
		$num_error_t = `cat $file_res | grep -i 'error' | wc -l`; chop($num_error_t);
		$num_warn  += $num_warn_t; 
		$num_error += $num_error_t;
#check for warnings that we do not count
		foreach $warn (@ignored_warnings) {
			$num_warn_t  = `cat $file_res | grep -i $warn | grep -i '$file' | wc -l`; 
			chop($num_warn_t);
			$num_warn  -= $num_warn_t;
		}
	}

	if ($targetweight<0) {#negatively weighted targets
		if ($num_error==0) {
			print     "$target compiles $targetweight($targetweight)\n";
			print RTX "$target $targetweight\n";
		}
		else { 
			print     "$target does not compile 0($targetweight)\n";
			print RTX "$target 0\n";
		}
	} 
	else { #positively weighted targets
		if ($num_warn+$num_error>0) { print("$target warnings - $num_warn, errors   - $num_error, see $file_res.\t"); } 
		else { print ("$target OK.\t"); }

		if ($num_error==0) { 
			$d=&max($targetweight-$num_warn,$targetweight/2); 
			print("$d/$targetweight\n"); 
			if ($targetweight>0) { print RTX "$d\n"; }
			$grade+=$d; 
		} else { 
			print("0/$targetweight\n");
			if ($targetweight>0) { print RTX "0\n"; }
		}
	}
}

print("== RUNTIME ================================\n");
$old_grade=$grade;
my @pts_array=test("",-6);
my @extra_pts_array=test("",-8);
my @executables_array=test("",-9);
my $run_max=0;
#foreach my $testname (@tests) { $run_max+=$pts_array[$testname]; }
my %pts;
my %extra_pts;
my %executables;
for (my $i=0;$i<=$#tests; ++$i) {
  $run_max+=$pts_array[$i]; #only 
  #print "$i - $pts_array[$i]\n";
  $pts{$tests[$i]}=$pts_array[$i];
  $extra_pts{$tests[$i]}=$extra_pts_array[$i];
  $executables{$tests[$i]}=$executables_array[$i];
}

foreach my $testname (@tests) {
  my $d =0;
  my $num_exe_s = `ls *.exe | wc -l`; chop($num_exe_s);
  if ($num_exe_s >0 ) {
	  $exe = &random_executable("*$executables{$testname}.exe");
	  if ($testname>99) { $exe="bcc_cg.exe"; }

	  print "test $testname (using $exe): ";

	  if ($testname>99) {
		  my $CG_log_file_base = `basename $exe exe`; chop($CG_log_file_base);
		  $CG_log_file = $CG_log_file_base . "cgl";
		  unlink ( $CG_log_file );
		  if (-e $exe) {
			  test($exe,eval($testname-100));

			  if (-e $CG_log_file) {
				  my $CG_file = "$CG_log_file_base.test" . eval($testname-100) . ".cgl";
				  my $move_CG_log = "mv $CG_log_file $CG_file";
				  system($move_CG_log);
				  print "$CG_log_file was created (see $CG_file) - check memory\n";
				  $d=0;
			  } else {
				  print "$CG_log_file wasn't found\nOK ";
				  $d = $pts{$testname} + $extra_pts{$testname};
			  }
		  } else {
			  print "$exe doesn't exist - skipping the test\n";
			  $d=0;
		  }
	  }
	  else {
#rename($exe,"$login"."_$exe");
#my ($retcode,@result)=test("$login"."_$exe",$testname);
		  my ($retcode,@result)=test($exe,$testname);
#rename("$login"."_$exe",$exe);
		  my $diff1 = 0;
		  foreach $r (@result) { $diff1 += $r; }
#print "RC $retcode, diff1 $diff1  ";
#foreach my $f (@result) { print "result $f\n"; }

#  result   (diff in output, diff in file, diff in file mod white)
		  if ($diff1==0) {
			  print  ("OK ");
			  $d = $pts{$testname} + $extra_pts{$testname};
		  } else {
			  if ($result[0]>0) { 
				  $d = 0;
				  print "see master output for test $testname"; 
			  }
			  elsif ($#result>2 && $result[1]>0 && $result[2]==0) { #extra white line
				  $d = $pts{$testname} -1;
				  print "output file contains extra white space, see test.pl case $testname "; 
			  } else {
				  print "output files are different, see test.pl case $testname "; 
			  }
		  }
	  }
  } else { print("No executables were created for test $testname\n"); }
	print " ($d/$pts{$testname} points)\n";
  $grade += $d;
  print RTX "test$testname $d\n";
}

#print("== CODE QUALITY ========================\n");
##code efficiency and etc
##for loops,  
#foreach $file (@check_quality_files) {
#  foreach $keyword ("abcdefg") {
#    `cat $file | grep -in \"$keyword\" > temp`;
#    $count=`cat temp | wc -l`;
#    chop($count);
#    if ($count>0) {
#      print("\"$keyword\" is used in $file $count times:\n");
#      $uses=`cat temp`;
#      $uses =~ s/^(\d+):\s+/[$1]: /gm;
#      print $uses;
#    } 
#  }
#}

print("\n===========================================\n");
my $cq = 100 - $subm_max - $targetsweighttotal - $run_max;
print("GRADE before code quality check +[0..$cq] is $grade");
print("\n===========================================\n");

#clean
`make clean`;
`rm -rf html temp`;

#copy script
`cp ../../test.pl .`;
system("cp `which grading.1.pl` .");
system("cp `which timer.pl` .");

#zip
my $zipfile_graded="graded.zip";
`rm $zipfile_graded`;
my @allfiles=split(" ",`ls`);
`zip $zipfile_graded @allfiles`;
#`rm -rf @allfiles`;

print "Grading script usage \"$0 $course $loc $login\".\
your code (@required_files r.txt) should be in $loc/$login/ relative to\
(@common_files).\
NOTES:\n";
print test("",-10);

chdir("../..");
exit(0);
############
############


############
# subroutines
############
#choose a random executable to run
sub random_executable {
  my @list=split(' ', `ls @_`);
  return $list[rand ($#list+1)];
}

############
sub max { my($a,$b) = @_; return ($a>$b)?$a:$b; }

############
sub newest_file { return `ls -t | head -n 1`; }
