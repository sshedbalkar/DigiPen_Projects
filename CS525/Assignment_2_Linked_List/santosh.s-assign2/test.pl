#!/usr/bin/perl -w

BEGIN { push(@INC, '/home/dvolper/bin2'); };
require("timer.pl");

sub test () {
  my ($exe,$test)=@_;
#print "test($exe,$test)\n";
#10+24+37+15=86
#1+1+1+1+2+2+2+3+3+3+3+3+3+3+3=34
#        10|30|               ---------- 26 -----------------|----------30------------7-------|20|15
#                                                             EXTRA ----------------------------  LEAK
  if ($test==-1)    { return (0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,125); } # which tests to run
  elsif ($test==-9) { return (0,0,0,0,0,0,0,0,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0); } #executable
  elsif ($test==-6) { return (2,2,2,3,3,3,3,3,3,3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15); } #weights
  elsif ($test==-8) { return (0,0,0,0,0,0,0,0,0,0, 0, 0, 0, 0, 3, 3, 3, 3, 4, 4, 5, 5, 3, 2,20,  0); } #extra weights
  elsif ($test==-4) { return ("spvector.c"); } #quality
  elsif ($test==-5) { return ("spvector.c","spvector.h"); } #required
  elsif ($test==-3) { return ("gcc0","bcc0","msc0","bcc_cg"); } #from Makefile 
  elsif ($test==-7) { return (   8,     8,    8,      0); } #compiler weights
  elsif ($test==-2) { # files
    return ("Makefile","driver.c","bcc_cg.cgi",
	"out0", "out1", "out2", "out3", "out4", "out5", "out6", "out7", 
	"out8", "out9", "out10", "out11", "out12", "out13", "out14", 
	"out15", "out16", "out17", "out18", "out19", "out20", "out21", 
	"out22", "out23", "out24"
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
