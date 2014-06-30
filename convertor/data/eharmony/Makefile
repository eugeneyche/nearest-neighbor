default: directories in/EH-slice1-data.csv in/EH-slice1-labels.csv in/EH-slice2-data.csv in/EH-slice2-labels.csv
directories:
	if [ ! -d in ]; then mkdir in; fi
	if [ ! -d out ]; then mkdir out; fi

clean:
	rm -f in/*
	rm -f out/*

new: clean default

BASE_URL := "http://bmcfee.github.io/data"

in/EH-slice1-data.csv:
	cd in; wget $(BASE_URL)/EH-slice1-data.csv.gz && gunzip EH-slice1-data.csv.gz 

in/EH-slice1-labels.csv:
	cd in; wget $(BASE_URL)/EH-slice1-labels.csv.gz && gunzip EH-slice1-labels.csv.gz 

in/EH-slice2-data.csv:
	cd in; wget $(BASE_URL)/EH-slice2-data.csv.gz && gunzip EH-slice2-data.csv.gz 

in/EH-slice2-labels.csv:
	cd in; wget $(BASE_URL)/EH-slice2-labels.csv.gz && gunzip EH-slice2-labels.csv.gz 
