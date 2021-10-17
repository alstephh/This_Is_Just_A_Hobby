#!/usr/bin/python3

from pdfminer.high_level import extract_text

files = ['2020-01-01-upload.pdf', '2020-01-02-upload.pdf', '2020-01-04-upload.pdf', '2020-01-10-upload.pdf', '2020-01-20-upload.pdf', '2020-01-22-upload.pdf', '2020-01-23-upload.pdf', '2020-01-25-upload.pdf', '2020-01-30-upload.pdf', '2020-02-11-upload.pdf', '2020-02-17-upload.pdf', '2020-02-23-upload.pdf', '2020-02-24-upload.pdf', '2020-02-28-upload.pdf', '2020-03-04-upload.pdf', '2020-03-05-upload.pdf', '2020-03-12-upload.pdf', '2020-03-13-upload.pdf', '2020-03-17-upload.pdf', '2020-03-21-upload.pdf', '2020-04-02-upload.pdf', '2020-04-04-upload.pdf', '2020-04-15-upload.pdf', '2020-04-23-upload.pdf', '2020-05-01-upload.pdf', '2020-05-03-upload.pdf', '2020-05-07-upload.pdf', '2020-05-11-upload.pdf', '2020-05-17-upload.pdf', '2020-05-20-upload.pdf', '2020-05-21-upload.pdf', '2020-05-24-upload.pdf', '2020-05-29-upload.pdf', '2020-06-02-upload.pdf', '2020-06-03-upload.pdf', '2020-06-04-upload.pdf', '2020-06-07-upload.pdf', '2020-06-08-upload.pdf', '2020-06-12-upload.pdf', '2020-06-14-upload.pdf', '2020-06-15-upload.pdf', '2020-06-21-upload.pdf', '2020-06-22-upload.pdf', '2020-06-25-upload.pdf', '2020-06-26-upload.pdf', '2020-06-28-upload.pdf', '2020-06-30-upload.pdf', '2020-07-02-upload.pdf', '2020-07-06-upload.pdf', '2020-07-08-upload.pdf', '2020-07-20-upload.pdf', '2020-07-24-upload.pdf', '2020-08-01-upload.pdf', '2020-08-03-upload.pdf', '2020-08-09-upload.pdf', '2020-08-19-upload.pdf', '2020-08-20-upload.pdf', '2020-09-02-upload.pdf', '2020-09-04-upload.pdf', '2020-09-05-upload.pdf', '2020-09-06-upload.pdf', '2020-09-11-upload.pdf', '2020-09-13-upload.pdf', '2020-09-16-upload.pdf', '2020-09-22-upload.pdf', '2020-09-27-upload.pdf', '2020-09-29-upload.pdf', '2020-09-30-upload.pdf', '2020-10-05-upload.pdf', '2020-10-19-upload.pdf', '2020-11-01-upload.pdf', '2020-11-03-upload.pdf', '2020-11-06-upload.pdf', '2020-11-10-upload.pdf', '2020-11-11-upload.pdf', '2020-11-13-upload.pdf', '2020-11-24-upload.pdf', '2020-11-30-upload.pdf']
keywords = ['username','name','password','pass']

for i in files:
	text = extract_text(i)

	for j in keywords:
		if (j in text):
			print(i)
