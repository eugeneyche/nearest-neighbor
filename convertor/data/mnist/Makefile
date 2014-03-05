default: directories in/train-images-idx3-ubyte in/train-labels-idx1-ubyte in/t10k-images-idx3-ubyte in/t10k-labels-idx1-ubyte

directories:
	if [ ! -d in ]; then mkdir in; fi
	if [ ! -d out ]; then mkdir out; fi

clean:
	rm -f in/*
	rm -f out/*

new: clean default

BASE_URL := "http://yann.lecun.com/exdb/mnist/"

in/train-images-idx3-ubyte:
	cd in; wget $(BASE_URL)train-images-idx3-ubyte.gz && gunzip train-images-idx3-ubyte.gz 
in/train-labels-idx1-ubyte:
	cd in; wget $(BASE_URL)train-labels-idx1-ubyte.gz && gunzip train-labels-idx1-ubyte.gz 
in/t10k-images-idx3-ubyte:
	cd in; wget $(BASE_URL)t10k-images-idx3-ubyte.gz && gunzip t10k-images-idx3-ubyte.gz 
in/t10k-labels-idx1-ubyte:
	cd in; wget $(BASE_URL)t10k-labels-idx1-ubyte.gz && gunzip t10k-labels-idx1-ubyte.gz 
