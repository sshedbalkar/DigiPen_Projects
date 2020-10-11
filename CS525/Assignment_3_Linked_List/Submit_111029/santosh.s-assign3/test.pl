#!/usr/bin/perl -w

BEGIN { push(@INC, '/home/dvolper/bin2'); };
BEGIN { push(@INC, '.'); };
require("timer.pl");

#my @out = &test("gcc-args.exe",shift);
#print "@out\n";
#exit;

#################################
sub test () {
  my ($exe,$test)=@_;
  #                           <<<<<<<        RUNTIME           >>>>>>>>>  <<<< CodeGuard   >>>>>
  if ($test==-1)    { return (0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,118); } # which tests to run
  elsif ($test==-9) { return (0,0,0,0,0,0,0,0,0,0, 0, 0, 0, 0, 0, 0, 0, 0,  0); } #executable
  elsif ($test==-6) { return (2,2,2,2,2,2,2,2,2,2, 2, 2, 2, 2, 2, 2, 2, 2, 20); } #weights
  elsif ($test==-8) { return (0,0,0,0,0,0,0,0,0,0, 0, 0, 0, 0, 0, 0, 0, 0,  0); } #extra weights
  elsif ($test==-4) { return ("vector.cpp"); } #quality
  elsif ($test==-5) { return ("vector.cpp","vector.h"); } #required
  elsif ($test==-3) { return ("gcc0","bcc0","msc0","bcc_cg","additional","nocompile1","nocompile2" ); } #from Makefile 
  elsif ($test==-7) { return (     6,     6,     6,       0,           5,         -10,         -10 ); } #compiler weights
  elsif ($test==-10) { return 
"tagret nocompile1 checks if Insert is prohibited for const vectors \
tagret nocompile2 checks if SparseVector::operator[] DISallows writing to const vectors \
tagret additional checks if copy construction and assignment use \"const&\"\n";}
  elsif ($test==-2) { # files
    return ("Makefile","driver.2.cpp","driver.cpp","driver-nocompile1.cpp","driver-nocompile2.cpp",
	"out0", "out1", "out2", "out3", "out4",
	"out5", "out6", "out7", "out8", "out9",
	"out10", "out11", "out12", "out13", "out14","out15", "out16", "out17",
	"bcc_cg.cgi"
	);  
  }
  elsif ($test >99 ) { 
		system("rm -f bcc_cg.cgl");
		print("CodeGuard test  ");
		if (!(-e "bcc_cg.exe")) { return (1,1); }

	  my $stat=&timer("./bcc_cg.exe",eval($test-100),"",">/dev/null",3);
		if (-e "bcc_cg.cgl") { return (1,1); }
		else { return (0,0); }
  }
  elsif ($test >-1 ) { 
	  my $stat=&timer("./$exe","$test","",">student.out$test",3);
    return ($stat,&diff("student.out$test","out$test"));
  }
}
1
