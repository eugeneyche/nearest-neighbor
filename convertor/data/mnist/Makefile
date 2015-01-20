BASE_URL := "http://yann.lecun.com/exdb/mnist"
PARTS := train-images-idx3-ubyte \
	     train-labels-idx1-ubyte \
		 t10k-images-idx3-ubyte  \
		 t10k-labels-idx1-ubyte

default:
	@for p in $(PARTS); do 		   \
		wget $(BASE_URL)/$$p.gz && \
			 gunzip $$p.gz  && \
			 $(RM) $$p.gz;     \
	done

clean:
	rm -f $(PARTS)

new: clean default
