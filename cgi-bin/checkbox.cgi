#!/usr/bin/python

# Import modules for CGI handling 
import cgi, cgitb 

# Create instance of FieldStorage 
form = cgi.FieldStorage()
print form.keys()

# Get data from fields
first_name = form.getvalue('first_name')
last_name = form.getvalue('last_name')

print "HTTP/1.1 200 OK"
print "Content-type:text/html\r\n\r\n"
print "<html>"
print "<head>"
print "<title>Checkbox - Third CGI Program</title>"
print "</head>"
print "<body>"
print "<h2> first name is : %s and last name is: %s</h2>" % (first_name, last_name)
print "</body>"
print "</html>"