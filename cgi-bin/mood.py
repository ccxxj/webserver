#!/usr/bin/python
import cgi

import cgitb #these 2 lines activates a special exception handler that will 
cgitb.enable()#display detailed reports in the web browser if any errors occur (great for debugging)

#this is to let browser know what content type it is (maybe pass in via env)
print "<html>"
print "<head><title>The MOOD of the DAY</title></head>"
print "<body><center>"
print "<h1> Hello User!! </h1><br />"
print '<h1> How do you feel today?</h1><br />'


form = cgi.FieldStorage()
if form.getvalue("name"):
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
if form.getvalue("happy"):
	print "<h2> Yay! I am happy too!</h2>"
	print '<br>'
	print '<br>'
	print '<a href="happy.jpeg">'
	print '<img src="/img/happy.jpeg" alt="My Sample Image" width="400">'
	print '</a>'	
if form.getvalue("sad"):
	print "<h2> Sorry to hear that, would you want to tell me why?</h2>"
	print '<div><label>Message</label><br/>'
	print '<textarea name="message"></textarea></div>'
	print '<br>'
	print '<br>'
	print '<a href="bird.jpeg">'
	print '<img src="/img/bird.jpeg" alt="My Sample Image" width="400">'
	print '</a>'

print '<form method="post" action="mood.py">'
print '<p>Name: <input type="text" name="name" /></p>'
print '<input type="checkbox" name="happy" /> Happy'
print '<input type="checkbox" name="sad" /> Sad'
print '<input type="submit" value="Submit" />'
print "</form>"
print "</body><center>"
print "</html>"