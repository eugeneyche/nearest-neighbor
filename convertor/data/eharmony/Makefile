BASE_URL := "http://bmcfee.github.io/data"
PARTS := EH-slice1-data.csv \
         EH-slice1-labels.csv \
         EH-slice2-data.csv \
         EH-slice2-labels.csv

default:
	@for p in $(PARTS); do 		   \
		wget $(BASE_URL)/$$p.gz && \
			 gunzip $$p.gz  && \
			 $(RM) $$p.gz;     \
	done

clean:
	rm -f $(PARTS)

new: clean default
