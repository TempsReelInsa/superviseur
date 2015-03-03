
all:
	make -C src

clean:
	make -C src clean

install: all
	./upload.sh $(ROBOT)  src/robot
