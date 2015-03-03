
ROBOT = 03

all:
	make -C src

clean:
	make -C src clean

install:
	./upload.sh $(ROBOT) src/robot
