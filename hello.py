!/usr/bin/python

#this is to let browser know what content type it is (maybe pass in via env)
print "Content-type:text/html\r\n\r\n" 
print "<html>"
print "<head><title>My First CGI Program</title></head>"
print "<body>"
print "<p> It WOrks!! </p>"
for i in range(5):
	print "<h1> Hello Wolrd!"
print "</body>"
print "</html>"