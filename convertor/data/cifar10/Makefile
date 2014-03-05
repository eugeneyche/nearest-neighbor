default: directories in/cifar-10-batches-bin/data

directories:
	if [ ! -d in ]; then mkdir in; fi
	if [ ! -d out ]; then mkdir out; fi

clean:
	rm -f -r in/*
	rm -f out/*

new: clean default

in/cifar-10-binary.tar.gz:
	cd in; wget http://www.cs.toronto.edu/~kriz/cifar-10-binary.tar.gz
in/cifar-10-batches-bin/data: in/cifar-10-binary.tar.gz
	cd in; tar -xvf cifar-10-binary.tar.gz
	touch in/cifar-10-batches-bin/data
