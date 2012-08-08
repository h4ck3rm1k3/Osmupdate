#! /usr/bin/perl -w
eval 'exec /usr/bin/perl -S $0 ${1+"$@"}'
    if 0; #$running_under_some_shell

use warnings;
use strict;
use File::Find ();

use File::Copy;
# Set the variable $File::Find::dont_use_nlink if you're using AFS,
# since AFS cheats.

# for the convenience of &wanted calls, including -eval statements:
use vars qw/*name *dir *prune/;
*name   = *File::Find::name;
*dir    = *File::Find::dir;
*prune  = *File::Find::prune;

sub wanted;
use Cwd qw(cwd);
my $base= cwd;
mkdir "osmupdate_temp/temp.m" or die "cannot mkdir $!" unless -d "osmupdate_temp/temp.m";

# Traverse desired filesystems
File::Find::find({wanted => \&wanted}, 'osmupdate_temp/');
exit;

sub wanted {
#    warn $name;
    #osmupdate_temp/temp.m100866575.txt
    #osmupdate_temp/temp.m100866575.osc.gz 
    my $fname = $name;
   
    if (/^temp\.m.*\z/)
    {
	if ($fname =~/osmupdate_temp\/temp.m(\d{3})(\d{3})(\d{3}).(osc.gz|txt)/)
	{
	    my $seq1 = $1;
	    my $seq2 = $2;
	    my $seq3 = $3;
	    my $ext=$4;

	    my $dir1=  $base ."/osmupdate_temp/temp.m/$seq1";
	    if (! -d $dir1){
		warn "going to make $dir1";
		mkdir $dir1 or die "cannot make $dir1 with error $! $@";
	    }

	    my $dir2= $base . "/osmupdate_temp/temp.m/$seq1/$seq2";
	    if (! -d $dir2) {
		warn  "going to make $dir2";
		mkdir $dir2 or die "cannot make $dir2 with error $! $@";
	    }
	    
#/886/623.osc.gz
	    my $target= $base . "/" .  "osmupdate_temp/temp.m/${seq1}/${seq2}/${seq3}.${ext}";
	    my $source = $base . "/" . $name;
	    move ($source,$target) or die "cannot mv $source to $target with error $! $@";
	    warn "moved $source to $target\n";
	}
    }
}
