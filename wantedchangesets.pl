use LWP::Simple;
use Try::Tiny;

binmode STDOUT, ':utf8';
$content = get("http://planet.openstreetmap.org/?C=M;O=A");


while ($content =~ m/<a href=\"(changesets.+)\">/g)
{
    my $n=$1;
    print "looking at $n\n";
    getstore("http://planet.openstreetmap.org/$n", $n);
    
    
#    print "Done!\n";   
}



