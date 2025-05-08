SKETCH = 6502_monitor_2

.PHONY: all clean dump upload compile

all: $(SKETCH)/hello-world-final.h upload

$(SKETCH)/hello-world-final.h: hello-world-final.bin
	hexdump -C hello-world-final.bin | python3 read_dump.py > $(SKETCH)/hello-world-final.h

hello-world-final.bin: hello-world-final.s
	vasm6502_oldstyle -Fbin -dotdir hello-world-final.s -o hello-world-final.bin

compile: $(SKETCH)/hello-world-final.h
	arduino-cli compile --fqbn arduino:avr:mega $(SKETCH)

upload: compile
	arduino-cli upload -p /dev/cu.usbmodem1101 --fqbn arduino:avr:mega $(SKETCH)

clean:
	rm -f $(SKETCH)/hello-world-final.h hello-world-final.bin