#!/usr/bin/python
import cgi

#this is to let browser know what content type it is (maybe pass in via env)
print "Content-type:text/html\r\n\r\n" 
print "<html>"
print "<head><title>My First CGI Program</title></head>"
print "<body>"
print "<p> Hello Program!! </p>"
form = cgi.FieldStorage()
if form.getvalue("name"):
	name = form.getvalue("name")
	print '<h1> Hello ' + name + '! Thanks for my script!</h1><br />'
if form.getvalue("happy"):
	print "<p> Yay! I am happy too!</p>"
if form.getvalue("sad"):
	print "<p> Sorry to hear that, but why!</p>"

print '<form method="post" action="hello_complex.py">'
print '<p>Name: <input type="text" name="name" /></p>'
print '<input type="checkbox" name="happy" /> Happy'
print '<input type="checkbox" name="sad" /> Sad'
print '<input type="submit" value="Submit" />'
print "</form>"
print "</body>"
print "</html>"