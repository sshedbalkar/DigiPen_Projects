#!/usr/bin/perl -w

#################
#my $exe = shift;
#my $test = shift;
#my $input = shift;
#my $output = shift;
#my $time = shift;
#my @result = &timer($exe,$test,$input,$output,$time);
#foreach my $r (@result) {
# print "$r\n";
#}
#exit;
#################
sub timer () {
  if ( `uname -a` =~ /Cygwin/ ) {
    return timer_win(@_);
  } else {
    return timer_unix(@_);
  }
}
#################
sub timer_win() {
  my ($exe,$pars,$input,$output,$time)=@_;

  use threads;
  use Thread::Semaphore;
  use threads::shared;
  my $status : shared = -1;
  $s  = Thread::Semaphore->new(1);  # thread finish
  $s2 = Thread::Semaphore->new(1);  # status
  $s->down(1);

#print "using Cygwin\n";

  $thr1 = async { #worker
		print "$exe $pars $input $output\n"; 
		`$exe $pars $input $output`; 
    $s2->down; #critical section for status
    if ($status!=1) { $status=0; } 
    $s2->up; 
#print "test$pars done\n";

    $s->up; #let main part continue
  };

  $thr2 = async { #guard
#print "sleep $time\n";
    sleep $time;
    #print "guard$pars is done\n";
    $s2->down; 
    if ($status!=0) { 
      print "KILL test $pars\n";
      $status=1; #even if worker finishes, it won't chnage status 
      $s2->up; 
      $s->up; #let main part continue
    }
  };


  $s->down; #wait the first thread to finish
  $s2->down; #lock status variable
  if ($status==1) {
    print "timeout on test$pars: \n"; 
    my $exename=$exe;
    if ($exename =~ /(.*)\/(.*)/) { 
      $exename = $2; 
    } #clean the name off forward slashes
    if ($exename =~ /^(.*)\.exe(.*)/) { 
      $exename = $1; 
    } #clean the name extension
    #print "exename = $exename\n";
    #$line=`ps | grep $exename`; chop($d); 
    #print "$line\n";
    $pid=`ps | grep $exename | awk '(/$exename/){ print \$1; exit; }'`; chop($pid);
    #print "$pid\n";
    if ($pid eq "") { 
      print "worker thread finished meanwhile, still not counted\n"; 
      `rm $output`; 
    }
    else {
      while ($pid ne "") {
	print "kill $pid\n";
	`kill $pid`; 
	$pid=`ps | grep $exename | awk '(/$exename/){ print \$1; exit; }'`; chop($pid);
	#print "=========\n$pid\n";
      }
      $thr1->detach;
    }
    `rm $output`; 
#print "==========\n";
  }
  elsif ($status==0) { 
#print "detach guard$pars\n"; $thr2->detach; 
#print "----------\n";
#print `cat $output`;
#print "==========\n";
  }
  elsif ($status==-1) { 
#print "detach worker$pars AND guard$pars\n"; $thr1->detach; $thr2->detach; 
	}
  return $status;
}
#################
sub timer_unix() {
  my ($exe,$pars,$input,$output,$time)=@_;

  use threads;
  use Thread::Semaphore;
  my $status : shared = -1;
  my $s : shared = Thread::Semaphore->new(1); #wait at least one thread to finish
  my $s2 : shared = Thread::Semaphore->new(1); #status semaphore

  $s->down;

  $thr1 = async { #worker
		print "$exe $pars $input $output\n"; 
		`$exe $pars $input $output`; 
    $s2->down; 
    if ($status!=1) { $status=0; } 
#$s2->up; 
    $s->up;
#print "worker$pars done\n";
  };

  $thr2 = async { #guard
    sleep $time;
#print "guard$pars done\n"; 
    $s2->down; 
    if ($status!=0) { $status=1; } 
#$s2->up; 
#print "KILL test$testid\n"; 
    $s->up;
  };


  $s->down; #wait at least one thread to finish
#print "main\n";
  if ($status==1) {
    my $exename=$exe;
    if ($exe =~ /\/(.*)/) { $exename = $1; } #clean the name off forward slashes
    print "TIMEOUT -- killing $exe  $pars $input $output\n";
#$d=`ps -aef | grep $exe`; chop($d); 
#print "=========\n$d\n";
    $d=`ps -aef | grep $exe | awk '(!/aef/ && (!/perl/)){    print \$2; exit; }'`; chop($d);
#print "ps -aef | grep $exe | awk '(!/aef/ && (!/perl/)){ print \$2; exit; }'\n";
#print "=========$d\n";
    while ($d ne "") {
#print "time exceeded - kill ";
#sleep(1);
      `kill $d`; 
#$d2=`ps -aef | grep $exe`; chop($d2); 
#print "=========\n$d2\n";
      $d=`ps -aef | grep $exe | awk '(!/aef/ && (!/perl/)){    print \$2; exit; }'`; chop($d);
#print "ps -aef | grep $exe | awk '(!/aef/ && (!/perl/)){ print \$2; exit; }'\n";
#print "=========\n$d\n";
    }
  }
  elsif ($status==0) { 
#print "detach guard$pars\n"; 
		$thr2->detach; 
	}
  elsif ($status==-1) { 
#print "detach worker$pars AND guard$pars\n"; 
		$thr1->detach; 
		$thr2->detach; 
	}
  return $status;
}
#################################
sub check_file_output () {
  my ($m,$s)=@_;
    if (!(-e $s)) { return (100,100); }
    my $d2 = `diff $s $m --strip-trailing-cr `; chop ($d2);
    my $d3 = `diff $s $m --strip-trailing-cr -B `; chop ($d3); #allow blank lines
    return (length($d2), length($d3));
}
#################################
sub diff () {
  my ($f1,$f2)=@_;
#  my $d1 = `diff $f1 $f2 --strip-trailing-cr -wiBy | grep "<" | wc -l`; chop ($d1); #line exists in f1, but not in f2
#  my $d2 = `diff $f1 $f2 --strip-trailing-cr -wiBy | grep ">" | wc -l`; chop ($d2); #line exists in f2, but not in f1
#  my $d3 = `diff $f1 $f2 --strip-trailing-cr -wiBy | grep "|" | wc -l`; chop ($d3); #lines differ
#  return $d1+$d2+$d3;
  if (!(-e $f1) || !(-e $f2)) { return (100); }

  my $d = `diff $f1 $f2 --strip-trailing-cr -w | wc -l`; 
	chop ($d); #lines differ
  return $d;
}
1
