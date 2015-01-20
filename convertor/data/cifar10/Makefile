BASE_URL := "http://www.cs.toronto.edu/~kriz"
PARTS := cifar-10-binary
TARGET := cifar-10-batches-bin

default:
	@for p in $(PARTS); do 		   \
		wget $(BASE_URL)/$$p.tar.gz && \
			 tar -xvf $$p.tar.gz  && \
			 rm -f $$p.tar.gz;     \
	done

clean:
	$(RM) $(TARGET)

new: clean default
