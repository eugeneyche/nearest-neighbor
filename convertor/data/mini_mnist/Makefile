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
	make -C ../mnist
	cp ../mnist/in/train-images-idx3-ubyte in
in/train-labels-idx1-ubyte:
	make -C ../mnist
	cp ../mnist/in/train-labels-idx1-ubyte in
in/t10k-images-idx3-ubyte:
	make -C ../mnist
	cp ../mnist/in/t10k-images-idx3-ubyte in
in/t10k-labels-idx1-ubyte:
	make -C ../mnist
	cp ../mnist/in/t10k-labels-idx1-ubyte in
