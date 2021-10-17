#!/usr/bin/python3

import requests
import os

url = 'http://10.10.10.248/documents/'


for j in range(2019,2022):
	for i in range(1,13):
		for k in range(1,31):
			date = f'{j}-{i:02}-{k:02}-upload.pdf'
			r = requests.get(url+date)
			if(r.status_code == 200):
				os.system(f'wget {url}{date} -O {date}')

