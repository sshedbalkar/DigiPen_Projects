#!/usr/bin/perl -w

exit &timer(@ARGV);


#################
#################
sub timer() {
#foreach $p (@_) { print "-- $p\n"; }
	my ($exe,$UID,$pars,$input,$output,$time)=@_;

#print "Starting exe=$exe pars=$pars in=$input out=$output\n" .
#"\tusing UID = $UID\n\ttimeout = $time\n"; 
	my $retval = fork();
	if ($retval == 0) {
# this is the child process
#print "\tworker process start (\"$exe $pars $UID $input $output\").. \n";
		system ("$exe $pars $UID $input $output"); 
#print "\tworker process done, stop guard\n";
		&kill_executable("sleep-guard","$UID");
#exit;   # this terminates the child process
	} else {
# this is the parent process
#print "\tguard process start (\"./sleep-guard $time $UID\")..\n";
		system("sleep-guard $time $UID");
#print "\tguard process sleep done, kill worker ...\n";
#print `ps x | grep $UID`;
		my $status = &kill_executable("$exe","$UID");
#print "\tguard process done\n";
		exit 1-$status;
	}
}

sub kill_executable() {
	my ($exe,$UID)=@_;
	my $status = 0; # 0 killed, 1 not found
	my $d=`ps x | grep $exe | awk '(/$UID/ && !/grep/ && (!/perl/)){print \$1; exit;}'`;
	chop($d); 
	while ($d ne "") { #while executable is running
		$status = 1;
#print "\tkill PID = $d\n";
#print "\t". `ps x | grep $d`;
		`kill $d`; 
#sleep 1;
		$d=`ps x | grep $exe | awk '(/$UID/ && !/grep/ && (!/perl/)){print \$1; exit;}'`;
	chop($d); 
	}
#print "\tkill_executable(($exe,$UID) returns $status\n";
	return $status;
}
