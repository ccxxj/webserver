https://www.infor.uva.es/~jvegas/cursos/web/cgi-bin/fuentes/readdata.html

if($ENV{'REQUEST_METHOD' eq "GET"})
	$in = $ENV{'QUERY_STRING'};
elsif ($ENV{'REQUEST_METHOD' eq "POST"})
	read(STDIN, $in, $ENV{'CONTENT_LENGTH'});
@in = split(/&/, $in);
foreach(@in){
	s/\+/ /g;
	($name, $value) = split(/=/, $_);
}