#!/usr/bin/python
import cgi
import time

import cgitb #these 2 lines activates a special exception handler that will 
cgitb.enable()#display detailed reports in the web browser if any errors occur (great for debugging)

#this is to let browser know what content type it is (maybe pass in via env)
print "Content-type:text/html\r\n\r\n" 
print "<html>"
print "<head><title>The MOOD of the DAY</title></head>"
print "<body><center>"
print "<h1> Hello User!! </h1><br />"
print '<h1> How do you feel today?</h1><br />'


form = cgi.FieldStorage()
if form.getvalue("name"):
	# time.sleep(5)
	name = form.getvalue("name")
	print '<h1> Hello ' + name + '! Thanks for your input!</h1><br />'
	if name == "Irem":	
		print '<a href="icikrikc.jpeg">'
		print '<img src="/img/icikrikc.jpeg" alt="My Sample Image" width="200">'
		print '</a>'	
	if name == "Xiaojing":	
		print '<a href="Xiaojing.jpeg">'
		print '<img src="/img/Xiaojing.jpeg" alt="My Sample Image" width="200">'
		print '</a>'

mood = form.getvalue("radio_mood")
if mood == "happy":
	print "<h2> Yay! I am happy too!</h2>"
	print '<br>'
	print '<br>'
	print '<a href="happy.jpeg">'
	print '<img src="/img/happy.jpeg" alt="My Sample Image" width="400">'
	print '</a>'	
if mood == "sad":
	print "<h2> Sorry to hear that.</h2>"
	print '<br>'
	print '<a href="bird.jpeg">'
	print '<img src="/img/bird.jpeg" alt="My Sample Image" width="400">'
	print '</a>'

print '<form method="post" action="mood.py">'
print '<p>Name: <input type="text" name="name" required/></p>'
print '<input type="radio" name="radio_mood" value="happy" required" /> Happy'
print '<input type="radio" name="radio_mood" value="sad" required/> Sad'
print '<input type="submit" value="Submit" />'
print "</form>"
print "</body><center>"
print "</html>"